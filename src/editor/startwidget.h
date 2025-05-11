#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include "../Define.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class StartWidget;
}

QT_END_NAMESPACE

class StartWidget : public QWidget {
    Q_OBJECT

public:
    explicit StartWidget(QWidget *parent = nullptr);

    ~StartWidget() override;

private:
    Ui::StartWidget *ui;

private slots:
    void openNewProjectDialog();
};


#endif //STARTWIDGET_H
