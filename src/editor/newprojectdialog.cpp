#include "newprojectdialog.h"
#include "ui_NewProjectDialog.h"
#include <QFileDialog>
#include <qregularexpression.h>
#include <spdlog/spdlog.h>


NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewProjectDialog) {
    ui->setupUi(this);
    setWindowTitle("Create a New Project");

    connect(ui->Browser_PB, &QPushButton::clicked, this, &NewProjectDialog::selectProjectPath);
    connect(ui->Cancel_PB, &QPushButton::clicked, this, &NewProjectDialog::reject);
    connect(ui->Create_PB, &QPushButton::clicked, this, &NewProjectDialog::accept);
    connect(ui->ProjectPath_LE, &QLineEdit::textChanged, this, &NewProjectDialog::validateInputs);
    connect(ui->ProjectName_LE, &QLineEdit::textChanged, this, &NewProjectDialog::validateInputs);

    validateInputs();
}

NewProjectDialog::~NewProjectDialog() {
    delete ui;
}

QString NewProjectDialog::getProjectName() const {
    return ui->ProjectName_LE->text().trimmed();
}

QString NewProjectDialog::getProjectPath() const {
    return ui->ProjectPath_LE->text().trimmed();
}

void NewProjectDialog::selectProjectPath() {
    QString startPath = ui->ProjectPath_LE->text().trimmed();
    if (startPath.isEmpty()) {
        startPath = QDir::homePath();
    }

    QString dir = QFileDialog::getExistingDirectory(this, tr("choose path"), startPath);
    if (!dir.isEmpty()) {
        ui->ProjectPath_LE->setText(dir);
        spdlog::info("Selected project path: {}", dir.toStdString());
    } else {
        spdlog::info("Project path selection cancelled");
    }

    validateInputs();
}

void NewProjectDialog::validateInputs() {
    bool isNameValid = this->isNameValid();
    if (!isNameValid) {
        ui->Create_PB->setEnabled(false);
        return;
    }
    bool isPathValid = this->isPathValid();
    if (!isPathValid) {
        ui->Create_PB->setEnabled(false);
        return;
    }
    ui->ProjectState_LB->setText("OK");
    ui->Create_PB->setEnabled(true);
}

bool NewProjectDialog::isPathValid() const {
    QString path = getProjectPath();
    if (path.isEmpty()) {
        ui->ProjectState_LB->setText("Path is Empty");
        return false;
    }
    const QDir dir(path);
    if (!dir.exists()) {
        ui->ProjectState_LB->setText("Dir not Exists");
        return false;
    }
    const QFileInfo dirInfo(path);
    if (!dirInfo.isWritable()) {
        ui->ProjectState_LB->setText("Path is not writable");
        return false;
    }
    QStringList entries = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    if (!entries.isEmpty()) {
        ui->ProjectState_LB->setText("Dir is not Empty");
        return false;
    }
    return true;
}

bool NewProjectDialog::isNameValid() const {
    if (getProjectName().isEmpty()) {
        ui->ProjectState_LB->setText("Name is Empty");
        return false;
    }
    if (getProjectName().contains(QRegularExpression("[<>:\"/\\\\|?*]"))) {
        ui->ProjectState_LB->setText("Name is illegal");
        return false;
    }
    return true;
}
