#pragma once

#include <vector>
#include <cstdint>

#include "IPwNode.hpp"

extern "C" {
#include <pipewire/pipewire.h>
#include <spa/utils/hook.h>
}

#include "../helpers/Memory.hpp"

class CPipewireDevice {
  public:
    CPipewireDevice(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props);
    ~CPipewireDevice();

    void                     setMode(size_t x);

    std::string              m_name, m_mediaClass;
    uint32_t                 m_id    = 0;
    pw_device*               m_proxy = nullptr;
    spa_hook                 m_listener;
    std::vector<std::string> m_modes;
    size_t                   m_currentMode = 0;

    WP<CPipewireDevice>      m_self;

    // maps UI index to real PipeWire profile index
    std::vector<uint32_t> m_modePwIdx;
};