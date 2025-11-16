#include "Engine.hpp"

static Hyprutils::I18n::CI18nEngine engine;
static std::string                  engineLocale;

//
void I18n::initEngine() {
    engine.setFallbackLocale("en_US");
    engineLocale = engine.getSystemLocale().locale();

    // en_US (English)
    engine.registerEntry("en_US", TXT_KEY_PW_CENTER_TITLE, "Pipewire Control Center");

    engine.registerEntry("en_US", TXT_KEY_BUTTON_APPS, "Apps");
    engine.registerEntry("en_US", TXT_KEY_BUTTON_NODES, "Nodes");
    engine.registerEntry("en_US", TXT_KEY_BUTTON_INPUTS, "Inputs");
    engine.registerEntry("en_US", TXT_KEY_BUTTON_CONFIGURATION, "Configuration");
    engine.registerEntry("en_US", TXT_KEY_BUTTON_GRAPH, "Graph");

    engine.registerEntry("en_US", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 port";
        return "{count} ports";
    });
    engine.registerEntry("en_US", TXT_KEY_GRAPH_PURE_INPUTS, "Pure Inputs");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Active Inputs");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_UNCONNECTED_IO, "Unconnected IO");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_ACTIVE_IO, "Active IO");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Active Outputs");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_PURE_OUTPUTS, "Pure Outputs");
    //

    // id_ID (Indonesian)
    engine.registerEntry("id_ID", TXT_KEY_PW_CENTER_TITLE, "Pusat Kontrol Pipewire");

    engine.registerEntry("id_ID", TXT_KEY_BUTTON_APPS, "Aplikasi");
    engine.registerEntry("id_ID", TXT_KEY_BUTTON_NODES, "Node");
    engine.registerEntry("id_ID", TXT_KEY_BUTTON_INPUTS, "Input");
    engine.registerEntry("id_ID", TXT_KEY_BUTTON_CONFIGURATION, "Konfigurasi");
    engine.registerEntry("id_ID", TXT_KEY_BUTTON_GRAPH, "Graf");

    engine.registerEntry("id_ID", TXT_KEY_GRAPH_N_PORTS, "{count} port");
    engine.registerEntry("id_ID", TXT_KEY_GRAPH_PURE_INPUTS, "Input Murni");
    engine.registerEntry("id_ID", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Input Aktif");
    engine.registerEntry("id_ID", TXT_KEY_GRAPH_UNCONNECTED_IO, "I/O Tidak Terhubung");
    engine.registerEntry("id_ID", TXT_KEY_GRAPH_ACTIVE_IO, "I/O Aktif");
    engine.registerEntry("id_ID", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Output Aktif");
    engine.registerEntry("id_ID", TXT_KEY_GRAPH_PURE_OUTPUTS, "Output Murni");

    // ja_JP (Japanese)
    engine.registerEntry("ja_JP", TXT_KEY_PW_CENTER_TITLE, "Pipewireコントロールセンター");

    engine.registerEntry("ja_JP", TXT_KEY_BUTTON_APPS, "アプリ");
    engine.registerEntry("ja_JP", TXT_KEY_BUTTON_NODES, "ノード");
    engine.registerEntry("ja_JP", TXT_KEY_BUTTON_INPUTS, "入力");
    engine.registerEntry("ja_JP", TXT_KEY_BUTTON_CONFIGURATION, "設定");
    engine.registerEntry("ja_JP", TXT_KEY_BUTTON_GRAPH, "グラフ");

    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_N_PORTS, "{count}ポート");
    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_PURE_INPUTS, "入力のみ");
    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_ACTIVE_INPUTS, "アクティブ入力");
    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_UNCONNECTED_IO, "接続なし入出力");
    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_ACTIVE_IO, "アクティブ入出力");
    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "アクティブ出力");
    engine.registerEntry("ja_JP", TXT_KEY_GRAPH_PURE_OUTPUTS, "出力のみ");
    //

    // nl_NL (Dutch)
    engine.registerEntry("nl_NL", TXT_KEY_PW_CENTER_TITLE, "Pipewire Controle Centrum");

    engine.registerEntry("nl_NL", TXT_KEY_BUTTON_APPS, "Applicaties");
    engine.registerEntry("nl_NL", TXT_KEY_BUTTON_NODES, "Nodes");
    engine.registerEntry("nl_NL", TXT_KEY_BUTTON_INPUTS, "Inputs");
    engine.registerEntry("nl_NL", TXT_KEY_BUTTON_CONFIGURATION, "Configuratie");
    engine.registerEntry("nl_NL", TXT_KEY_BUTTON_GRAPH, "Grafiek");

    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 poort";
        return "{count} poorten";
    });
    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_PURE_INPUTS, "Pure inputs");
    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Actieve inputs");
    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_UNCONNECTED_IO, "Niet verbonden I/O");
    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_ACTIVE_IO, "Actieve I/O");
    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Actieve outputs");
    engine.registerEntry("nl_NL", TXT_KEY_GRAPH_PURE_OUTPUTS, "Pure outputs");
    //

    // pl_PL (Polish)
    engine.registerEntry("pl_PL", TXT_KEY_PW_CENTER_TITLE, "Centrum Sterowania Pipewire");

    engine.registerEntry("pl_PL", TXT_KEY_BUTTON_APPS, "Aplikacje");
    engine.registerEntry("pl_PL", TXT_KEY_BUTTON_NODES, "Node'y");
    engine.registerEntry("pl_PL", TXT_KEY_BUTTON_INPUTS, "Wejścia");
    engine.registerEntry("pl_PL", TXT_KEY_BUTTON_CONFIGURATION, "Ustawienia");
    engine.registerEntry("pl_PL", TXT_KEY_BUTTON_GRAPH, "Graf");

    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 port";
        int last    = count % 10;
        int lastTwo = count % 100;
        if (last >= 2 && last <= 4 && (lastTwo < 12 || lastTwo > 14))
            return "{count} porty";
        return "{count} portów";
    });
    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_PURE_INPUTS, "Czyste wejścia");
    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Aktywne wejścia");
    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_UNCONNECTED_IO, "Niepodłączone wejścia/wyjścia");
    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_ACTIVE_IO, "Aktywne wejścia/wyjścia");
    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Aktywne wyjścia");
    engine.registerEntry("pl_PL", TXT_KEY_GRAPH_PURE_OUTPUTS, "Czyste wyjścia");
    //
}

std::string I18n::localize(eTextKeys key, const Hyprutils::I18n::translationVarMap& vars) {
    return engine.localizeEntry(engineLocale, key, vars);
}
