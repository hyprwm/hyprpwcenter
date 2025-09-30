#include "PwPort.hpp"
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
#include <spa/param/audio/format-utils.h>
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

static void onPortInfo(void* data, const pw_port_info* info) {
    if (!info)
        return;

    CPipewirePort* port = (CPipewirePort*)data;

    port->m_output = info->direction != PW_DIRECTION_INPUT;

    if (info->change_mask & PW_PORT_CHANGE_MASK_PARAMS) {
        for (uint32_t i = 0; i < info->n_params; ++i) {
            const auto& pi = info->params[i];
            pw_port_enum_params(port->m_proxy, 0, pi.id, 0, UINT32_MAX, nullptr);
        }
    }

    if (info->props) {
        const char* ch = spa_dict_lookup(info->props, PW_KEY_AUDIO_CHANNEL);
        if (ch && *ch) {
            port->m_channels.clear();
            port->m_channels.emplace_back(sc<spa_audio_channel>(0), ch);
        }
    }
}

static void onPortParam(void* data, int seq, uint32_t id, uint32_t index, uint32_t next, const spa_pod* param) {
    CPipewirePort*      port = (CPipewirePort*)data;

    const spa_pod_prop* p;
    spa_pod_object*     obj = (spa_pod_object*)param;

    if (id == SPA_PARAM_Format || id == SPA_PARAM_EnumFormat) {
        struct spa_audio_info info;

        spa_format_audio_parse(param, &info);

        if (info.media_type == SPA_MEDIA_TYPE_audio) {
            if (info.media_subtype == SPA_MEDIA_SUBTYPE_raw) {
                struct spa_audio_info info;
                if (spa_format_audio_parse(param, &info) == 0) {
                    const struct spa_audio_info_raw* raw = &info.info.raw;
                    port->m_channels.resize(raw->channels);
                    for (uint32_t i = 0; i < raw->channels; ++i) {
                        port->m_channels[i] = {sc<spa_audio_channel>(raw->position[i]), ""};
                    }
                }
            } else if (info.media_subtype == SPA_MEDIA_SUBTYPE_dsp) {
                // DSP ports are mono, and the data is sent in onPortInfo
                if (port->m_channels.empty()) // fallback
                    port->m_channels.emplace_back(SPA_AUDIO_CHANNEL_MONO, "");
            }

            // in other cases we're fucked AFAIK.
        }
    } else if (id == SPA_PARAM_Props) {
        SPA_POD_OBJECT_FOREACH(obj, p) {
            if (p->key == SPA_PROP_channelMap) {
                if (spa_pod_is_array(&p->value)) {
                    const spa_pod_array* arr = rc<const spa_pod_array*>(&p->value);
                    uint32_t             n   = SPA_POD_ARRAY_N_VALUES(arr);
                    const uint32_t*      ids = rc<const uint32_t*>(SPA_POD_ARRAY_VALUES(arr));

                    port->m_channels.clear();
                    port->m_channels.resize(n);
                    for (size_t i = 0; i < n; ++i) {
                        port->m_channels.at(i) = {sc<spa_audio_channel>(ids[i]), ""};
                    }
                }
                continue;
            }
        }
    }

    if (port->m_node)
        g_ui->updateNode(port->m_node);
}

static const pw_port_events PORT_EVENTS = {
    .version = PW_VERSION_PORT_EVENTS,
    .info    = onPortInfo,
    .param   = onPortParam,
};

CPipewirePort::CPipewirePort(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) : m_id(id) {

    const char* nid = prop(props, PW_KEY_NODE_ID);

    if (nid) {
        try {
            m_nodeID = std::stoi(nid);
        } catch (...) { Debug::log(ERR, "pw: port has node id of {}, which isnt an int?", nid); }
    }

    m_proxy = rc<pw_port*>(pw_registry_bind(g_pipewire->m_pwState.registry, id, PW_TYPE_INTERFACE_Port, PW_VERSION_PORT, 0));

    spa_zero(m_listener);
    pw_port_add_listener(m_proxy, &m_listener, &PORT_EVENTS, this);

    pw_port_enum_params(m_proxy, 0, SPA_PARAM_Props, 0, UINT32_MAX, nullptr);

    uint32_t ids[] = {SPA_PARAM_Props};
    pw_port_subscribe_params(m_proxy, ids, 1);
}

CPipewirePort::~CPipewirePort() {
    if (!m_proxy)
        return;

    pw_proxy_destroy(rc<pw_proxy*>(m_proxy));
}
