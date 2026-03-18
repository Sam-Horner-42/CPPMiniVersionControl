#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setRepoContext(const QString& name, const QString& path);

private slots:
    //void on_initRepo_clicked(); // event handler, initializes a new repo
    //void on_selectRepo_clicked(); // open an existing repository

private:
    Ui::MainWindow ui;
};

