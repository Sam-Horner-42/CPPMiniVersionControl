// InitRepoDialog.h
#pragma once
#include <QtWidgets/QDialog>
#include <QFileDialog>
#include "ui_InitRepoDialog.h"

class InitRepoDialog : public QDialog
{
    Q_OBJECT

public:
    InitRepoDialog(QWidget* parent = nullptr);
    ~InitRepoDialog();

    QString getRepoName() const;
    QString getRepoPath() const;

private slots:
    void on_okButton_clicked();
    void on_browseButton_clicked(); // file picker

private:
    Ui::InitRepoDialog ui;
};