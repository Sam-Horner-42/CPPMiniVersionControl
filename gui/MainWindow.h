#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "../includes/RepositoryManager.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QTextEdit>
#include <QLabel>
#include <QDir>
#include <QMessageBox>
#include <QFileInfo>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QDirIterator>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QListWidgetItem>

//#include "../includes/Repository.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(RepositoryManager* repositoryManager, QWidget *parent = nullptr);
    ~MainWindow();
    
	void setRepoContext(const QString& name, const QString& path);
	void refreshFileTable();
	void refreshHistoryTab();
	void refreshDiffTab();
private slots:
	// Push Buttons
	void on_commitStaged_clicked();
	void on_stageAll_clicked();
	void on_stageSelected_clicked();
	void on_restoreToCommit_clicked();

	// The Table Widget, files and their status
	void on_fileTable_cellClicked(int row, int column);

	// File menu actions
	void on_actionSettings_3_triggered();
	void on_actionExit_2_triggered();

	// Repository menu action
	void on_actionStage_Files_triggered();
	void on_actionCommit_Staged_triggered(); 
	void on_actionCompare_Files_triggered();
	void on_actionRestore_to_Prior_Commit_triggered();

	void onFileModified(const QString& path);

	void onDirectoryChanged(const QString& path);

private:
    Ui::MainWindow ui;
	// History Tab Widgets
	QListWidget* historyList;

	RepositoryManager* m_repoManager;

	// Diff Tab Widgets
	QTextEdit* diffView;

	// Analytics Tab Widgets
	QLabel* statsLabel;

	void setupTabWidgets(); // Helper to initialize layouts

	QString selectedFile;
	QFileSystemWatcher* m_fileWatcher; // watches for modified files
};

