#include "GraphConnection.hpp"
#include "GraphNode.hpp"
#include "Graph.hpp"
#include "../UI.hpp"

#include <hyprutils/animation/BezierCurve.hpp>

using namespace Hyprutils::Math;
using namespace Hyprutils::Animation;

constexpr const float  LINE_THICK     = 2;
constexpr const float  BOX_PAD        = 0.05;
constexpr const size_t CURVE_PTS      = 50;
constexpr const size_t BOX_HANDLE_LEN = 70;

CGraphConnection::CGraphConnection(WP<CGraphNode> a, size_t portA, WP<CGraphNode> b, size_t portB, WP<CPipewireLink> link) :
    m_link(link), m_a(a), m_b(b), m_portA(portA), m_portB(portB) {

    m_line = Hyprtoolkit::CLineBuilder::begin()
                 ->thick(LINE_THICK)
                 ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {}})
                 ->color([] { return g_ui->m_backend->getPalette()->m_colors.accent; })
                 ->commence();

    update();
}

CGraphConnection::~CGraphConnection() {
    if (!m_view)
        return;

    m_view->m_container->removeChild(m_line);
}

void CGraphConnection::update() {
    if (!m_a || !m_b)
        return;

    auto posA = m_a->getOutputPos(m_portA);
    auto posB = m_b->getInputPos(m_portB);

    if (posA == Vector2D{} || posB == Vector2D{}) {
        m_line->rebuild()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {}})->points({})->commence();
        return;
    }

    CBox box;

    box.x = std::min(posA.x, posB.x - BOX_HANDLE_LEN);
    box.y = std::min(posA.y, posB.y);
    box.w = std::abs(std::max((posA.x + BOX_HANDLE_LEN), posB.x) - std::min((posB.x - BOX_HANDLE_LEN), posA.x));
    box.h = std::abs(posA.y - posB.y);

    box.scaleFromCenter(1.F + (2 * BOX_PAD));

    // now we have the box, let's make the curve
    CBezierCurve bc;
    bc.setup4({
        (posA - box.pos()) / box.size(),
        (posA + Vector2D{BOX_HANDLE_LEN, 0} - box.pos()) / box.size(),
        (posB - Vector2D{BOX_HANDLE_LEN, 0} - box.pos()) / box.size(),
        (posB - box.pos()) / box.size(),
    });

    std::vector<Vector2D> points;
    points.resize(CURVE_PTS);
    for (size_t i = 0; i < CURVE_PTS; ++i) {
        points.at(i) = Vector2D{
            bc.getXForT(sc<float>(i) / sc<float>(CURVE_PTS)),
            bc.getYForT(sc<float>(i) / sc<float>(CURVE_PTS)),
        };
    }

    m_line->rebuild()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, box.size()})->points(std::move(points))->commence();
    m_line->setAbsolutePosition(box.pos());
}
