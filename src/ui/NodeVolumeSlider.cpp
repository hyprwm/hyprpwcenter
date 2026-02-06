#include "NodeVolumeSlider.hpp"
#include "UI.hpp"
#include "../pw/PwState.hpp"
#include <cmath>

constexpr float NODE_BOTTOM_HEIGHT = 8;
constexpr float INNER_MARGIN       = 4;
constexpr float BUTTON_HEIGHT      = 26;
constexpr float RIGHT_GAP          = 5;
constexpr float SPACER_WIDTH       = 1;

static float precalcVolTextWidth() {
    auto text   = Hyprtoolkit::CTextBuilder::begin()->text(std::string{"100%"})->async(false)->commence();
    auto layout = Hyprtoolkit::CColumnLayoutBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1.F, 1.F}})->commence();
    auto null   = Hyprtoolkit::CNullBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {99999.F, 99999.F}})->commence();
    layout->addChild(text);
    null->addChild(layout);
    layout->forceReposition();
    return layout->size().x;
}

CNodeVolumeSlider::CNodeVolumeSlider(uint32_t id, const std::string& name) : m_id(id) {
    static const float VOL_TEXT_WIDTH = precalcVolTextWidth();

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

    m_container = Hyprtoolkit::CNullBuilder::begin()
                      ->size({
                          Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                          Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                          {1.F, 1.F},
                      })
                      ->commence();
    m_container->setMargin(INNER_MARGIN);

    m_mainLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                       ->gap(5)
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                           {1.F, 1.F},
                       })
                       ->commence();

    m_topLayout =
        Hyprtoolkit::CRowLayoutBuilder::begin()->gap(10)->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1.F, 1.F}})->commence();
    m_topLayout->setGrow(true);

    m_slider = Hyprtoolkit::CSliderBuilder::begin()
                   ->min(0.F)
                   ->max(1.F)
                   ->val(logdVolume())
                   ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, NODE_BOTTOM_HEIGHT}})
                   ->onChanged([this](SP<Hyprtoolkit::CSliderElement> s, float val) {
                       if (m_settingVolume)
                           return;

                       setVolume(unlogVolume(val), true);
                       g_pipewire->setVolume(m_id, unlogVolume(val));
                   })
                   ->commence();

    m_topName = Hyprtoolkit::CTextBuilder::begin()->text(std::string{name})->commence();

    m_topRightContainer =
        Hyprtoolkit::CNullBuilder::begin()
            ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {VOL_TEXT_WIDTH + BUTTON_HEIGHT + (RIGHT_GAP * 2) + SPACER_WIDTH, BUTTON_HEIGHT}})
            ->commence();
    m_topRightLayout =
        Hyprtoolkit::CRowLayoutBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1, 1}})->gap(RIGHT_GAP)->commence();
    m_topRightSpacer = Hyprtoolkit::CNullBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {SPACER_WIDTH, SPACER_WIDTH}})->commence();
    m_topRightSpacer->setGrow(true);

    m_topVol    = Hyprtoolkit::CTextBuilder::begin()->text(std::format("{}%", sc<int>(logdVolume() * 100.F)))->commence();
    m_topSpacer = Hyprtoolkit::CNullBuilder::begin()->commence();
    m_topSpacer->setGrow(true);

    m_muteButtonContainer = Hyprtoolkit::CNullBuilder::begin()
                                ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {BUTTON_HEIGHT, BUTTON_HEIGHT}})
                                ->commence();
    m_muteButtonContainer->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    m_muteButtonContainer->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_VCENTER, true);

    m_muteButton = Hyprtoolkit::CButtonBuilder::begin()
                       ->label("")
                       ->fontSize({Hyprtoolkit::CFontSize::HT_FONT_H3})
                       ->noBorder(true)
                       ->onMainClick([this](SP<Hyprtoolkit::CButtonElement>) {
                           setMuted(!m_muted);
                           g_pipewire->setMuted(m_id, m_muted);
                       })
                       ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1, 1}})
                       ->commence();

    m_muteButton->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    m_muteButton->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_CENTER, true);

    m_buttonIcon = Hyprtoolkit::CImageBuilder::begin()
                       ->icon(g_ui->m_volumeUpIconHandle)
                       ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {0.65, 0.65}})
                       ->commence();

    m_buttonIcon->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    m_buttonIcon->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_CENTER, true);

    m_muteButtonContainer->addChild(m_muteButton);
    m_muteButtonContainer->addChild(m_buttonIcon);

    m_topLayout->addChild(m_topName);
    m_topLayout->addChild(m_topSpacer);
    m_topLayout->addChild(m_topRightContainer);

    m_topRightContainer->addChild(m_topRightLayout);
    m_topRightLayout->addChild(m_topRightSpacer);
    m_topRightLayout->addChild(m_muteButtonContainer);
    m_topRightLayout->addChild(m_topVol);

    m_mainLayout->addChild(m_topLayout);
    m_mainLayout->addChild(m_slider);

    m_container->addChild(m_mainLayout);

    m_background->addChild(m_container);
}

CNodeVolumeSlider::~CNodeVolumeSlider() = default;

void CNodeVolumeSlider::setVolume(float v, bool force) {
    if (!force && m_slider->sliding())
        return;

    m_vol = v;

    m_settingVolume = true;

    m_topVol->rebuild()->text(std::format("{}%", sc<int>(logdVolume() * 100.F)))->commence();
    m_slider->rebuild()->val(logdVolume())->commence();

    m_settingVolume = false;
}

void CNodeVolumeSlider::setMuted(bool x) {
    m_muted = x;
    m_buttonIcon->rebuild()->icon(m_muted ? g_ui->m_mutedIconHandle : g_ui->m_volumeUpIconHandle)->commence();
}

// K constant for logarithmic volume. Can be a matter of taste, but 8 seems alright.
constexpr float K = 8;

//
float CNodeVolumeSlider::logdVolume() {
    return std::log(1 + (K * m_vol)) / std::log(1 + K);
}

float CNodeVolumeSlider::unlogVolume(float x) {
    return (std::pow(1 + K, x) - 1) / K;
}
