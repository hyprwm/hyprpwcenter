#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../helpers/Memory.hpp"
#include "PwPort.hpp"

extern "C" {
#include <spa/param/audio/raw.h>
}

class IPwNode {
  public:
    virtual ~IPwNode() = default;

    virtual void                   setVolume(float x, bool force = false) = 0;
    virtual void                   setMute(bool x)                        = 0;
    virtual bool                   controllable()                         = 0;

    static const char*             getNameForChannel(spa_audio_channel);

    uint32_t                       m_id = 0;
    std::string                    m_name, m_mediaClass;

    float                          m_volume = 0;
    bool                           m_muted  = false;
    bool                           m_isApp  = false;

    std::vector<WP<CPipewirePort>> m_ports;

    WP<IPwNode>                    m_self;
};
