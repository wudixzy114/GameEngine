#ifndef ENGINE_H
#define ENGINE_H

#include <QApplication>
#include "mainwidget.h"
#include "startwidget.h"
#include "newprojectdialog.h"

class Editor : public QApplication {
    Q_OBJECT

private:
    std::unique_ptr<StartWidget> startWidget_;
    std::unique_ptr<MainWidget> mainWidget_;
    std::unique_ptr<NewProjectDialog> newProjectWidget_;

public:
    QString projectName_;
    QString projectPath_;

public:
    // 获取单例实例
    static Editor &getInstance(int argc, char **argv) {
        static Editor instance(argc, argv);
        return instance;
    }

    // 禁止拷贝构造和赋值
    Editor(const Editor &) = delete;

    Editor &operator=(const Editor &) = delete;

    // 启动编辑器
    void start();

private:
    Editor(int argc, char **argv);

    ~Editor() override = default;

public:
    void setProjectInfo(const QString &projectName, const QString &projectPath);

    void startMainWidget();

    void startNewProjectWidget();
};

#endif //ENGINE_H
