//
// Created by wudixzy on 25-5-11.
//

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QMainWindow {
Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;

private:
    Ui::MainWidget *ui;
};


#endif //MAINWIDGET_H
