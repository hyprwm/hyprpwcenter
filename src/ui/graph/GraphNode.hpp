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
    enum eNodePolarity : uint8_t {
        NODE_OUTPUT,
        NODE_IO,
        NODE_INPUT,
    };

    CGraphNode(WP<IPwNode> node, const Hyprutils::Math::Vector2D& initialPos);
    ~CGraphNode();

    SP<Hyprtoolkit::CRectangleElement> m_background;

    bool                               contains(const Hyprutils::Math::Vector2D&);
    Hyprutils::Math::Vector2D          pos();
    Hyprutils::Math::Vector2D          size();
    void                               setPos(const Hyprutils::Math::Vector2D&);

    void                               update();
    eNodePolarity                      nodePolarity();

    WP<IPwNode>                        m_node;
    WP<CGraphView>                     m_view;

  private:
    SP<Hyprtoolkit::CColumnLayoutElement> m_layoutInside;
    SP<Hyprtoolkit::CTextElement>         m_text;
    SP<Hyprtoolkit::CTextElement>         m_subtext;

    struct SAnchor {
        SP<Hyprtoolkit::CNullElement>      anchorPad;
        SP<Hyprtoolkit::CRectangleElement> rightAnchor;
        SP<Hyprtoolkit::CRectangleElement> leftAnchor;
        SP<Hyprtoolkit::CTextElement>      rightText;
        SP<Hyprtoolkit::CTextElement>      leftText;
    };

    std::vector<SP<SAnchor>>  m_anchors;

    Hyprutils::Math::Vector2D m_pos;
};
