#include "Graph.hpp"
#include "../UI.hpp"

#include <ranges>

using namespace Hyprutils::Math;

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

    m_lastInitialPos = Vector2D{(CANVAS_SIZE / 2.F) - 200.F, (CANVAS_SIZE / 2.F) - 200.F};
    m_scrollArea->setScroll(m_lastInitialPos - Vector2D{40.F, 40.F});
}

CGraphView::~CGraphView() = default;

void CGraphView::center() {
    m_scrollArea->setScroll(Vector2D{(CANVAS_SIZE / 2.F) - 200.F, (CANVAS_SIZE / 2.F) - 200.F} - Vector2D{40.F, 40.F});
}

void CGraphView::addNode(WP<IPwNode> node) {
    if (std::ranges::find_if(m_nodes, [node](const auto& e) { return e->m_node == node; }) != m_nodes.end())
        return;

    m_lastInitialPos += Vector2D{0, 100}; // FIXME: this sucks XD

    auto x    = m_nodes.emplace_back(makeShared<CGraphNode>(node, m_lastInitialPos));
    x->m_view = m_self;
    m_container->addChild(x->m_background);
}

void CGraphView::removeNode(WP<IPwNode> node) {
    std::erase_if(m_nodes, [node](const auto& e) { return !e || !e->m_node || e->m_node == node; });
}

SP<CGraphNode> CGraphView::nodeFromCoord(const Hyprutils::Math::Vector2D& pos) {
    for (const auto& n : m_nodes | std::ranges::views::reverse) {
        if (n->contains(pos))
            return n;
    }

    return nullptr;
}
