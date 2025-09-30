#include "Graph.hpp"
#include "GraphConnection.hpp"
#include "../UI.hpp"
#include "../../pw/IPwNode.hpp"
#include "../../pw/PwLink.hpp"

#include <ranges>

#include <hyprutils/animation/BezierCurve.hpp>

using namespace Hyprutils::Math;
using namespace Hyprutils::Animation;

constexpr float CANVAS_SIZE = 10000.F;

CGraphView::CGraphView() {
    m_background = Hyprtoolkit::CRectangleBuilder::begin()
                       ->color([] { return g_ui->m_backend->getPalette()->m_colors.background.brighten(0.1F); })
                       ->rounding(6)
                       ->borderThickness(1)
                       ->borderColor([] { return g_ui->m_backend->getPalette()->m_colors.alternateBase; })
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           {1.F, 1.F},
                       })
                       ->commence();

    m_scrollArea = Hyprtoolkit::CScrollAreaBuilder::begin()
                       ->scrollX(true)
                       ->scrollY(true)
                       ->size({
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT,
                           {1.F, 1.F},
                       })
                       ->commence();

    m_container = Hyprtoolkit::CNullBuilder::begin()
                      ->size({
                          Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE,
                          Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE,
                          {CANVAS_SIZE, CANVAS_SIZE},
                      })
                      ->commence();

    m_container->setReceivesMouse(true);
    m_container->setMouseMove([this](const Vector2D& local) {
        m_lastMousePos = local;

        if (m_mouseDown) {

            const auto DELTA = m_lastMousePos - m_posAtStart;

            if (m_draggingNode) {
                const auto newPos = DELTA + m_elementPosAtStart;
                m_draggingNode->setPos(newPos);
                updateAllConnections(m_draggingNode);
            } else {
                const auto newPos = -DELTA + m_elementPosAtStart;
                m_scrollArea->setScroll(newPos);
            }
        }
    });
    m_container->setMouseEnter([this](const Vector2D& local) { m_mouseDown = false; });
    m_container->setMouseLeave([this]() { m_mouseDown = false; });
    m_container->setMouseButton([this](Hyprtoolkit::Input::eMouseButton button, bool down) {
        if (down) {
            m_posAtStart = m_lastMousePos;

            m_mouseDown = true;

            m_draggingNode = nodeFromCoord(m_lastMousePos);
            if (m_draggingNode)
                m_elementPosAtStart = m_draggingNode->pos();
            else
                m_elementPosAtStart = m_scrollArea->getCurrentScroll();
        } else {
            m_mouseDown = false;
            m_draggingNode.reset();
        }
    });

    m_background->addChild(m_scrollArea);
    m_scrollArea->addChild(m_container);

    rearrange();
}

CGraphView::~CGraphView() = default;

constexpr const float COLUMN_GAP  = 400.F;
constexpr const float ELEMENT_GAP = 40.F;

//
void CGraphView::rearrange() {
    m_initialPos = Vector2D{(CANVAS_SIZE / 2.F) - 200.F, (CANVAS_SIZE / 2.F) - 200.F};
    m_scrollArea->setScroll(m_initialPos - Vector2D{40.F, 40.F});

    m_inOffset  = 20;
    m_outOffset = 20;
    m_ioOffset  = 20;

    for (const auto& n : m_nodes) {
        float size = 0;
        switch (n->nodePolarity()) {
            case CGraphNode::NODE_OUTPUT:
                n->setPos(m_initialPos + Vector2D{0.F, m_inOffset});
                size = n->size().y;
                if (size == 0) // FIXME: this requires something in the toolkit to fix. We have to asynchronously wait for the text...
                    size = 150;
                m_inOffset += size + ELEMENT_GAP;
                break;

            case CGraphNode::NODE_IO:
                n->setPos(m_initialPos + Vector2D{COLUMN_GAP, m_ioOffset});
                size = n->size().y;
                if (size == 0)
                    size = 150;
                m_ioOffset += size + ELEMENT_GAP;
                break;

            case CGraphNode::NODE_INPUT:
                n->setPos(m_initialPos + Vector2D{COLUMN_GAP * 2, m_inOffset});
                size = n->size().y;
                if (size == 0)
                    size = 150;
                m_inOffset += size + ELEMENT_GAP;
                break;
        }
    }

    updateAllConnections();
}

void CGraphView::addNode(WP<IPwNode> node) {
    auto it = std::ranges::find_if(m_nodes, [node](const auto& e) { return e->m_node == node; });
    if (it != m_nodes.end()) {
        (*it)->update();
        return;
    }

    auto x    = m_nodes.emplace_back(makeShared<CGraphNode>(node, m_initialPos));
    x->m_view = m_self;
    m_container->addChild(x->m_background);

    updateAllConnections();
}

void CGraphView::removeNode(WP<IPwNode> node) {
    std::erase_if(m_nodes, [node](const auto& e) { return !e || !e->m_node || e->m_node == node; });

    updateAllConnections();
}

void CGraphView::addLink(WP<CPipewireLink> link) {
    auto nodeA = nodeFromID(link->m_nodeAID);
    auto nodeB = nodeFromID(link->m_nodeBID);

    auto portA = nodeA->portFromID(link->m_portAID);
    auto portB = nodeB->portFromID(link->m_portBID);

    connect(nodeA, nodeB, portA, portB, link);
}

void CGraphView::removeLink(WP<CPipewireLink> link) {
    std::erase_if(m_connections, [link](const auto& c) { return !c || !c->m_link || c->m_link == link; });
}

SP<CGraphNode> CGraphView::nodeFromCoord(const Hyprutils::Math::Vector2D& pos) {
    for (const auto& n : m_nodes | std::ranges::views::reverse) {
        if (n->contains(pos))
            return n;
    }

    return nullptr;
}

SP<CGraphNode> CGraphView::nodeFromID(size_t x) {
    for (const auto& n : m_nodes | std::ranges::views::reverse) {
        if (n->m_node && n->m_node->m_id == x)
            return n;
    }

    return nullptr;
}

void CGraphView::updateAllConnections(SP<CGraphNode> withNode) {
    for (const auto& c : m_connections) {
        if (withNode && c->m_a != withNode && c->m_b != withNode)
            continue;
        c->update();
    }
}

void CGraphView::connect(SP<CGraphNode> a, SP<CGraphNode> b, size_t portA, size_t portB, WP<CPipewireLink> link) {
    auto x    = m_connections.emplace_back(makeShared<CGraphConnection>(a, portA, b, portB, link));
    x->m_view = m_self;
    m_container->addChild(x->m_line);
}
