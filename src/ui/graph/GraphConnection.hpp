#pragma once

#include <hyprtoolkit/element/Line.hpp>

#include "../../helpers/Memory.hpp"

class IPwNode;
class CGraphView;
class CGraphNode;
class CPipewireLink;

class CGraphConnection {
  public:
    CGraphConnection(WP<CGraphNode> a, size_t portA, WP<CGraphNode> b, size_t portB, WP<CPipewireLink> link);
    CGraphConnection(WP<CGraphNode> a, size_t portA, const Hyprutils::Math::Vector2D& pos);
    ~CGraphConnection();

    SP<Hyprtoolkit::CLineElement> m_line;

    void                          update();
    void                          updateDest(const Hyprutils::Math::Vector2D& pos);

    WP<CGraphView>                m_view;
    WP<CPipewireLink>             m_link;

    WP<CGraphNode>                m_a, m_b;

  private:
    size_t                    m_portA = 0, m_portB = 0;
    bool                      m_isDest = false;
    Hyprutils::Math::Vector2D m_dest;
};
