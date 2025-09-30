#pragma once

#include "GraphNode.hpp"

#include "../../helpers/Memory.hpp"

class CGraphNode;
class CGraphConnection;
class CPipewireLink;

class CGraphView {
  public:
    CGraphView();
    ~CGraphView();

    void                               addNode(WP<IPwNode> node);
    void                               removeNode(WP<IPwNode> node);
    void                               addLink(WP<CPipewireLink> link);
    void                               removeLink(WP<CPipewireLink> link);

    void                               rearrange();

    SP<Hyprtoolkit::CRectangleElement> m_background;

    WP<CGraphView>                     m_self;

  private:
    SP<CGraphNode>                      nodeFromCoord(const Hyprutils::Math::Vector2D& pos);
    SP<CGraphNode>                      nodeFromID(size_t x);

    void                                connect(SP<CGraphNode> a, SP<CGraphNode> b, size_t portA, size_t portB, WP<CPipewireLink> link);
    void                                updateAllConnections(SP<CGraphNode> withNode = nullptr);

    void                                endDrag();

    SP<Hyprtoolkit::CNullElement>       m_container;
    SP<Hyprtoolkit::CScrollAreaElement> m_scrollArea;

    std::vector<SP<CGraphNode>>         m_nodes;
    std::vector<SP<CGraphConnection>>   m_connections;
    SP<CGraphConnection>                m_liveConnection;

    Hyprutils::Math::Vector2D           m_posAtStart, m_lastMousePos, m_elementPosAtStart;
    SP<CGraphNode>                      m_draggingNode;
    size_t                              m_startedPort      = 0;
    bool                                m_startedPortInput = false;

    bool                                m_mouseDown = false;

    Hyprutils::Math::Vector2D           m_initialPos = {};
    float                               m_inOffset = 0, m_outOffset = 0, m_ioOffset = 0;

    friend class CGraphNode;
    friend class CGraphConnection;
};