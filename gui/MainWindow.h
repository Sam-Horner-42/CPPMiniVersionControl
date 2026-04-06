#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
//#include "../includes/Repository.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
	// mainWindow needs access to the Repository pointer
	//void setRepoContext(Repository* repo, const QString& name, const QString& path);
	void setRepoContext(const QString& name, const QString& path);
private slots:
    // All of the buttons on the MainWindow belong here
private slots:
	// Push Buttons
	void on_commitStaged_clicked();
	void on_discardChanges_clicked();
	void on_stageAll_clicked();

	// The Table Widget, files and their status
	void on_fileTable_cellClicked(int row, int column);

	// File menu actions
	void on_actionSettings_3_triggered();
	void on_actionExit_2_triggered();

	// Repository menu action
	void on_actionInitialize_Local_Repository_triggered();
	void on_actionOpen_Repository_2_triggered();
	void on_actionClone_Repository_2_triggered();
	void on_actionStage_Files_triggered();
	void on_actionCommit_Staged_triggered(); 
	void on_actionCompare_Files_triggered();
	void on_actionRestore_to_Prior_Commit_triggered();

private:
    Ui::MainWindow ui;
	// History Tab Widgets
	QListWidget* historyList;

	// Diff Tab Widgets
	QTextEdit* diffView;

	// Analytics Tab Widgets
	QLabel* statsLabel;

	void setupTabWidgets(); // Helper to initialize layouts
	//Repository* currentRepo = nullptr; // Store pointer to the active repo
};

