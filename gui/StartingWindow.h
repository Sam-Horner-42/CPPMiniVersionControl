#pragma once

#include <QtWidgets/QDialog>
#include "ui_StartingWindow.h"
#include "../includes/RepositoryManager.h"

class StartingWindow : public QDialog
{
    Q_OBJECT

public:
    StartingWindow(RepositoryManager* manager, QWidget* parent = nullptr);
    ~StartingWindow();
signals:
	// takes in the repo name and path, the boolean determines whether this is a new repository or an existing one
    void repoSelected(const QString& name, const QString& path, bool isNew); // pass values forward

private slots:
    void on_initRepo_clicked(); // event handler, initializes a new repo
    void on_selectRepo_clicked(); // open an existing repository
private:
    Ui::StartingWindow ui;
	RepositoryManager* m_repoManager;
};
