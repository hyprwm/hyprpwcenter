#pragma once

#include "GraphNode.hpp"

#include "../../helpers/Memory.hpp"

class CGraphNode;

class CGraphView {
  public:
    CGraphView();
    ~CGraphView();

    void                               addNode(WP<IPwNode> node);
    void                               removeNode(WP<IPwNode> node);

    void                               center();

    SP<Hyprtoolkit::CRectangleElement> m_background;

    WP<CGraphView>                     m_self;

  private:
    SP<CGraphNode>                      nodeFromCoord(const Hyprutils::Math::Vector2D& pos);

    SP<Hyprtoolkit::CNullElement>       m_container;
    SP<Hyprtoolkit::CScrollAreaElement> m_scrollArea;

    std::vector<SP<CGraphNode>>         m_nodes;

    Hyprutils::Math::Vector2D           m_posAtStart, m_lastMousePos, m_elementPosAtStart;
    SP<CGraphNode>                      m_draggingNode;

    bool                                m_mouseDown = false;

    Hyprutils::Math::Vector2D           m_lastInitialPos = {};

    friend class CGraphNode;
};