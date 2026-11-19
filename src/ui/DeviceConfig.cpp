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
                           Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE,
                           {1.F, NODE_BOX_HEIGHT},
                       })
                       ->commence();

    m_container = Hyprtoolkit::CNullBuilder::begin()
                      ->size({
                          Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                          Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                          {1.F, 1.F},
                      })
                      ->commence();
    m_container->setMargin(INNER_MARGIN);

    m_mainLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                       ->gap(10)
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           {1.F, 1.F},
                       })
                       ->commence();

    m_topLayout =
        Hyprtoolkit::CRowLayoutBuilder::begin()->gap(10)->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, 1.F}})->commence();
    m_topLayout->setGrow(true);

    m_dropdown = Hyprtoolkit::CComboboxBuilder::begin()
                     ->items(std::vector<std::string>{modes})
                     ->currentItem(current)
                     ->onChanged([this](SP<Hyprtoolkit::CComboboxElement>, size_t idx) {
                         if (m_updating)
                             return;

                         g_pipewire->setMode(m_id, idx);
                     })
                     ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, DEVICE_BOTTOM_HEIGHT}})
                     ->commence();

    m_topName   = Hyprtoolkit::CTextBuilder::begin()->text(std::string{name})->commence();
    m_topSpacer = Hyprtoolkit::CNullBuilder::begin()->commence();
    m_topSpacer->setGrow(true);

    m_topLayout->addChild(m_topName);
    m_topLayout->addChild(m_topSpacer);

    m_mainLayout->addChild(m_topLayout);
    m_mainLayout->addChild(m_dropdown);

    m_container->addChild(m_mainLayout);

    m_background->addChild(m_container);
}

CDeviceConfig::~CDeviceConfig() = default;

void CDeviceConfig::update(const std::vector<std::string>& modes, size_t current) {
    m_updating = true;
    m_dropdown->rebuild()->items(std::vector<std::string>{modes})->currentItem(current)->commence();
    m_updating = false;
}
