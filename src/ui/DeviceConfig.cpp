#include "DeviceConfig.hpp"
#include "UI.hpp"
#include "../pw/PwState.hpp"
#include <cmath>

constexpr float NODE_BOX_HEIGHT      = 56;
constexpr float DEVICE_BOTTOM_HEIGHT = 26;
constexpr float INNER_MARGIN         = 4;

CDeviceConfig::CDeviceConfig(uint32_t id, const std::string& name, const std::vector<std::string>& modes, size_t current) : m_id(id) {
    m_background = Hyprtoolkit::CRectangleBuilder::begin()
                       ->color([] { return g_ui->m_backend->getPalette()->m_colors.background.brighten(0.05F); })
                       ->rounding(6)
                       ->borderThickness(1)
                       ->borderColor([] { return g_ui->m_backend->getPalette()->m_colors.alternateBase; })
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                           {1.F, 1.F},
                       })
                       ->commence();
    m_mainLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                       ->gap(10)
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                           {1.F, 1.F},
                       })
                       ->commence();
    m_mainLayout->setMargin(INNER_MARGIN);

    m_dropdown = Hyprtoolkit::CComboboxBuilder::begin()
                     ->items(std::vector<std::string>{modes})
                     ->currentItem(current)
                     ->onChanged([this](SP<Hyprtoolkit::CComboboxElement>, size_t idx) {
                         if (m_updating)
                             return;

                         g_pipewire->setMode(m_id, idx);
                     })
                     ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1.F, 1.F}})
                     ->commence();

    m_topName   = Hyprtoolkit::CTextBuilder::begin()
        ->text(std::string{name})
        ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1.F, 1.F}})
        ->commence();

    m_mainLayout->addChild(m_topName);
    m_mainLayout->addChild(m_dropdown);

    m_background->addChild(m_mainLayout);
}

CDeviceConfig::~CDeviceConfig() = default;

void CDeviceConfig::update(const std::vector<std::string>& modes, size_t current) {
    m_updating = true;
    m_dropdown->rebuild()->items(std::vector<std::string>{modes})->currentItem(current)->commence();
    m_updating = false;
}
