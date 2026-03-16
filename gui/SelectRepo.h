#pragma once

#include <QtWidgets/QDialog>
#include "ui_SelectRepo.h"

class SelectRepo : public QDialog
{
    Q_OBJECT

public:
    SelectRepo(QWidget* parent = nullptr);
    ~SelectRepo();
signals:
    void repoSelected(const QString& name, const QString& path); // pass values forward

private slots:
    void on_initRepo_clicked(); // event handler, initializes a new repo
    //void onOpenRepoClicked(); // open an exist
private:
    Ui::SelectRepo ui;
};
