#include "UI.hpp"
#include "NodeVolumeSlider.hpp"
#include "DeviceConfig.hpp"
#include "./graph/Graph.hpp"
#include "../pw/PwState.hpp"

using namespace Hyprutils::Math;

constexpr float MAIN_PADDING  = 10;
constexpr float SMALL_PADDING = 6;

CUI::CUI() {
    m_backend = Hyprtoolkit::CBackend::create();
    m_window  = m_backend->openWindow(Hyprtoolkit::SWindowCreationData{
         .title  = "Pipewire Control Center",
         .class_ = "hyprpwcenter",
    });

    m_background = Hyprtoolkit::CRectangleBuilder::begin()->color([this] { return m_backend->getPalette()->m_colors.background; })->commence();

    m_layout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                   ->gap(SMALL_PADDING)
                   ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1, 1}})
                   ->commence();
    m_layout->setMargin(MAIN_PADDING);

    m_tabs.buttonLayout = Hyprtoolkit::CRowLayoutBuilder::begin()
                              ->gap(SMALL_PADDING)
                              ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, 30.F}})
                              ->commence();

    m_tabs.tabContainer = Hyprtoolkit::CScrollAreaBuilder::begin()
                              ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1, 10}})
                              ->scrollY(true)
                              ->commence();
    m_tabs.tabContainer->setGrow(true);

    m_tabs.tabContainerNoScroll =
        Hyprtoolkit::CNullBuilder::begin()->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1, 10}})->commence();
    m_tabs.tabContainerNoScroll->setGrow(true);

    m_tabs.nodesTab.nodesLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                                      ->gap(SMALL_PADDING)
                                      ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                                      ->commence();

    m_tabs.inputsTab.inputsLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                                        ->gap(SMALL_PADDING)
                                        ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                                        ->commence();

    m_tabs.appsTab.appsLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                                    ->gap(SMALL_PADDING)
                                    ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                                    ->commence();

    m_tabs.configTab.configLayout = Hyprtoolkit::CColumnLayoutBuilder::begin()
                                        ->gap(SMALL_PADDING)
                                        ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, {1, 1}})
                                        ->commence();

    m_tabs.nodesButton = Hyprtoolkit::CButtonBuilder::begin()
                             ->label("Nodes")
                             ->noBorder(true)
                             ->onMainClick([this](SP<Hyprtoolkit::CButtonElement>) { changeTab(1); })
                             ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})
                             ->commence();

    m_tabs.inputsButton = Hyprtoolkit::CButtonBuilder::begin()
                              ->label("Inputs")
                              ->noBorder(true)
                              ->onMainClick([this](SP<Hyprtoolkit::CButtonElement>) { changeTab(2); })
                              ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})
                              ->commence();

    m_tabs.appsButton = Hyprtoolkit::CButtonBuilder::begin()
                            ->label("Apps")
                            ->noBorder(true)
                            ->onMainClick([this](SP<Hyprtoolkit::CButtonElement>) { changeTab(0); })
                            ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})
                            ->commence();

    m_tabs.configButton = Hyprtoolkit::CButtonBuilder::begin()
                              ->label("Configuration")
                              ->noBorder(true)
                              ->onMainClick([this](SP<Hyprtoolkit::CButtonElement>) { changeTab(3); })
                              ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})
                              ->commence();

    m_tabs.graphButton = Hyprtoolkit::CButtonBuilder::begin()
                             ->label("Graph")
                             ->noBorder(true)
                             ->onMainClick([this](SP<Hyprtoolkit::CButtonElement>) { changeTab(4); })
                             ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_AUTO, Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, {1.F, 1.F}})
                             ->commence();

    auto hr = Hyprtoolkit::CRectangleBuilder::begin() //
                  ->color([this] { return Hyprtoolkit::CHyprColor{m_backend->getPalette()->m_colors.text.darken(0.65)}; })
                  ->size({Hyprtoolkit::CDynamicSize::HT_SIZE_PERCENT, Hyprtoolkit::CDynamicSize::HT_SIZE_ABSOLUTE, {1.F, 9.F}})
                  ->commence();
    hr->setMargin(4);

    m_window->m_events.closeRequest.listenStatic([this] {
        cleanState();
        m_backend->addIdle([this] {
            m_window->close();
            m_backend->destroy();
        });
    });

    m_window->m_rootElement->addChild(m_background);

    m_background->addChild(m_layout);

    m_layout->addChild(m_tabs.buttonLayout);
    m_layout->addChild(hr);
    m_layout->addChild(m_tabs.tabContainer);

    m_tabs.buttonLayout->addChild(m_tabs.appsButton);
    m_tabs.buttonLayout->addChild(m_tabs.nodesButton);
    m_tabs.buttonLayout->addChild(m_tabs.inputsButton);
    m_tabs.buttonLayout->addChild(m_tabs.configButton);
    m_tabs.buttonLayout->addChild(m_tabs.graphButton);

    changeTab(0);
}

CUI::~CUI() = default;

void CUI::cleanState() {
    m_layout->removeChild(m_tabs.tabContainerNoScroll);
    m_tabs.tabContainerNoScroll.reset();
    m_tabs.graphTab.graphView.reset();
}

void CUI::run() {
    m_tabs.graphTab.graphView         = makeShared<CGraphView>();
    m_tabs.graphTab.graphView->m_self = m_tabs.graphTab.graphView;

    m_backend->addFd(g_pipewire->getFd(), [] { g_pipewire->dispatch(); });

    m_window->open();

    g_pipewire->dispatch();
    m_backend->enterLoop();
}

void CUI::updateNode(WP<IPwNode> node) {
    m_tabs.graphTab.graphView->addNode(node);

    for (const auto& n : m_tabs.nodesTab.nodeSliders) {
        if (n->m_id != node->m_id)
            continue;

        n->setVolume(node->m_volume);
        n->setMuted(node->m_muted);
        return;
    }

    for (const auto& n : m_tabs.inputsTab.inputSliders) {
        if (n->m_id != node->m_id)
            continue;

        n->setVolume(node->m_volume);
        n->setMuted(node->m_muted);
        return;
    }

    for (const auto& n : m_tabs.appsTab.appSliders) {
        if (n->m_id != node->m_id)
            continue;

        n->setVolume(node->m_volume);
        n->setMuted(node->m_muted);
        return;
    }

    SP<CNodeVolumeSlider> x;

    if (node->m_isApp) {
        x = m_tabs.appsTab.appSliders.emplace_back(makeShared<CNodeVolumeSlider>(node->m_id, node->m_name));
        m_tabs.appsTab.appsLayout->addChild(x->m_background);
    } else if (node->m_mediaClass.starts_with("Audio/Source")) {
        x = m_tabs.inputsTab.inputSliders.emplace_back(makeShared<CNodeVolumeSlider>(node->m_id, node->m_name));
        m_tabs.inputsTab.inputsLayout->addChild(x->m_background);
    } else {
        x = m_tabs.nodesTab.nodeSliders.emplace_back(makeShared<CNodeVolumeSlider>(node->m_id, node->m_name));
        m_tabs.nodesTab.nodesLayout->addChild(x->m_background);
    }

    x->setVolume(node->m_volume);
    x->setMuted(node->m_muted);
}

void CUI::nodeRemoved(WP<IPwNode> node) {
    m_tabs.graphTab.graphView->removeNode(node);

    for (const auto& n : m_tabs.nodesTab.nodeSliders) {
        if (n->m_id != node->m_id)
            continue;

        m_tabs.nodesTab.nodesLayout->removeChild(n->m_background);
    }

    for (const auto& n : m_tabs.inputsTab.inputSliders) {
        if (n->m_id != node->m_id)
            continue;

        m_tabs.appsTab.appsLayout->removeChild(n->m_background);
    }

    for (const auto& n : m_tabs.appsTab.appSliders) {
        if (n->m_id != node->m_id)
            continue;

        m_tabs.appsTab.appsLayout->removeChild(n->m_background);
    }

    std::erase_if(m_tabs.nodesTab.nodeSliders, [node](const auto& e) { return !e || e->m_id == node->m_id; });
    std::erase_if(m_tabs.inputsTab.inputSliders, [node](const auto& e) { return !e || e->m_id == node->m_id; });
    std::erase_if(m_tabs.appsTab.appSliders, [node](const auto& e) { return !e || e->m_id == node->m_id; });
}

void CUI::updateDevice(WP<CPipewireDevice> node) {
    for (const auto& n : m_tabs.configTab.deviceConfigs) {
        if (n->m_id != node->m_id)
            continue;

        n->update(node->m_modes, node->m_currentMode);

        return;
    }

    auto x = m_tabs.configTab.deviceConfigs.emplace_back(makeShared<CDeviceConfig>(node->m_id, node->m_name, node->m_modes, node->m_currentMode));
    m_tabs.configTab.configLayout->addChild(x->m_background);
}

void CUI::deviceRemoved(WP<CPipewireDevice> node) {
    for (const auto& n : m_tabs.configTab.deviceConfigs) {
        if (n->m_id != node->m_id)
            continue;

        m_tabs.configTab.configLayout->removeChild(n->m_background);
    }

    std::erase_if(m_tabs.configTab.deviceConfigs, [node](const auto& e) { return !e || e->m_id == node->m_id; });
}

void CUI::changeTab(size_t idx) {
    if (idx == m_tab)
        return;

    m_tabs.tabContainer->clearChildren();

    m_tab = idx;

    if (m_tab == 4) {
        m_layout->removeChild(m_tabs.tabContainer);
        m_layout->addChild(m_tabs.tabContainerNoScroll);
    } else {
        m_layout->removeChild(m_tabs.tabContainerNoScroll);
        m_layout->addChild(m_tabs.tabContainer);
    }

    switch (idx) {
        case 0: m_tabs.tabContainer->addChild(m_tabs.appsTab.appsLayout); break;
        case 1: m_tabs.tabContainer->addChild(m_tabs.nodesTab.nodesLayout); break;
        case 2: m_tabs.tabContainer->addChild(m_tabs.inputsTab.inputsLayout); break;
        case 3: m_tabs.tabContainer->addChild(m_tabs.configTab.configLayout); break;
        case 4: m_tabs.tabContainerNoScroll->addChild(m_tabs.graphTab.graphView->m_background); break;
        default: break;
    }

    if (idx == 4)
        m_tabs.graphTab.graphView->center();
}
