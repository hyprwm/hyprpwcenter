#include "GraphNode.hpp"
#include "GraphConnection.hpp"
#include "Graph.hpp"
#include "../UI.hpp"
#include "../../pw/IPwNode.hpp"
#include "../../helpers/Log.hpp"
#include "../../i18n/Engine.hpp"

using namespace Hyprutils::Math;

constexpr float ANCHOR_WIDTH    = 12.F;
constexpr float ANCHOR_WIDTH_SQ = ANCHOR_WIDTH * ANCHOR_WIDTH;

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
    m_text->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    m_text->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);

    m_subtext = Hyprtoolkit::CTextBuilder::begin()->text(std::format("<i>{} ports</i>", node->m_ports.size()))->fontSize({Hyprtoolkit::CFontSize::HT_FONT_SMALL})->commence();
    m_subtext->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
    m_subtext->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_HCENTER, true);

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
    m_subtext->rebuild()->text(std::format("<i>{}</i>", I18n::localize(I18n::TXT_KEY_GRAPH_N_PORTS, {{"count", std::to_string(m_node->m_ports.size())}})))->commence();

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

    size_t      maxRows = std::max(inputVec.size(), outputVec.size());

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
            anchor->rightText->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
            anchor->rightText->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_RIGHT, true);
            anchor->rightText->setAbsolutePosition({-5.F, 0.F});

            anchor->rightAnchor->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
            anchor->rightAnchor->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_RIGHT, true);
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
            anchor->leftText->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
            anchor->leftText->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_LEFT, true);
            anchor->leftText->setAbsolutePosition({5.F, 0.F});

            anchor->leftAnchor->setPositionMode(Hyprtoolkit::IElement::HT_POSITION_ABSOLUTE);
            anchor->leftAnchor->setPositionFlag(Hyprtoolkit::IElement::HT_POSITION_FLAG_LEFT, true);
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

        if (hasIn && hasOut) {

            bool hasConnectionIn = false, hasConnectionOut = false;

            for (const auto& c : m_view->m_connections) {
                if (c->m_a && c->m_a->m_node == m_node)
                    hasConnectionIn = true;
                else if (c->m_b && c->m_b->m_node == m_node)
                    hasConnectionOut = true;

                if (hasConnectionIn && hasConnectionOut)
                    return NODE_IO;
            }

            if (hasConnectionIn)
                return NODE_INPUT;
            if (hasConnectionOut)
                return NODE_OUTPUT;
            return NODE_UNCONNECTED_IO;
        }
    }

    if (hasIn)
        return NODE_PURE_OUTPUT;

    return NODE_PURE_INPUT;
}

Vector2D CGraphNode::size() {
    return m_background->size();
}

Vector2D CGraphNode::getInputPos(size_t idx) {
    if (idx >= m_anchors.size())
        return {};

    const auto& anchor = m_anchors.at(idx);

    if (!anchor->leftAnchor)
        return {};

    return m_pos + m_layoutInside->posFromParent() + anchor->leftAnchor->posFromParent() + anchor->anchorPad->posFromParent() + anchor->leftAnchor->size() / 2.F;
}

Vector2D CGraphNode::getOutputPos(size_t idx) {
    if (idx >= m_anchors.size())
        return {};

    const auto& anchor = m_anchors.at(idx);

    if (!anchor->rightAnchor)
        return {};

    return m_pos + m_layoutInside->posFromParent() + anchor->rightAnchor->posFromParent() + anchor->anchorPad->posFromParent() + anchor->rightAnchor->size() / 2.F;
}

size_t CGraphNode::portFromID(size_t id) {
    size_t inSize = 0, outSize = 0;

    for (size_t i = 0; i < m_node->m_ports.size(); ++i) {
        const auto& p = m_node->m_ports.at(i);

        if (p->m_id == id)
            return p->m_output ? outSize : inSize;

        if (p->m_output)
            outSize += p->m_channels.size();
        else
            inSize += p->m_channels.size();
    }

    return 0;
}

std::optional<size_t> CGraphNode::inputFromPos(const Hyprutils::Math::Vector2D& x) {
    for (size_t i = 0; i < m_anchors.size(); ++i) {
        if (getInputPos(i).distanceSq(x) < ANCHOR_WIDTH_SQ)
            return i;
    }

    return std::nullopt;
}

std::optional<size_t> CGraphNode::outputFromPos(const Hyprutils::Math::Vector2D& x) {
    for (size_t i = 0; i < m_anchors.size(); ++i) {
        if (getOutputPos(i).distanceSq(x) < ANCHOR_WIDTH_SQ)
            return i;
    }

    return std::nullopt;
}

uint32_t CGraphNode::inPortToID(size_t idx) {
    size_t inSize = 0;

    for (size_t i = 0; i < m_node->m_ports.size(); ++i) {
        const auto& p = m_node->m_ports.at(i);

        if (inSize >= idx)
            return p->m_id;

        if (!p->m_output)
            inSize += p->m_channels.size();
    }

    return 0;
}

uint32_t CGraphNode::outPortToID(size_t idx) {
    size_t outSize = 0;

    for (size_t i = 0; i < m_node->m_ports.size(); ++i) {
        const auto& p = m_node->m_ports.at(i);

        if (outSize >= idx)
            return p->m_id;

        if (p->m_output)
            outSize += p->m_channels.size();
    }

    return 0;
}
