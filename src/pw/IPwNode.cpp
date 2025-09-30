#include "IPwNode.hpp"

const char* IPwNode::getNameForChannel(spa_audio_channel c) {
    switch (c) {
        case SPA_AUDIO_CHANNEL_UNKNOWN: return "Unknown";
        case SPA_AUDIO_CHANNEL_NA: return "N/A";
        case SPA_AUDIO_CHANNEL_FL: return "FL";
        case SPA_AUDIO_CHANNEL_FR: return "FR";
        case SPA_AUDIO_CHANNEL_FC: return "FC";
        case SPA_AUDIO_CHANNEL_LFE: return "LFE";
        case SPA_AUDIO_CHANNEL_SL: return "SL";
        case SPA_AUDIO_CHANNEL_FLC: return "FLC";
        case SPA_AUDIO_CHANNEL_FRC: return "FRC";
        case SPA_AUDIO_CHANNEL_RC: return "RC";
        case SPA_AUDIO_CHANNEL_RL: return "RL";
        case SPA_AUDIO_CHANNEL_RR: return "RR";
        case SPA_AUDIO_CHANNEL_TC: return "TC";
        case SPA_AUDIO_CHANNEL_TFL: return "TFL";

        default: break;
    }
    return "lazy";
}