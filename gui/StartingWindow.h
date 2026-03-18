#pragma once

#include <QtWidgets/QDialog>
#include "ui_StartingWindow.h"

class StartingWindow : public QDialog
{
    Q_OBJECT

public:
    StartingWindow(QWidget* parent = nullptr);
    ~StartingWindow();
signals:
    void repoSelected(const QString& name, const QString& path); // pass values forward

private slots:
    void on_initRepo_clicked(); // event handler, initializes a new repo
    void on_selectRepo_clicked(); // open an existing repository
private:
    Ui::StartingWindow ui;
};
