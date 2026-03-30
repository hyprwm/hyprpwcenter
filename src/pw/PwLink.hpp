#pragma once

#include <vector>
#include <cstdint>

extern "C" {
#include <pipewire/pipewire.h>
#include <spa/utils/hook.h>
#include <spa/param/audio/raw.h>
}

#include "../helpers/Memory.hpp"

class IPwNode;

class CPipewireLink {
  public:
    CPipewireLink(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props);
    ~CPipewireLink();

    uint32_t          m_id = 0;

    uint32_t          m_nodeAID = 0, m_nodeBID = 0, m_portAID = 0, m_portBID = 0;
    pw_link*          m_proxy = nullptr;
    spa_hook          m_listener;

    WP<CPipewireLink> m_self;
};