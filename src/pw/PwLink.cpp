#include "PwLink.hpp"
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

static void onLinkInfo(void* data, const pw_link_info* info) {
    if (!info)
        return;

    CPipewireLink* link = (CPipewireLink*)data;

    if (const char* s = prop(info->props, PW_KEY_LINK_INPUT_NODE))
        link->m_nodeBID = std::stoi(s);
    if (const char* s = prop(info->props, PW_KEY_LINK_INPUT_PORT))
        link->m_portBID = std::stoi(s);
    if (const char* s = prop(info->props, PW_KEY_LINK_OUTPUT_NODE))
        link->m_nodeAID = std::stoi(s);
    if (const char* s = prop(info->props, PW_KEY_LINK_OUTPUT_PORT))
        link->m_portAID = std::stoi(s);

    if (!link->m_portAID || !link->m_portBID || !link->m_nodeAID || !link->m_nodeBID) {
        Debug::log(ERR, "Link {} is invalid: missing props", link->m_id);
        return;
    }

    g_ui->updateLink(link->m_self);
}

static const pw_link_events LINK_EVENTS = {
    .version = PW_VERSION_LINK_EVENTS,
    .info    = onLinkInfo,
};

CPipewireLink::CPipewireLink(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) : m_id(id) {

    m_proxy = rc<pw_link*>(pw_registry_bind(g_pipewire->m_pwState.registry, id, PW_TYPE_INTERFACE_Link, PW_VERSION_LINK, 0));

    spa_zero(m_listener);
    pw_link_add_listener(m_proxy, &m_listener, &LINK_EVENTS, this);
}

CPipewireLink::CPipewireLink(uint32_t nodeA, uint32_t nodeB, uint32_t portA, uint32_t portB) {
    pw_properties* props = pw_properties_new(PW_KEY_LINK_OUTPUT_NODE, std::to_string(nodeA).c_str(), PW_KEY_LINK_OUTPUT_PORT, std::to_string(portA).c_str(), PW_KEY_LINK_INPUT_NODE,
                                             std::to_string(nodeB).c_str(), PW_KEY_LINK_INPUT_PORT, std::to_string(portB).c_str(), PW_KEY_LINK_PASSIVE, "false", nullptr);

    auto           proxy = rc<pw_link*>(pw_core_create_object(g_pipewire->m_pwState.core, "link-factory", PW_TYPE_INTERFACE_Link, PW_VERSION_LINK, &props->dict, 0));
    pw_properties_free(props);

    if (!proxy)
        return;

    m_proxy = proxy;

    spa_zero(m_listener);
    pw_link_add_listener(m_proxy, &m_listener, &LINK_EVENTS, this);
}

CPipewireLink::~CPipewireLink() {
    if (g_ui)
        g_ui->removeLink(m_self);

    if (!m_proxy)
        return;

    pw_proxy_destroy(rc<pw_proxy*>(m_proxy));
}
