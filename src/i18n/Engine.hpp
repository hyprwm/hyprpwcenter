#pragma once

#include <cstdint>
#include <string>
#include <hyprutils/i18n/I18nEngine.hpp>

namespace I18n {
    //NOLINTNEXTLINE
    enum eTextKeys : uint64_t {
        TXT_KEY_PW_CENTER_TITLE = 0,

        TXT_KEY_BUTTON_APPS,
        TXT_KEY_BUTTON_NODES,
        TXT_KEY_BUTTON_INPUTS,
        TXT_KEY_BUTTON_CONFIGURATION,
        TXT_KEY_BUTTON_GRAPH,

        TXT_KEY_GRAPH_N_PORTS,
        TXT_KEY_GRAPH_PURE_INPUTS,
        TXT_KEY_GRAPH_ACTIVE_INPUTS,
        TXT_KEY_GRAPH_UNCONNECTED_IO,
        TXT_KEY_GRAPH_ACTIVE_IO,
        TXT_KEY_GRAPH_ACTIVE_OUTPUTS,
        TXT_KEY_GRAPH_PURE_OUTPUTS,
    };

    void        initEngine();
    std::string localize(eTextKeys key, const Hyprutils::I18n::translationVarMap& vars = {});
};
