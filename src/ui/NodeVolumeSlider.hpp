#pragma once

#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/element/Slider.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Null.hpp>
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/Image.hpp>

#include "../helpers/Memory.hpp"

class CNodeVolumeSlider {
  public:
    CNodeVolumeSlider(uint32_t id, const std::string& name);
    ~CNodeVolumeSlider();

    void                               setVolume(float, bool force = false);
    void                               setMuted(bool);

    SP<Hyprtoolkit::CRectangleElement> m_background;

    uint32_t                           m_id    = 0;
    float                              m_vol   = 0.5F;
    bool                               m_muted = false;

  private:
    SP<Hyprtoolkit::CColumnLayoutElement> m_mainLayout;
    SP<Hyprtoolkit::CRowLayoutElement>    m_topLayout;
    SP<Hyprtoolkit::CNullElement>         m_topSpacer;
    SP<Hyprtoolkit::CRowLayoutElement>    m_topRightLayout;
    SP<Hyprtoolkit::CTextElement>         m_topName;
    SP<Hyprtoolkit::CTextElement>         m_topVol;
    SP<Hyprtoolkit::CSliderElement>       m_slider;
    SP<Hyprtoolkit::CButtonElement>       m_muteButton;
    SP<Hyprtoolkit::CImageElement>        m_buttonIcon;

    bool                                  m_settingVolume = false;

    float                                 logdVolume();
    float                                 unlogVolume(float x);
};
