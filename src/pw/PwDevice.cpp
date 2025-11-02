#include "PwDevice.hpp"
#include "PwState.hpp"
#include "../ui/UI.hpp"
#include "../helpers/Log.hpp"

extern "C" {
#include <pipewire/pipewire.h>
#include <spa/utils/defs.h>
#include <spa/utils/hook.h>
#include <spa/utils/result.h>
#include <spa/param/param.h>
#include <spa/param/props.h>
#include <spa/pod/iter.h>
#include <spa/pod/parser.h>
#include <spa/pod/pod.h>
#include <spa/pod/builder.h>
}

static const char* prop(const spa_dict* props, const char* key) {
    if (!props)
        return nullptr;
    const spa_dict_item* it;
    spa_dict_for_each(it, props) {
        if (std::string_view(it->key) == key)
            return it->value;
    }
    return nullptr;
}

static void onDeviceInfo(void* data, const pw_device_info* info) {
    if (!info)
        return;

    CPipewireDevice* dev = (CPipewireDevice*)data;

    if (info->change_mask & PW_DEVICE_CHANGE_MASK_PARAMS) {
        for (uint32_t i = 0; i < info->n_params; i++) {
            const auto& p = info->params[i];
            if (p.id == SPA_PARAM_EnumProfile || p.id == SPA_PARAM_Profile) {
                uint32_t ids[] = {SPA_PARAM_EnumProfile, SPA_PARAM_Profile};
                pw_device_subscribe_params(dev->m_proxy, ids, 2);
                pw_device_enum_params(dev->m_proxy, 0, SPA_PARAM_EnumProfile, 0, UINT32_MAX, nullptr);
                pw_device_enum_params(dev->m_proxy, 0, SPA_PARAM_Profile, 0, UINT32_MAX, nullptr);
            }
        }
    }
}

static void onDeviceParam(void* data, int seq, uint32_t id, uint32_t index, uint32_t next, const spa_pod* param) {
    CPipewireDevice*    dev = (CPipewireDevice*)data;

    const spa_pod_prop* p;
    spa_pod_object*     obj = (spa_pod_object*)param;

    int32_t             profileIdx = INT32_MAX;
    std::string         desc;

    SPA_POD_OBJECT_FOREACH(obj, p) {
        if (p->key == SPA_PARAM_PROFILE_index) {
            spa_pod_get_int(&p->value, &profileIdx);
            continue;
        }

        if (p->key == SPA_PARAM_PROFILE_description) {
            const char* x = "";
            spa_pod_get_string(&p->value, &x);
            desc = x;
            continue;
        }
    }

    if (profileIdx == INT32_MAX)
        return;

    Debug::log(TRACE, "device {}: update profile id {}", dev->m_id, profileIdx);

    // isEnum == true: we’re processing list of possible profiles
    // isEnum == false: we’re processing current profile
    const bool isEnum = (id == SPA_PARAM_EnumProfile);

    // We keep 2 parallel vectors on the device:
    //  m_modes[i]: user-facing name
    //  m_modePwIdx[i]: PW index
    uint32_t uiIdx = UINT32_MAX;
    // Figure out if we already have UI slot for this PipeWire profile index
    for (uint32_t i = 0; i < dev->m_modePwIdx.size(); ++i) {
        if (dev->m_modePwIdx[i] == (uint32_t)profileIdx) {
            uiIdx = i;
            break;
        }
    }

    // If we didn't find an existing UI slot, we need to create a new entry
    //  in both arrays to represent this newly seen profile
    if (uiIdx == UINT32_MAX) {
        // New entry will go at the end
        uiIdx = dev->m_modes.size();
        // Grow the names vector, we'll fill it later
        dev->m_modes.emplace_back();
        // Record real PipeWire profile index
        dev->m_modePwIdx.push_back((uint32_t)profileIdx);
    }

    // At this point uiIdx points to the right UI slot
    dev->m_modes[uiIdx] = desc;

    // If this is "current profile", remember ui index
    if (!isEnum)
        dev->m_currentMode = uiIdx;

    g_ui->updateDevice(dev->m_self);
}

static const pw_device_events DEVICE_EVENTS = {
    .version = PW_VERSION_DEVICE_EVENTS,
    .info    = onDeviceInfo,
    .param   = onDeviceParam,
};

CPipewireDevice::CPipewireDevice(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) : m_id(id) {
    auto        mc  = prop(props, PW_KEY_MEDIA_CLASS);
    const char* nm  = prop(props, PW_KEY_DEVICE_NAME);
    const char* dsc = prop(props, PW_KEY_DEVICE_NICK);
    if (!mc)
        return;
    if (!std::string_view{mc}.starts_with("Audio/Device"))
        return;

    m_name       = dsc ? dsc : (nm ? nm : "");
    m_mediaClass = mc ? mc : "";
    m_proxy      = sc<pw_device*>(pw_registry_bind(g_pipewire->m_pwState.registry, id, PW_TYPE_INTERFACE_Device, PW_VERSION_DEVICE, 0));

    spa_zero(m_listener);
    pw_device_add_listener(m_proxy, &m_listener, &DEVICE_EVENTS, this);

    pw_device_enum_params(m_proxy, 0, SPA_PARAM_Props, 0, UINT32_MAX, nullptr);

    uint32_t ids[] = {SPA_PARAM_Props};
    pw_device_subscribe_params(m_proxy, ids, 1);
}

CPipewireDevice::~CPipewireDevice() {
    if (g_ui)
        g_ui->deviceRemoved(m_self);

    if (!m_proxy)
        return;

    pw_proxy_destroy(rc<pw_proxy*>(m_proxy));
}

void CPipewireDevice::setMode(size_t x) {
    if (x >= m_modes.size() || x >= m_modePwIdx.size())
        return;

    // Translate from UI index to real PipeWire profile index
    uint32_t pwIdx = m_modePwIdx[x];

    uint8_t         buf[256];
    spa_pod_builder b    = SPA_POD_BUILDER_INIT(buf, sizeof(buf));
    const spa_pod*  prof = rc<spa_pod*>(
        spa_pod_builder_add_object(
            &b,
            SPA_TYPE_OBJECT_ParamProfile,   SPA_PARAM_Profile,
            SPA_PARAM_PROFILE_index,        SPA_POD_Int((int)pwIdx),
            SPA_PARAM_PROFILE_save,         SPA_POD_Bool(true)));

    pw_device_set_param(m_proxy, SPA_PARAM_Profile, 0, prof);
}
