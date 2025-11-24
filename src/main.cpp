#include "pw/PwState.hpp"
#include "ui/UI.hpp"
#include "helpers/Logger.hpp"
#include "i18n/Engine.hpp"

#include <hyprutils/cli/ArgumentParser.hpp>
#include <hyprutils/memory/Casts.hpp>

#include <print>

using namespace Hyprutils::CLI;
using namespace Hyprutils::Memory;
using namespace std::string_literals;

#define ASSERT(expr)                                                                                                                                                               \
    if (!(expr)) {                                                                                                                                                                 \
        g_logger->log(LOG_CRIT, "Failed assertion at line {} in {}: {} was false", __LINE__,                                                                                       \
                      ([]() constexpr -> std::string { return std::string(__FILE__).substr(std::string(__FILE__).find("/src/") + 1); })(), #expr);                                 \
        std::abort();                                                                                                                                                              \
    }

int main(int argc, char** argv, char** envp) {

    CArgumentParser parser(std::span<const char*>(cc<const char**>(argv), argc));
    ASSERT(parser.registerBoolOption("verbose", "", "Turn on verbose logging"));
    ASSERT(parser.registerBoolOption("quiet", "", "Turn off logging"));

    ASSERT(parser.registerBoolOption("help", "h", "Show help menu"));

    if (auto result = parser.parse(); !result) {
        g_logger->log(LOG_CRIT, "Failed to parse arguments: {}", result.error());
        return 1;
    }

    if (parser.getBool("help").value_or(false)) {
        std::println("{}", parser.getDescription("hyprpwcenter v"s + HYPRPWCENTER_VERSION));
        return 0;
    }

    if (parser.getBool("verbose").value_or(false))
        g_logger->setLogLevel(LOG_TRACE);
    if (parser.getBool("quiet"))
        g_logger->setEnableStdout(false);

    I18n::initEngine();

    g_pipewire = makeUnique<CPipewireState>(argc, argv);
    g_ui       = makeUnique<CUI>();

    g_ui->run();

    return 0;
};