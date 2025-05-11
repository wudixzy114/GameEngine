#include <QApplication>
#include "mainwidget.h"
#include "startwidget.h"
#include "Editor.h"
#include "spdlog/spdlog.h"

Editor::Editor(int argc, char **argv) : QApplication(argc, argv) {
    // 可在此初始化其他资源（如果需要）
}

// 启动编辑器
void Editor::start() {
    if (!startWidget_) {
        startWidget_ = std::make_unique<StartWidget>();
    }
    startWidget_->show();
    spdlog::info("Editor started successfully");
}

// 开始主窗口
void Editor::startMainWidget() {
    if (!mainWidget_) {
        mainWidget_ = std::make_unique<MainWidget>();
        mainWidget_->show();
    }
    if (startWidget_) {
        startWidget_->hide();
    }
}

void Editor::startNewProjectWidget() {
    if (!newProjectWidget_) {
        newProjectWidget_ = std::make_unique<NewProjectDialog>();
        newProjectWidget_->show();
    }
}

void Editor::setProjectInfo(const QString &projectName, const QString &projectPath) {
    projectName_ = projectName;
    projectPath_ = projectPath;
}
