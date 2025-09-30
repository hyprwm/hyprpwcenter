#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "../helpers/Memory.hpp"

extern "C" {
#include <spa/param/audio/raw.h>
}

class IPwNode {
  public:
    virtual ~IPwNode() = default;

    virtual void                   setVolume(float x) = 0;
    virtual void                   setMute(bool x)    = 0;

    static const char*             getNameForChannel(spa_audio_channel);

    uint32_t                       m_id = 0;
    std::string                    m_name, m_mediaClass;

    float                          m_volume = 0;
    bool                           m_muted  = false;
    bool                           m_isApp  = false;

    std::vector<spa_audio_channel> m_channelsOut;

    WP<IPwNode>                    m_self;
};
