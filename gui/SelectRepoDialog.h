// This is the header file for the select repo dialog window
// Author: Sam Horner 040935005
#pragma once

#include <QtWidgets/QDialog>
#include <QString>
#include <QStandardItemModel>
#include <QMap> 
#include <QDebug>
#include "ui_SelectRepoDialog.h"
#include "../includes/RepositoryManager.h"

// Struct to bundle the repo data together
struct RepoInfo {
	QString name;
	QString id;
	QString path;
};

class SelectRepoDialog : public QDialog
{
	Q_OBJECT

public:
	SelectRepoDialog(RepositoryManager* repoManager, QWidget* parent = nullptr);
	~SelectRepoDialog();

	// Methods to get the data back to StartingWindow
	QString getSelectedRepoName() const;
	QString getSelectedRepoPath() const;

private slots:
	void on_okButton_clicked();

private:
	Ui::SelectRepoDialog ui;
	RepositoryManager* m_repoManager;
	QStandardItemModel* model; // This model is where the repository data is sent to for display 

	// Map of repo names and file paths
	QMap<QString, RepoInfo> repoMap;
};