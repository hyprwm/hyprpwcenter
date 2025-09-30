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

class CPipewirePort {
  public:
    CPipewirePort(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props);
    ~CPipewirePort();

    uint32_t                                               m_id = 0, m_nodeID = 0;
    pw_port*                                               m_proxy = nullptr;
    spa_hook                                               m_listener;
    bool                                                   m_output = true;

    std::vector<std::pair<spa_audio_channel, std::string>> m_channels;

    WP<CPipewirePort>                                      m_self;
    WP<IPwNode>                                            m_node;
};