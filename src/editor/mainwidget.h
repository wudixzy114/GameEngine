#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include <QMap>

class QAction;
class QMenu;
class QToolBar;
class QDockWidget;
class QTextEdit;
class SceneViewWidget;
class PropertyEditorWidget;
class AssetsBrowserWidget;


class MainWidget : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

    ~MainWidget() override;

    void registerAction(const QString &id, QAction *action, const QString &menuPath = QString());

    void addCustomDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newProject();

    void openFile();

    bool saveFile();

    bool saveFileAs();

    void about();

    void playScene();

    void pauseScene();

    void stopScene();

private:
    void createActions();

    void createMenus();

    void createToolBars();

    void createStatusBar();

    void createDockWidgets();

    void createCentralWidget();

    void readSettings();

    void writeSettings();

private:
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *toolsMenu;
    QMenu *gameMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *gameToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;

    QAction *undoAct;
    QAction *redoAct;

    QAction *playAct;
    QAction *pauseAct;
    QAction *stopAct;

    QAction *aboutAct;
    QAction *aboutQtAct;

    QDockWidget *propertyInspectorDockWidget;
    QDockWidget *assetBrowserDockWidget;
    QDockWidget *hierarchyDockWidget;
    QDockWidget *consoleDockWidget;

    SceneViewWidget *m_sceneView;
    PropertyEditorWidget *m_propertyEditor;
    AssetsBrowserWidget *m_assetsBrowser;
    QTextEdit *m_consoleOutput;
};


#endif //MAINWIDGET_H
