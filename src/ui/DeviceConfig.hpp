#pragma once

#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/element/Slider.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Null.hpp>
#include <hyprtoolkit/element/Combobox.hpp>

#include "../helpers/Memory.hpp"

class CDeviceConfig {
  public:
    CDeviceConfig(uint32_t id, const std::string& name, const std::vector<std::string>& modes, size_t current);
    ~CDeviceConfig();

    void                               update(const std::vector<std::string>& modes, size_t current);

    SP<Hyprtoolkit::CRectangleElement> m_background;

    size_t                             m_id      = 0;
    size_t                             m_current = 0;

  private:
    SP<Hyprtoolkit::CColumnLayoutElement> m_mainLayout;
    SP<Hyprtoolkit::CTextElement>         m_topName;
    SP<Hyprtoolkit::CComboboxElement>     m_dropdown;

    bool                                  m_updating = false;
};
