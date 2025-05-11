#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui {
    class NewProjectDialog;
}

QT_END_NAMESPACE

class NewProjectDialog : public QDialog {
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = nullptr);

    ~NewProjectDialog() override;

private:
    Ui::NewProjectDialog *ui;

public:
    QString getProjectName() const;

    QString getProjectPath() const;

private slots:
    void selectProjectPath();

    void validateInputs();

private:
    bool isPathValid() const;

    bool isNameValid() const;
};


#endif //NEWPROJECTDIALOG_H
