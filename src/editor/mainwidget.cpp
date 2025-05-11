#include "mainwidget.h"
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextEdit>
#include <QCloseEvent>
#include <QSettings>
#include <QIcon>
#include <QDesktopServices>
#include <QUrl>

MainWidget::MainWidget(QWidget *parent) : QMainWindow(parent) {
    QCoreApplication::setOrganizationName("RedFish");
    QCoreApplication::setApplicationName("RedFishGameEngine");

    m_consoleOutput = new QTextEdit(this);
    m_consoleOutput->setReadOnly(true);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createCentralWidget();
    createDockWidgets();

    setWindowTitle("RedFish GameEngine");

    readSettings();

    statusBar()->showMessage(tr("Ready"), 2000);
}

MainWidget::~MainWidget() = default;

void MainWidget::createActions() {
    newAct = new QAction(QIcon(":/icons/new.png"), tr("&New Project..."), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new project"));
    connect(newAct, &QAction::triggered, this, &MainWidget::newProject);

    openAct = new QAction(QIcon(":/icons/open.png"), tr("&Open Project..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing project"));
    connect(openAct, &QAction::triggered, this, &MainWidget::openFile);

    saveAct = new QAction(QIcon(":/icons/save.png"), tr("&Save Project"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current project"));
    connect(saveAct, &QAction::triggered, this, &MainWidget::saveFile);

    saveAsAct = new QAction(tr("Save Project &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the project under a new name"));
    connect(saveAsAct, &QAction::triggered, this, &MainWidget::saveFileAs);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    undoAct = new QAction(QIcon(":/icons/undo.png"), tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    // connect(undoAct, &QAction::triggered, ...); // Connect to undo stack
    undoAct->setEnabled(false); // Initially

    redoAct = new QAction(QIcon(":/icons/redo.png"), tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    // connect(redoAct, &QAction::triggered, ...); // Connect to redo stack
    redoAct->setEnabled(false); // Initially

    playAct = new QAction(QIcon(":/icons/play.png"), tr("&Play"), this);
    playAct->setShortcut(Qt::Key_F5);
    playAct->setStatusTip(tr("Play the current scene"));
    connect(playAct, &QAction::triggered, this, &MainWidget::playScene);

    pauseAct = new QAction(QIcon(":/icons/pause.png"), tr("&Pause"), this);
    pauseAct->setShortcut(Qt::Key_F6);
    pauseAct->setStatusTip(tr("Pause the game"));
    pauseAct->setCheckable(true); // It's a toggle
    // connect(pauseAct, &QAction::triggered, this, &MainWidget::pauseScene);
    pauseAct->setEnabled(false); // Enabled when playing

    stopAct = new QAction(QIcon(":/icons/stop.png"), tr("&Stop"), this);
    stopAct->setShortcut(Qt::Key_F7);
    stopAct->setStatusTip(tr("Stop the game"));
    // connect(stopAct, &QAction::triggered, this, &MainWidget::stopScene);
    stopAct->setEnabled(false); // Enabled when playing

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWidget::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWidget::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));
    toolsMenu = menuBar()->addMenu(tr("&Tools"));

    gameMenu = menuBar()->addMenu(tr("&Game"));
    gameMenu->addAction(playAct);
    gameMenu->addAction(pauseAct);
    gameMenu->addAction(stopAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWidget::createToolBars() {
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName("FileToolBar");
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName("EditToolBar");
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);

    gameToolBar = addToolBar(tr("Game Control"));
    gameToolBar->setObjectName("GameControlToolBar");
    gameToolBar->addAction(playAct);
    gameToolBar->addAction(pauseAct);
    gameToolBar->addAction(stopAct);
}

void MainWidget::createStatusBar() {
    // pass
}

void MainWidget::createCentralWidget() {
    // --- Example: Using a simple QTextEdit as central widget ---
    // If you have a SceneViewWidget, use that:
    // m_sceneView = new SceneViewWidget(this);
    // setCentralWidget(m_sceneView);

    // For now, let's use a placeholder
    QTextEdit *placeholderCentral = new QTextEdit("Main Scene View / Editor Area", this);
    placeholderCentral->setAlignment(Qt::AlignCenter);
    placeholderCentral->setReadOnly(true);
    setCentralWidget(placeholderCentral);
}

void MainWidget::createDockWidgets() {
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    propertyInspectorDockWidget = new QDockWidget(tr("Property Inspector"), this);
    propertyInspectorDockWidget->setObjectName("PropertyInspectorDock");
    QTextEdit *tempPropEdit = new QTextEdit("Property Editor Content", propertyInspectorDockWidget);
    propertyInspectorDockWidget->setWidget(tempPropEdit);
    addDockWidget(Qt::RightDockWidgetArea, propertyInspectorDockWidget);
    viewMenu->addAction(propertyInspectorDockWidget->toggleViewAction());

    assetBrowserDockWidget = new QDockWidget(tr("Asset Browser"), this);
    assetBrowserDockWidget->setObjectName("AssetBrowserDock");
    // m_assetBrowser = new AssetBrowserWidget(assetBrowserDockWidget);
    QTextEdit *tempAssetBrowser = new QTextEdit("Asset Browser Content", assetBrowserDockWidget); // Placeholder
    assetBrowserDockWidget->setWidget(tempAssetBrowser /* m_assetBrowser */);
    addDockWidget(Qt::BottomDockWidgetArea, assetBrowserDockWidget);
    viewMenu->addAction(assetBrowserDockWidget->toggleViewAction());

    hierarchyDockWidget = new QDockWidget(tr("Scene Hierarchy"), this);
    hierarchyDockWidget->setObjectName("HierarchyDock");
    QTextEdit *tempHierarchy = new QTextEdit("Scene Hierarchy Content", hierarchyDockWidget); // Placeholder
    hierarchyDockWidget->setWidget(tempHierarchy);
    addDockWidget(Qt::LeftDockWidgetArea, hierarchyDockWidget);
    viewMenu->addAction(hierarchyDockWidget->toggleViewAction());

    consoleDockWidget = new QDockWidget(tr("Console"), this);
    consoleDockWidget->setObjectName("ConsoleDock");
    consoleDockWidget->setWidget(m_consoleOutput); // Use the pre-created m_consoleOutput
    addDockWidget(Qt::BottomDockWidgetArea, consoleDockWidget);
    viewMenu->addAction(consoleDockWidget->toggleViewAction());

    tabifyDockWidget(assetBrowserDockWidget, consoleDockWidget);
    assetBrowserDockWidget->raise();
}

void MainWidget::closeEvent(QCloseEvent *event) {
    writeSettings();
    event->accept();
}

void MainWidget::newProject() {
    // pass
}

void MainWidget::openFile() {
}

bool MainWidget::saveFile() {
}

bool MainWidget::saveFileAs() {
}

void MainWidget::about() {
}

void MainWidget::playScene() {
}

void MainWidget::pauseScene() {
}

void MainWidget::stopScene() {
}

void MainWidget::readSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    const QByteArray state = settings.value("windowState", QByteArray()).toByteArray();
    if (!state.isEmpty()) {
        restoreState(state);
    }
}

void MainWidget::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
}

void MainWidget::registerAction(const QString &id, QAction *action, const QString &menuPath) {
    // TODO: Implement logic to add this action to a menu (specified by menuPath, e.g., "Tools/MyPlugin/DoSomething")
    // and potentially a toolbar.
    // m_registeredActions.insert(id, action);
    // Example: if menuPath is "Tools/MyPlugin/Action1"
    // QMenu* targetMenu = toolsMenu; // or find/create based on path
    // targetMenu->addAction(action);

    Q_UNUSED(id);
    Q_UNUSED(action);
    Q_UNUSED(menuPath);
    m_consoleOutput->append("Extensibility: Action registered (not fully implemented).");
}

void MainWidget::addCustomDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) {
    if (!dockWidget) return;
    dockWidget->setParent(this); // Ensure proper parentage
    addDockWidget(area, dockWidget);
    if (viewMenu) {
        // Ensure viewMenu is created
        viewMenu->addAction(dockWidget->toggleViewAction());
    }
    m_consoleOutput->append(QString("Extensibility: Dock widget '%1' added.").arg(dockWidget->windowTitle()));
}




