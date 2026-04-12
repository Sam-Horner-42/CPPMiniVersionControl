#include "MainWindow.h"
#include <QMessageBox>
#include <QFileInfo>

// Author: Sam Horner 040935005
namespace Ui {
    class MainWindow;
}
MainWindow::MainWindow(RepositoryManager* manager, QWidget *parent)
    : QMainWindow(parent)
	, m_repoManager(manager)
{
    ui.setupUi(this);
	setupTabWidgets();
	m_fileWatcher = new QFileSystemWatcher(this);

	connect(m_fileWatcher, &QFileSystemWatcher::fileChanged,
		this, &MainWindow::onFileModified);
}

MainWindow::~MainWindow()
{}


//void MainWindow::setRepoContext(Repository* repo, const QString& name, const QString& path)
void MainWindow::setRepoContext(const QString& name, const QString& path)
{
	this->setWindowTitle(QString("MiniVersionControl - " + name));

	// Clear and populate history
	historyList->clear();
	
}

void MainWindow::onFileModified(const QString& path) {
	// If the repo manager is not null
	if (m_repoManager) {
		// Create a QFileInfo object using the full path
		QFileInfo fileInfo(path);

		// Extract just the file name (e.g., "data.txt") and convert to std::string
		std::string fileName = fileInfo.fileName().toStdString();

		// Pass the extracted file name to the repo manager
		m_repoManager->updateFileStatus(fileName, TrackedFile::status::Modified);

		// Refresh the UI
		refreshFileTable();
	}
}

void MainWindow::setupTabWidgets() {
	// build history tab
	QVBoxLayout* historyLayout = new QVBoxLayout(ui.history);
	historyList = new QListWidget(ui.history);
	// Add mock commit history
	historyLayout->addWidget(historyList);

	// Build diff tab
	QVBoxLayout* diffLayout = new QVBoxLayout(ui.diffTab);
	diffView = new QTextEdit(ui.diffTab);
	
	diffView->setReadOnly(true);

	// Setting a mono font for code/diffs
	QFont monoFont("Courier New", 10);
	diffView->setFont(monoFont);
	diffLayout->addWidget(diffView);

	// Build Analytics tab
	QVBoxLayout* analyticsLayout = new QVBoxLayout(ui.analytics);
	statsLabel = new QLabel(ui.analytics);

	statsLabel->setAlignment(Qt::AlignCenter);
	analyticsLayout->addWidget(statsLabel);
	
}

void MainWindow::refreshFileTable() {
	ui.fileTable->setRowCount(0); // Clear existing rows

	// If the repo manager is not null
	if (m_repoManager) {
		std::vector<TrackedFile> files = m_repoManager->getCurrentFiles();
		ui.fileTable->setColumnCount(2);
		ui.fileTable->setHorizontalHeaderLabels({ "File", "Status" });

		// Loop through the tracked files
		for (size_t i = 0; i < files.size(); ++i) {
			const TrackedFile& file = files[i];

			int rowPosition = ui.fileTable->rowCount();
			ui.fileTable->insertRow(rowPosition);

			// Column 0: File Name
			QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(file.getFileName()));
			ui.fileTable->setItem(rowPosition, 0, nameItem);

			// Column 1: Status
			QTableWidgetItem* statusItem = new QTableWidgetItem(QString::fromStdString(file.getStatusAsString()));
			ui.fileTable->setItem(rowPosition, 1, statusItem);
		}
	}
}

void MainWindow::refreshHistoryTab() {
	// Clear the UI list to prevent duplicates
	historyList->clear();

	// Fetch the vector from the backend
	std::vector<std::string> history = m_repoManager->getCommitHistory();

	// Loop through the vector and add to the QListWidget
	for (const std::string& entry : history) {
		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(entry));

		historyList->addItem(item);
	}

	// Scroll to the bottom so the newest commit is visible
	historyList->scrollToBottom();
}

void MainWindow::on_commitStaged_clicked()
{
	QString commitMessage = ui.commitInput->toPlainText();
	qDebug() << "Commit Staged button clicked. Message:" << commitMessage;
	if (!commitMessage.isEmpty()) {
		//currentRepo->commit(commitMessage.toStdString());
		ui.commitInput->clear();
		QMessageBox::warning(this, "Invalid Commit Message", "Please provide a valid, not empty commit message.");
	}
}

void MainWindow::on_discardChanges_clicked()
{
	// I think we can just call restore to last commit here and update all file statuses
	qDebug() << "Discard Changes button clicked.";
	// Backend hook to discard unstaged changes
}

void MainWindow::on_stageSelected_clicked() {
	qDebug() << "Stage All button clicked.";
	//if(selectedFile != nullptr) m_repoManager->setStaged(selectedFile.toStdString()); // stage the file currently selected at the cell
}

void MainWindow::on_stageAll_clicked()
{
	qDebug() << "Stage All button clicked.";
	//m_repoManager->stageAllFiles();
	// Backend hook to stage all modified/untracked files
}

// The table of files
void MainWindow::on_fileTable_cellClicked(int row, int column)
{
	QString fileName = ui.fileTable->item(row, 0)->text();

	selectedFile = fileName; // set the currently selected file
}


// File menu
void MainWindow::on_actionSettings_3_triggered()
{
	qDebug() << "Settings menu action triggered.";
}

void MainWindow::on_actionExit_2_triggered()
{
	qDebug() << "Exit menu action triggered.";
	close(); // Closes the MainWindow
}

// Repository menu
void MainWindow::on_actionStage_Files_triggered()
{
	qDebug() << "Stage Files action triggered.";
	on_stageAll_clicked();
}

void MainWindow::on_actionCommit_Staged_triggered()
{
	qDebug() << "Commit Staged action triggered from Menu.";
	// Simple call to the button logic to avoid complicating things
	on_commitStaged_clicked();
}

void MainWindow::on_actionCompare_Files_triggered()
{
	qDebug() << "Compare Files action triggered.";
}

void MainWindow::on_actionRestore_to_Prior_Commit_triggered()
{
	qDebug() << "Restore to Prior Commit action triggered.";
}

