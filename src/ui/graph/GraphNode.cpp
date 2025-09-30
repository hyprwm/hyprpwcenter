#include "GraphNode.hpp"
#include "Graph.hpp"
#include "../UI.hpp"
#include "../../pw/IPwNode.hpp"

using namespace Hyprutils::Math;

constexpr float BUBBLE_WIDTH = 200.F;
constexpr float ANCHOR_WIDTH = 12.F;

CGraphNode::CGraphNode(WP<IPwNode> node, const Hyprutils::Math::Vector2D& initialPos) : m_node(node), m_pos(initialPos) {
    m_background = Hyprtoolkit::CRectangleBuilder::begin()
                       ->color([] { return g_ui->m_backend->getPalette()->m_colors.background.brighten(0.3F); })
                       ->rounding(6)
                       ->borderThickness(1)
                       ->borderColor([] { return g_ui->m_backend->getPalette()->m_colors.alternateBase.brighten(0.3F); })
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO,
                           {1, 1},
                       })
                       ->commence();
    m_background->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);

    m_text = Hyprtoolkit::CTextBuilder::begin()->text(std::string{node->m_name})->commence();
    m_text->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_HCENTER);

    m_subtext =
        Hyprtoolkit::CTextBuilder::begin()->text(std::format("<i>{} channels out</i>", node->m_channelsOut.size()))->fontSize({Hyprtoolkit::CFontSize::HT_FONT_SMALL})->commence();
    m_subtext->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_HCENTER);

    m_layoutInside = Hyprtoolkit::CColumnLayoutBuilder::begin()
                         ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {BUBBLE_WIDTH, 1.F}})
                         ->gap(5)
                         ->commence();

    m_layoutInside->addChild(m_text);
    m_layoutInside->addChild(m_subtext);

    m_layoutInside->setMargin(10);

    m_background->addChild(m_layoutInside);
    m_background->setGrouped(true);

    update();

    setPos(m_pos);
}

CGraphNode::~CGraphNode() {
    if (!m_view)
        return;

    m_view->m_container->removeChild(m_background);
}

void CGraphNode::update() {
    if (!m_node)
        return;

    m_subtext->rebuild()->text(std::format("<i>{} channels out</i>", m_node->m_channelsOut.size()))->commence();

    m_anchors.clear();
    m_layoutInside->clearChildren();

    m_layoutInside->addChild(m_text);
    m_layoutInside->addChild(m_subtext);

    // add anchors later because we need to wait for layout
    for (size_t i = 0; i < m_node->m_channelsOut.size(); ++i) {
        auto anchor = m_anchors.emplace_back(makeShared<SAnchor>());

        anchor->anchorPad =
            Hyprtoolkit::CNullBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, ANCHOR_WIDTH}})->commence();

        anchor->anchor = Hyprtoolkit::CRectangleBuilder::begin()
                             ->color([] { return g_ui->m_backend->getPalette()->m_colors.accent; })
                             ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {ANCHOR_WIDTH, ANCHOR_WIDTH}})
                             ->rounding(ANCHOR_WIDTH / 2)
                             ->commence();

        anchor->rightText = Hyprtoolkit::CTextBuilder::begin()->text(IPwNode::getNameForChannel(m_node->m_channelsOut.at(i)))->commence();
        anchor->rightText->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_RIGHT);
        anchor->rightText->setAbsolutePosition({-5.F, 0.F});

        anchor->anchor->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_RIGHT);
        anchor->anchor->setAbsolutePosition({ANCHOR_WIDTH, 0.F});

        anchor->anchorPad->addChild(anchor->anchor);
        anchor->anchorPad->addChild(anchor->rightText);

        m_layoutInside->addChild(anchor->anchorPad);
    }
}

bool CGraphNode::contains(const Hyprutils::Math::Vector2D& p) {
    return CBox{m_pos, m_background->size()}.containsPoint(p);
}

Vector2D CGraphNode::pos() {
    return m_pos;
}

void CGraphNode::setPos(const Vector2D& x) {
    m_pos = x;
    m_background->setAbsolutePosition(m_pos);
}
