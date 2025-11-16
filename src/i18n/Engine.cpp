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
    engine.registerEntry("en_US", TXT_KEY_GRAPH_UNCONNECTED_IO, "Unconnected I/O");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_ACTIVE_IO, "Active I/O");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Active Outputs");
    engine.registerEntry("en_US", TXT_KEY_GRAPH_PURE_OUTPUTS, "Pure Outputs");

    // it_IT (Italian)
    engine.registerEntry("it_IT", TXT_KEY_PW_CENTER_TITLE, "Impostazioni di Pipewire");

    engine.registerEntry("it_IT", TXT_KEY_BUTTON_APPS, "App");
    engine.registerEntry("it_IT", TXT_KEY_BUTTON_NODES, "Uscite");
    engine.registerEntry("it_IT", TXT_KEY_BUTTON_INPUTS, "Ingressi");
    engine.registerEntry("it_IT", TXT_KEY_BUTTON_CONFIGURATION, "Configurazione");
    engine.registerEntry("it_IT", TXT_KEY_BUTTON_GRAPH, "Grafo");

    engine.registerEntry("it_IT", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 porta";
        return "{count} porte";
    });
    engine.registerEntry("it_IT", TXT_KEY_GRAPH_PURE_INPUTS, "Ingressi puri");
    engine.registerEntry("it_IT", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Ingressi attivi");
    engine.registerEntry("it_IT", TXT_KEY_GRAPH_UNCONNECTED_IO, "I/O non connesso");
    engine.registerEntry("it_IT", TXT_KEY_GRAPH_ACTIVE_IO, "I/O Attivo");
    engine.registerEntry("it_IT", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Usicte attive");
    engine.registerEntry("it_IT", TXT_KEY_GRAPH_PURE_OUTPUTS, "Uscite non connesse");

    // fi_FI (Finnish)
    engine.registerEntry("fi_FI", TXT_KEY_PW_CENTER_TITLE, "Pipewire-ohjauskeskus");

    engine.registerEntry("fi_FI", TXT_KEY_BUTTON_APPS, "Sovellukset");
    engine.registerEntry("fi_FI", TXT_KEY_BUTTON_NODES, "Solmut");
    engine.registerEntry("fi_FI", TXT_KEY_BUTTON_INPUTS, "Sisääntulot");
    engine.registerEntry("fi_FI", TXT_KEY_BUTTON_CONFIGURATION, "Asetukset");
    engine.registerEntry("fi_FI", TXT_KEY_BUTTON_GRAPH, "Kaavio");

    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 portti";
        return "{count} porttia";
    });
    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_PURE_INPUTS, "Vain Sisääntulot");
    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Aktiiviset Sisääntulot");
    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_UNCONNECTED_IO, "Kytkemättömät Siirrännät");
    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_ACTIVE_IO, "Aktiiviset Siirrännät");
    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Aktiiviset Ulostulot");
    engine.registerEntry("fi_FI", TXT_KEY_GRAPH_PURE_OUTPUTS, "Vain Ulostulot");
    //

    // fr_FR (French)
    engine.registerEntry("fr_FR", TXT_KEY_PW_CENTER_TITLE, "Centre de contrôle Pipewire");

    engine.registerEntry("fr_FR", TXT_KEY_BUTTON_APPS, "Applications");
    engine.registerEntry("fr_FR", TXT_KEY_BUTTON_NODES, "Noeuds");
    engine.registerEntry("fr_FR", TXT_KEY_BUTTON_INPUTS, "Entrées");
    engine.registerEntry("fr_FR", TXT_KEY_BUTTON_CONFIGURATION, "Configuration");
    engine.registerEntry("fr_FR", TXT_KEY_BUTTON_GRAPH, "Graphe");

    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 port";
        return "{count} ports";
    });
    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_PURE_INPUTS, "Entrées simples");
    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Entrées actives");
    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_UNCONNECTED_IO, "E/S non connectées");
    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_ACTIVE_IO, "E/S actives");
    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Sorties actives");
    engine.registerEntry("fr_FR", TXT_KEY_GRAPH_PURE_OUTPUTS, "Sorties simples");

    // hu_HU (Hungarian)
    engine.registerEntry("hu_HU", TXT_KEY_PW_CENTER_TITLE, "PipeWire Vezérlőközpont");

    engine.registerEntry("hu_HU", TXT_KEY_BUTTON_APPS, "Alkalmazások");
    engine.registerEntry("hu_HU", TXT_KEY_BUTTON_NODES, "Csomópontok");
    engine.registerEntry("hu_HU", TXT_KEY_BUTTON_INPUTS, "Bemenetek");
    engine.registerEntry("hu_HU", TXT_KEY_BUTTON_CONFIGURATION, "Beállítások");
    engine.registerEntry("hu_HU", TXT_KEY_BUTTON_GRAPH, "Gráf");

    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_N_PORTS, "{count} port");
    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_PURE_INPUTS, "Tiszta bemenetek");
    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Aktív bemenetek");
    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_UNCONNECTED_IO, "Nem csatlakoztatott I/O");
    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_ACTIVE_IO, "Aktív I/O");
    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Aktív kimenetek");
    engine.registerEntry("hu_HU", TXT_KEY_GRAPH_PURE_OUTPUTS, "Tiszta kimenetek");

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

    // sr_RS (Serbian)
    engine.registerEntry("sr_RS", TXT_KEY_PW_CENTER_TITLE, "Pipewire Контролни Центар");

    engine.registerEntry("sr_RS", TXT_KEY_BUTTON_APPS, "Апликације");
    engine.registerEntry("sr_RS", TXT_KEY_BUTTON_NODES, "Чворови");
    engine.registerEntry("sr_RS", TXT_KEY_BUTTON_INPUTS, "Улази");
    engine.registerEntry("sr_RS", TXT_KEY_BUTTON_CONFIGURATION, "Конфигурација");
    engine.registerEntry("sr_RS", TXT_KEY_BUTTON_GRAPH, "График");

    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 порт";
        if (count >= 2 && count <= 4)
            return "{count} порта";
        return "{count} портова";
    });
    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_PURE_INPUTS, "Чисти Улази");
    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Активни Улази");
    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_UNCONNECTED_IO, "Неповезани I/O");
    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_ACTIVE_IO, "Активни I/O");
    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Активни Излази");
    engine.registerEntry("sr_RS", TXT_KEY_GRAPH_PURE_OUTPUTS, "Чисти Излази");

    // sr_RS@latin (Serbian Latin)
    engine.registerEntry("sr_RS@latin@latin", TXT_KEY_PW_CENTER_TITLE, "Pipewire Kontrolni Centar");

    engine.registerEntry("sr_RS@latin", TXT_KEY_BUTTON_APPS, "Aplikacije");
    engine.registerEntry("sr_RS@latin", TXT_KEY_BUTTON_NODES, "Čvorovi");
    engine.registerEntry("sr_RS@latin", TXT_KEY_BUTTON_INPUTS, "Ulazi");
    engine.registerEntry("sr_RS@latin", TXT_KEY_BUTTON_CONFIGURATION, "Konfiguracija");
    engine.registerEntry("sr_RS@latin", TXT_KEY_BUTTON_GRAPH, "Grafik");

    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 port";
        if (count >= 2 && count <= 4)
            return "{count} porta";
        return "{count} portova";
    });
    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_PURE_INPUTS, "Čisti Ulazi");
    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Aktivni Ulazi");
    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_UNCONNECTED_IO, "Nepovezani I/O");
    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_ACTIVE_IO, "Aktivni I/O");
    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Aktivni Izlazi");
    engine.registerEntry("sr_RS@latin", TXT_KEY_GRAPH_PURE_OUTPUTS, "Čisti Izlazi");

    // sl_SI (Slovenian)
    engine.registerEntry("sl_SI", TXT_KEY_PW_CENTER_TITLE, "Pipewire Nadzorna Plošča");
    engine.registerEntry("sl_SI", TXT_KEY_BUTTON_APPS, "Aplikacije");
    engine.registerEntry("sl_SI", TXT_KEY_BUTTON_NODES, "Vozlišča");
    engine.registerEntry("sl_SI", TXT_KEY_BUTTON_INPUTS, "Vhodi");
    engine.registerEntry("sl_SI", TXT_KEY_BUTTON_CONFIGURATION, "Konfiguracija");
    engine.registerEntry("sl_SI", TXT_KEY_BUTTON_GRAPH, "Graf");
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "{count} priključek";
        if (count == 2)
            return "{count} priključka";
        if (count == 3 || count == 4)
            return "{count} priključki";

        return "{count} priključkov";
    });
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_PURE_INPUTS, "Čisti vhodi");
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Aktivni vhodi");
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_UNCONNECTED_IO, "Nepovezani vhodi/izhodi");
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_ACTIVE_IO, "Aktivni vhodi/izhodi");
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Aktivni izhodi");
    engine.registerEntry("sl_SI", TXT_KEY_GRAPH_PURE_OUTPUTS, "Čisti izhodi");
    //

    // tr_TR (Turkish)
    engine.registerEntry("tr_TR", TXT_KEY_PW_CENTER_TITLE, "Pipewire Kontrol Merkezi");

    engine.registerEntry("tr_TR", TXT_KEY_BUTTON_APPS, "Uygulamalar");
    engine.registerEntry("tr_TR", TXT_KEY_BUTTON_NODES, "Düğümler");
    engine.registerEntry("tr_TR", TXT_KEY_BUTTON_INPUTS, "Girişler");
    engine.registerEntry("tr_TR", TXT_KEY_BUTTON_CONFIGURATION, "Konfigürasyon");
    engine.registerEntry("tr_TR", TXT_KEY_BUTTON_GRAPH, "Grafik");

    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_N_PORTS, "{count} port");
    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_PURE_INPUTS, "Saf Girişler");
    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Aktif Girişler");
    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_UNCONNECTED_IO, "Bağlanmamış I/O");
    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_ACTIVE_IO, "Aktif I/O");
    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Aktif Çıkışlar");
    engine.registerEntry("tr_TR", TXT_KEY_GRAPH_PURE_OUTPUTS, "Saf Çıkışlar");
    //

    // el_GR (Greek)
    engine.registerEntry("el_GR", TXT_KEY_PW_CENTER_TITLE, "Κέντρο Ελέγχου Pipewire");

    engine.registerEntry("el_GR", TXT_KEY_BUTTON_APPS, "Εφαρμογές");
    engine.registerEntry("el_GR", TXT_KEY_BUTTON_NODES, "Kόμβοι");
    engine.registerEntry("el_GR", TXT_KEY_BUTTON_INPUTS, "Είσοδοι");
    engine.registerEntry("el_GR", TXT_KEY_BUTTON_CONFIGURATION, "Διαμόρφωση");
    engine.registerEntry("el_GR", TXT_KEY_BUTTON_GRAPH, "Γράφημα");

    engine.registerEntry("el_GR", TXT_KEY_GRAPH_N_PORTS, [](const Hyprutils::I18n::translationVarMap& vars) {
        const auto count = std::stoi(vars.at("count"));
        if (count == 1)
            return "1 θύρα";
        return "{count} θύρες";
    });
    engine.registerEntry("el_GR", TXT_KEY_GRAPH_PURE_INPUTS, "Καθαρές Είσοδοι");
    engine.registerEntry("el_GR", TXT_KEY_GRAPH_ACTIVE_INPUTS, "Ενεργές Είσοδοι");
    engine.registerEntry("el_GR", TXT_KEY_GRAPH_UNCONNECTED_IO, "Μη Συνδεδεμένες Είσοδοι/Έξοδοι");
    engine.registerEntry("el_GR", TXT_KEY_GRAPH_ACTIVE_IO, "Ενεργές Είσοδοι/Έξοδοι");
    engine.registerEntry("el_GR", TXT_KEY_GRAPH_ACTIVE_OUTPUTS, "Ενεργές Έξοδοι");
    engine.registerEntry("el_GR", TXT_KEY_GRAPH_PURE_OUTPUTS, "Καθαρές Έξοδοι");
}

std::string I18n::localize(eTextKeys key, const Hyprutils::I18n::translationVarMap& vars) {
    return engine.localizeEntry(engineLocale, key, vars);
}
