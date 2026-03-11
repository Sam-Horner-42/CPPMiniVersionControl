#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Test2.h"

class Test2 : public QMainWindow
{
    Q_OBJECT

public:
    Test2(QWidget *parent = nullptr);
    ~Test2();

private:
    Ui::Test2Class ui;
};

