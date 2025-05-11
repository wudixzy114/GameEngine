#include "newprojectdialog.h"
#include "ui_NewProjectDialog.h"
#include <QFileDialog>
#include <qregularexpression.h>
#include <spdlog/spdlog.h>


// 构造函数，初始化对话框
NewProjectDialog::NewProjectDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NewProjectDialog) {
    ui->setupUi(this);
    setWindowTitle("Create a New Project");

    // 连接按钮的点击事件到相应的槽函数
    connect(ui->Browser_PB, &QPushButton::clicked, this, &NewProjectDialog::selectProjectPath);
    connect(ui->Cancel_PB, &QPushButton::clicked, this, &NewProjectDialog::reject);
    connect(ui->Create_PB, &QPushButton::clicked, this, &NewProjectDialog::accept);
    connect(ui->ProjectPath_LE, &QLineEdit::textChanged, this, &NewProjectDialog::validateInputs);
    connect(ui->ProjectName_LE, &QLineEdit::textChanged, this, &NewProjectDialog::validateInputs);

    // 验证输入
    validateInputs();
}

// 析构函数，删除ui指针
NewProjectDialog::~NewProjectDialog() {
    delete ui;
}

// 获取项目名称
QString NewProjectDialog::getProjectName() const {
    return ui->ProjectName_LE->text().trimmed();
}

// 获取项目路径
QString NewProjectDialog::getProjectPath() const {
    return ui->ProjectPath_LE->text().trimmed();
}

// 选择项目路径
void NewProjectDialog::selectProjectPath() {
    QString startPath = ui->ProjectPath_LE->text().trimmed();
    if (startPath.isEmpty()) {
        startPath = QDir::currentPath();
    }

    // 弹出文件选择对话框，选择项目路径
    QString dir = QFileDialog::getExistingDirectory(this, tr("choose path"), startPath);
    if (!dir.isEmpty()) {
        ui->ProjectPath_LE->setText(dir);
        spdlog::info("Selected project path: {}", dir.toStdString());
    } else {
        spdlog::info("Project path selection cancelled");
    }

    // 验证输入
    validateInputs();
}

// 验证输入
void NewProjectDialog::validateInputs() {
    bool isNameValid = this->isNameValid();
    if (!isNameValid) {
        ui->Create_PB->setEnabled(false);
        ui->ProjectState_LB->setStyleSheet("QLabel { color: red; }");
        return;
    }
    bool isPathValid = this->isPathValid();
    if (!isPathValid) {
        ui->Create_PB->setEnabled(false);
        ui->ProjectState_LB->setStyleSheet("QLabel { color: red; }");
        return;
    }
    ui->ProjectState_LB->setText("OK");
    ui->ProjectState_LB->setStyleSheet("QLabel { color: green; }");
    ui->Create_PB->setEnabled(true);
}

// 验证路径是否有效
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

// 验证名称是否有效
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
