#include "pw/PwState.hpp"
#include "ui/UI.hpp"
#include "helpers/Log.hpp"
#include "i18n/Engine.hpp"

int main(int argc, char** argv, char** envp) {
    I18n::initEngine();

    g_pipewire = makeUnique<CPipewireState>(argc, argv);
    g_ui       = makeUnique<CUI>();

    for (int i = 1; i < argc; ++i) {
        std::string_view sv{argv[i]};

        if (sv == "--verbose") {
            Debug::verbose = true;
            continue;
        } else if (sv == "--quiet") {
            Debug::quiet = true;
            continue;
        }
    }

    g_ui->run();

    return 0;
};