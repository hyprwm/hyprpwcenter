#pragma once

#include <hyprtoolkit/element/Rectangle.hpp>
#include <hyprtoolkit/element/Text.hpp>
#include <hyprtoolkit/element/Slider.hpp>
#include <hyprtoolkit/element/RowLayout.hpp>
#include <hyprtoolkit/element/ColumnLayout.hpp>
#include <hyprtoolkit/element/Null.hpp>
#include <hyprtoolkit/element/Button.hpp>
#include <hyprtoolkit/element/ScrollArea.hpp>

#include "../../helpers/Memory.hpp"

class IPwNode;
class CGraphView;

class CGraphNode {
  public:
    CGraphNode(WP<IPwNode> node, const Hyprutils::Math::Vector2D& initialPos);
    ~CGraphNode();

    SP<Hyprtoolkit::CRectangleElement> m_background;

    bool                               contains(const Hyprutils::Math::Vector2D&);
    Hyprutils::Math::Vector2D          pos();
    void                               setPos(const Hyprutils::Math::Vector2D&);

    void                               update();

    WP<IPwNode>                        m_node;
    WP<CGraphView>                     m_view;

  private:
    SP<Hyprtoolkit::CColumnLayoutElement> m_layoutInside;
    SP<Hyprtoolkit::CTextElement>         m_text;
    SP<Hyprtoolkit::CTextElement>         m_subtext;

    struct SAnchor {
        SP<Hyprtoolkit::CNullElement>      anchorPad;
        SP<Hyprtoolkit::CRectangleElement> anchor;
        SP<Hyprtoolkit::CTextElement>      rightText;
    };

    std::vector<SP<SAnchor>>  m_anchors;

    Hyprutils::Math::Vector2D m_pos;
};
