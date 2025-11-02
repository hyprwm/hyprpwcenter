#include "PwState.hpp"
#include "PwDevice.hpp"
#include "../helpers/Log.hpp"
#include "../ui/UI.hpp"

#include <algorithm>

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
}

static void onGlobal(void* userdata, uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) {
    g_pipewire->onGlobal(id, permissions, type, version, props);
}

static void onGlobalRemoved(void* userdata, uint32_t id) {
    g_pipewire->onGlobalRemoved(id);
}

static const pw_registry_events REGISTRY_EVENTS = {
    .version       = PW_VERSION_REGISTRY_EVENTS,
    .global        = ::onGlobal,
    .global_remove = ::onGlobalRemoved,
};

CPipewireState::CPipewireState(int argc, char** argv) {
    pw_init(&argc, &argv);

    m_pwState.loop    = pw_main_loop_new(nullptr);
    m_pwState.context = pw_context_new(pw_main_loop_get_loop(m_pwState.loop), nullptr, 0);
    m_pwState.core    = pw_context_connect(m_pwState.context, nullptr, 0);
    if (!m_pwState.core) {
        Debug::log(CRIT, "Couldn't connect to pw");
        exit(1);
    }

    m_pwState.registry = pw_core_get_registry(m_pwState.core, PW_VERSION_REGISTRY, 0);
    spa_zero(m_pwState.registryListener);
    pw_registry_add_listener(m_pwState.registry, &m_pwState.registryListener, &REGISTRY_EVENTS, nullptr);
}

CPipewireState::~CPipewireState() {
    m_pwState.nodes.clear();

    if (m_pwState.registry)
        pw_proxy_destroy(reinterpret_cast<pw_proxy*>(m_pwState.registry));
    if (m_pwState.core)
        pw_core_disconnect(m_pwState.core);
    if (m_pwState.context)
        pw_context_destroy(m_pwState.context);
    if (m_pwState.loop)
        pw_main_loop_destroy(m_pwState.loop);
}

int CPipewireState::getFd() {
    return pw_loop_get_fd(pw_main_loop_get_loop(m_pwState.loop));
}

void CPipewireState::dispatch() {
    while (pw_loop_iterate(pw_main_loop_get_loop(m_pwState.loop), 0) != 0) {
        ;
    }
}

void CPipewireState::onGlobal(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props) {
    const auto SV = std::string_view{type};
    if (SV == PW_TYPE_INTERFACE_Node) {
        auto x    = m_pwState.nodes.emplace_back(makeShared<CPipewireNode>(id, permissions, type, version, props));
        x->m_self = x;
        checkNodePorts(x);
    } else if (SV == PW_TYPE_INTERFACE_Device) {
        auto x    = m_pwState.devices.emplace_back(makeShared<CPipewireDevice>(id, permissions, type, version, props));
        x->m_self = x;
    } else if (SV == PW_TYPE_INTERFACE_Port) {
        auto x    = m_pwState.ports.emplace_back(makeShared<CPipewirePort>(id, permissions, type, version, props));
        x->m_self = x;
        addPortToNode(x);
    } else if (SV == PW_TYPE_INTERFACE_Link) {
        auto x    = m_pwState.links.emplace_back(makeShared<CPipewireLink>(id, permissions, type, version, props));
        x->m_self = x;
    }
}

// If node goes away, drop all ports and links that belonged to it
void CPipewireState::onGlobalRemoved(uint32_t id) {
    // Remove links
    std::erase_if(m_pwState.links, [id](const auto& l) { return l->m_id == id || l->m_nodeAID == id || l->m_nodeBID == id; });
    // Remove ports
    std::erase_if(m_pwState.ports, [id](const auto& p) { return p->m_id == id || p->m_nodeID == id; });
    // Remove devices
    std::erase_if(m_pwState.devices, [id](const auto& d) { return d->m_id == id; });
    // Remove node itself
    std::erase_if(m_pwState.nodes, [id](const auto& n) { return n->m_id == id; });
}

void CPipewireState::setVolume(uint32_t id, float x) {
    for (const auto& n : m_pwState.nodes) {
        if (n->m_id != id)
            continue;

        n->setVolume(x);
        break;
    }
}

void CPipewireState::setMuted(uint32_t id, bool x) {
    for (const auto& n : m_pwState.nodes) {
        if (n->m_id != id)
            continue;

        n->setMute(x);
        break;
    }
}

void CPipewireState::setMode(uint32_t id, size_t mode) {
    for (const auto& d : m_pwState.devices) {
        if (d->m_id != id)
            continue;

        d->setMode(mode);
        break;
    }
}

void CPipewireState::addPortToNode(WP<CPipewirePort> port) {
    for (const auto& n : m_pwState.nodes) {
        if (n->m_id != port->m_nodeID)
            continue;

        if (std::ranges::contains(n->m_ports, port))
            break;

        n->m_ports.emplace_back(port);
        port->m_node = n;
        g_ui->updateNode(n);
    }
}

void CPipewireState::checkNodePorts(WP<IPwNode> node) {
    for (const auto& p : m_pwState.ports) {
        if (p->m_nodeID != node->m_id)
            continue;

        // There might be more matching ports so continue
        if (std::ranges::contains(node->m_ports, p))
            continue;

        node->m_ports.emplace_back(p);
        g_ui->updateNode(node);
    }
}

void CPipewireState::linkOrUnlink(WP<IPwNode> a, WP<IPwNode> b, uint32_t portA, uint32_t portB) {
    auto it = std::ranges::find_if(
        m_pwState.links, [a, b, portA, portB](const auto& l) { return l->m_nodeAID == a->m_id && l->m_nodeBID == b->m_id && l->m_portAID == portA && l->m_portBID == portB; });

    if (it != m_pwState.links.end()) {
        m_pwState.links.erase(it);
        return;
    }

    auto x    = m_pwState.links.emplace_back(makeShared<CPipewireLink>(a->m_id, b->m_id, portA, portB));
    x->m_self = x;
}
