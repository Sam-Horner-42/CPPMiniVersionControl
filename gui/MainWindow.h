// This file serves as the header for the main application window
// This defines all the slots for buttons and the different QObjects for displaying data
// Author: Sam Horner 040935005

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
#include <QFileSystemWatcher>
#include <QDirIterator>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QListWidgetItem>

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
	void refreshAnalyticsTab();
private slots:
	// Push Buttons
	void on_commitStaged_clicked();
	void on_stageAll_clicked();
	void on_stageSelected_clicked();

	// Exit menu option
	void on_actionExit_2_triggered();

	// The Table Widget, files and their status
	void on_fileTable_cellClicked(int row, int column);

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

