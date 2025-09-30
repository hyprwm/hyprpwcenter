#include "PwNode.hpp"
#include "PwState.hpp"
#include "../ui/UI.hpp"
#include "../helpers/Log.hpp"
#include "PwConstants.hpp"

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
#include <spa/param/audio/raw.h>
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

static void onNodeInfo(void* data, const pw_node_info* info) {
    ;
}

static void onNodeParam(void* data, int seq, uint32_t id, uint32_t index, uint32_t next, const spa_pod* param) {
    CPipewireNode*      node = (CPipewireNode*)data;

    const spa_pod_prop* p;
    spa_pod_object*     obj = (spa_pod_object*)param;

    SPA_POD_OBJECT_FOREACH(obj, p) {
        Debug::log(LOG, "Node {} -> has prop {}", node->m_name, p->key);

        if (p->key == SPA_PROP_channelVolumes) {
            if (spa_pod_is_array(&p->value)) {
                uint32_t     n = SPA_POD_ARRAY_N_VALUES(&p->value);
                const float* v = rc<const float*>(SPA_POD_ARRAY_VALUES(&p->value));

                if (n == 0)
                    continue;

                node->m_volume = v[0];
            }

            node->m_deviceBusy = false;

            if (node->m_queuedVolume >= 0.F)
                node->setVolume(node->m_queuedVolume);

            continue;
        }

        if (p->key == SPA_PROP_volume && node->m_isApp) {
            if (spa_pod_is_float(&p->value))
                spa_pod_get_float(&p->value, &node->m_volume);

            continue;
        }

        if (p->key == SPA_PROP_softMute) {
            if (spa_pod_is_bool(&p->value))
                spa_pod_get_bool(&p->value, &node->m_muted);

            continue;
        }

        if (p->key == SPA_PROP_channelMap) {
            if (spa_pod_is_array(&p->value)) {
                const spa_pod_array* arr = rc<const spa_pod_array*>(&p->value);
                uint32_t             n   = SPA_POD_ARRAY_N_VALUES(arr);
                const uint32_t*      ids = rc<const uint32_t*>(SPA_POD_ARRAY_VALUES(arr));

                node->m_channelsOut.clear();
                node->m_channelsOut.resize(n);
                for (size_t i = 0; i < n; ++i) {
                    node->m_channelsOut.at(i) = sc<spa_audio_channel>(ids[i]);
                }
            }
            continue;
        }
    }

    g_ui->updateNode(node->m_self);
}

static const pw_node_events NODE_EVENTS = {
    .version = PW_VERSION_NODE_EVENTS,
    .info    = onNodeInfo,
    .param   = onNodeParam,
};

CPipewireNode::CPipewireNode(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) {
    m_id = id;

    auto        mc  = prop(props, PW_KEY_MEDIA_CLASS);
    const char* nm  = prop(props, PW_KEY_NODE_NAME);
    const char* dsc = prop(props, PW_KEY_NODE_DESCRIPTION);
    if (!mc)
        return;
    if (!std::string_view{mc}.starts_with("Audio/") && !std::string_view{mc}.starts_with("Stream/"))
        return;

    if (std::string_view{mc}.starts_with("Stream/"))
        m_isApp = true;

    m_name       = dsc ? dsc : (nm ? nm : "");
    m_mediaClass = mc ? mc : "";
    m_proxy      = sc<pw_node*>(pw_registry_bind(g_pipewire->m_pwState.registry, id, PW_TYPE_INTERFACE_Node, PW_VERSION_NODE, 0));

    spa_zero(m_listener);
    pw_node_add_listener(m_proxy, &m_listener, &NODE_EVENTS, this);

    pw_node_enum_params(m_proxy, 0, SPA_PARAM_Props, 0, UINT32_MAX, nullptr);

    uint32_t ids[] = {SPA_PARAM_Props};
    pw_node_subscribe_params(m_proxy, ids, 1);
}

CPipewireNode::~CPipewireNode() {
    if (g_ui)
        g_ui->nodeRemoved(m_self);

    if (!m_proxy)
        return;

    pw_proxy_destroy(rc<pw_proxy*>(m_proxy));
}

void CPipewireNode::setVolume(float x) {
    if (std::abs(x - m_volume) < PW_VOLUME_EPSILON)
        return;

    if (m_deviceBusy) {
        m_queuedVolume = x;
        return;
    }

    std::vector<float> volumes;
    volumes.resize(m_channelsOut.size());
    for (size_t i = 0; i < m_channelsOut.size(); i++) {
        volumes.at(i) = x;
    }

    uint8_t         buffer[1024];
    spa_pod_builder builder;
    spa_pod_builder_init(&builder, buffer, sizeof(buffer));

    struct spa_pod_frame object_frame;
    spa_pod_builder_push_object(&builder, &object_frame, SPA_TYPE_OBJECT_Props, SPA_PARAM_Props);

    spa_pod_builder_prop(&builder, SPA_PROP_channelVolumes, 0);
    spa_pod_builder_array(&builder, sizeof(float), SPA_TYPE_Float, volumes.size(), volumes.data());

    const spa_pod* pod = rc<spa_pod*>(spa_pod_builder_pop(&builder, &object_frame));

    pw_node_set_param(m_proxy, SPA_PARAM_Props, 0, pod);

    m_deviceBusy   = true;
    m_queuedVolume = -1.F;
}

void CPipewireNode::setMute(bool x) {
    uint8_t         buffer[1024];
    spa_pod_builder builder;
    spa_pod_builder_init(&builder, buffer, sizeof(buffer));

    struct spa_pod_frame object_frame;
    spa_pod_builder_push_object(&builder, &object_frame, SPA_TYPE_OBJECT_Props, SPA_PARAM_Props);

    spa_pod_builder_prop(&builder, SPA_PROP_softMute, 0);
    spa_pod_builder_bool(&builder, x);

    const spa_pod* pod = rc<spa_pod*>(spa_pod_builder_pop(&builder, &object_frame));

    pw_node_set_param(m_proxy, SPA_PARAM_Props, 0, pod);

    m_muted = x;
    g_ui->updateNode(m_self);
}
