#include "startwidget.h"

#include "newprojectdialog.h"
#include "ui_StartWidget.h"
#include <QMessageBox>
#include <spdlog/spdlog.h>

#include "Editor.h"

StartWidget::StartWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StartWidget) {
    ui->setupUi(this);

    connect(ui->Create_PB, &QPushButton::clicked, this, &StartWidget::openNewProjectDialog);
}

StartWidget::~StartWidget() {
    delete ui;
}

void StartWidget::openNewProjectDialog() {
    NewProjectDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        const QString projectName = dialog.getProjectName();
        const QString projectPath = dialog.getProjectPath();
        if (projectName.isEmpty() || projectPath.isEmpty()) {
            QMessageBox::warning(this, "error", "name or path cannot be empty");
            spdlog::warn("Cannot create project, error in openNewProjectDialog");
            return;
        }
        Editor::getInstance(0, nullptr).setProjectInfo(projectName, projectPath);
        Editor::getInstance(0, nullptr).startMainWidget();
    }
}
