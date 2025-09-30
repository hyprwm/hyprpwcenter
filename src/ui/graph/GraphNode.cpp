#include "GraphNode.hpp"
#include "Graph.hpp"
#include "../UI.hpp"
#include "../../pw/IPwNode.hpp"
#include "../../helpers/Log.hpp"

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

    m_subtext = Hyprtoolkit::CTextBuilder::begin()->text(std::format("<i>{} ports</i>", node->m_ports.size()))->fontSize({Hyprtoolkit::CFontSize::HT_FONT_SMALL})->commence();
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

    m_text->rebuild()->text(std::string{m_node->m_name})->commence();
    m_subtext->rebuild()->text(std::format("<i>{} ports</i>", m_node->m_ports.size()))->commence();

    m_anchors.clear();
    m_layoutInside->clearChildren();

    m_layoutInside->addChild(m_text);
    m_layoutInside->addChild(m_subtext);

    if (m_node->m_ports.empty())
        return;

    std::vector<std::pair<spa_audio_channel, std::string>> inputVec, outputVec;

    for (size_t i = 0; i < m_node->m_ports.size(); ++i) {
        const auto& p = m_node->m_ports.at(i);

        if (p->m_output)
            outputVec.append_range(p->m_channels);
        else
            inputVec.append_range(p->m_channels);
    }

    size_t maxRows = std::max(inputVec.size(), outputVec.size());

    Debug::log(TRACE, "ui: graphnode: max rows {} for {} ports", maxRows, m_node->m_ports.size());

    static auto CHAN_NAME = [](const std::pair<spa_audio_channel, std::string>& p) -> std::string {
        if (p.second.empty())
            return IPwNode::getNameForChannel(p.first);
        return p.second;
    };

    for (size_t i = 0; i < maxRows; ++i) {
        auto       anchor = m_anchors.emplace_back(makeShared<SAnchor>());

        const bool HAS_RIGHT = outputVec.size() > i;
        const bool HAS_LEFT  = inputVec.size() > i;

        anchor->anchorPad =
            Hyprtoolkit::CNullBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, ANCHOR_WIDTH}})->commence();

        if (HAS_RIGHT) {
            anchor->rightAnchor = Hyprtoolkit::CRectangleBuilder::begin()
                                      ->color([] { return g_ui->m_backend->getPalette()->m_colors.accent; })
                                      ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {ANCHOR_WIDTH, ANCHOR_WIDTH}})
                                      ->rounding(ANCHOR_WIDTH / 2)
                                      ->commence();

            anchor->rightText = Hyprtoolkit::CTextBuilder::begin()->text(CHAN_NAME(outputVec.at(i)))->commence();
            anchor->rightText->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_RIGHT);
            anchor->rightText->setAbsolutePosition({-5.F, 0.F});

            anchor->rightAnchor->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_RIGHT);
            anchor->rightAnchor->setAbsolutePosition({ANCHOR_WIDTH, 0.F});

            anchor->anchorPad->addChild(anchor->rightAnchor);
            anchor->anchorPad->addChild(anchor->rightText);
        }

        if (HAS_LEFT) {
            anchor->leftAnchor = Hyprtoolkit::CRectangleBuilder::begin()
                                     ->color([] { return g_ui->m_backend->getPalette()->m_colors.accent; })
                                     ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {ANCHOR_WIDTH, ANCHOR_WIDTH}})
                                     ->rounding(ANCHOR_WIDTH / 2)
                                     ->commence();

            anchor->leftText = Hyprtoolkit::CTextBuilder::begin()->text(CHAN_NAME(inputVec.at(i)))->commence();
            anchor->leftText->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_LEFT);
            anchor->leftText->setAbsolutePosition({5.F, 0.F});

            anchor->leftAnchor->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_LEFT);
            anchor->leftAnchor->setAbsolutePosition({-ANCHOR_WIDTH, 0.F});

            anchor->anchorPad->addChild(anchor->leftAnchor);
            anchor->anchorPad->addChild(anchor->leftText);
        }

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

CGraphNode::eNodePolarity CGraphNode::nodePolarity() {
    bool hasIn = false, hasOut = false;

    for (size_t i = 0; i < m_node->m_ports.size(); ++i) {
        const auto& p = m_node->m_ports.at(i);

        if (p->m_output)
            hasOut = true;
        else
            hasIn = true;

        if (hasIn && hasOut)
            return NODE_IO;
    }

    if (hasIn)
        return NODE_INPUT;

    return NODE_OUTPUT;
}

Vector2D CGraphNode::size() {
    return m_background->size();
}
