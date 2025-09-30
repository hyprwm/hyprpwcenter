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
    ~CGraphConnection();

    SP<Hyprtoolkit::CLineElement> m_line;

    void                          update();

    WP<CGraphView>                m_view;
    WP<CPipewireLink>             m_link;

    WP<CGraphNode>                m_a, m_b;

  private:
    size_t m_portA = 0, m_portB = 0;
};
