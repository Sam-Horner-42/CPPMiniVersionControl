// SelectRepoDialog.h
#pragma once
#include <QtWidgets/QDialog>
#include "ui_SelectRepoDialog.h"

class SelectRepoDialog : public QDialog
{
    Q_OBJECT

public:
    SelectRepoDialog(QWidget* parent = nullptr);
    ~SelectRepoDialog();

private slots:
    void on_okButton_clicked();

private:

    Ui::SelectRepoDialog ui;
};