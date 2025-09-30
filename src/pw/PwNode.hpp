#pragma once

#include <vector>
#include <cstdint>

#include "IPwNode.hpp"

extern "C" {
#include <pipewire/pipewire.h>
#include <spa/utils/hook.h>
}

#include "../helpers/Memory.hpp"

class CPipewireNode : public IPwNode {
  public:
    CPipewireNode(uint32_t id, uint32_t permissions, const char* type, uint32_t version, const spa_dict* props);
    ~CPipewireNode();

    virtual void setVolume(float x);
    virtual void setMute(bool x);

    pw_node*     m_proxy = nullptr;
    spa_hook     m_listener;

    size_t       m_volChannels = 0;

    // Thanks fox: some soundcards take a while to register the update
    // and get overloaded if you send too many.
    bool  m_deviceBusy   = false;
    float m_queuedVolume = -1.F;
};