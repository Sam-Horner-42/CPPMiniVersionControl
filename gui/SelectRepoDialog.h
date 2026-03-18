// SelectRepoDialog.h
#pragma once
#include <QtWidgets/QDialog>
#include<QString>
#include<QStandardItemModel>
#include <QDebug>
#include "ui_SelectRepoDialog.h"

class SelectRepoDialog : public QDialog
{
    Q_OBJECT

public:
    SelectRepoDialog(QWidget* parent = nullptr);
    ~SelectRepoDialog();

    // Methods to get the data back to StartingWindow
    QString getSelectedRepoName() const;
    QString getSelectedRepoPath() const;

private slots:
    void on_okButton_clicked();

private:

    Ui::SelectRepoDialog ui;
    QStandardItemModel* model; // 

    // Map of repo names and file paths
    QMap<QString, QString> repoMap;
};