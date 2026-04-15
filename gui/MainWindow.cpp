#include "MainWindow.h"

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
	// Watch for specific file modifications (saves, edits)
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged,
        this, &MainWindow::onFileModified);

    // Watch for directory-level changes (new files added, files deleted)
    connect(m_fileWatcher, &QFileSystemWatcher::directoryChanged,
        this, &MainWindow::onDirectoryChanged);
}

MainWindow::~MainWindow()
{}


//void MainWindow::setRepoContext(Repository* repo, const QString& name, const QString& path)
void MainWindow::setRepoContext(const QString& name, const QString& path)
{
	// Sets the window title to display the repository name
	this->setWindowTitle(QString("MiniVersionControl - " + name));

	// Logs the repository path to the debug console for verification
	qDebug() << "This is the repo path " << path;

	// Watches the root repo directory itself for top-level changes
	m_fileWatcher->addPath(path);

	// Creates a QDir object for the repo path (used for directory operations)
	QDir repoDir(path);

	// --- Directory Watcher Loop ---
	// Iterates over all subdirectories recursively, including hidden ones
	QDirIterator dirIt(path, QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot,
		QDirIterator::Subdirectories);
	while (dirIt.hasNext()) {
		QString dirPath = dirIt.next();
		// Skips .vcm internals and metadata.json to avoid watching version control data
		if (!dirPath.contains("/.vcm")) {
			// Registers the directory so new files created inside it trigger a signal
			m_fileWatcher->addPath(dirPath);
		}
	}

	// Iterates over all files recursively, including hidden ones
	QDirIterator fileIt(path, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot,
		QDirIterator::Subdirectories);
	while (fileIt.hasNext()) {
		QString filePath = fileIt.next();
		// Skips any files inside the .vcm folder
		if (!filePath.contains("/.vcm/") && !filePath.contains("metadata.json")) {
			// Registers the file so modifications to it trigger a signal
			m_fileWatcher->addPath(filePath);
		}
	}

	// Rebuilds the file table UI to reflect the current state of tracked files
	refreshFileTable();

	// Clears the history panel ready to be repopulated
	refreshHistoryTab();
	
}

void MainWindow::onFileModified(const QString& path) {
	// If the repo manager is not null
	if (!m_repoManager) return;

	QFileInfo fileInfo(path);
	std::string fileName = fileInfo.fileName().toStdString();

	if (fileInfo.exists()) {
		// The file still exists, so it was Modified
		qDebug() << "File modified:" << path;
		m_repoManager->updateFileStatus(fileName, TrackedFile::status::Modified);
	}
	else {
		// The file no longer exists, so it was Deleted
		qDebug() << "File deleted:" << path;
		m_repoManager->deleteTrackedFile(fileName);

		// Remove it from the watcher since it's gone
		m_fileWatcher->removePath(path);
	}

	refreshFileTable();
}

void MainWindow::onDirectoryChanged(const QString& path) {
	if (!m_repoManager) return;

	QDir dir(path);
	QFileInfoList diskFiles = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot);

	const std::vector<TrackedFile>& trackedFiles = m_repoManager->getCurrentFiles();
	const QString vcmMarker = QDir::separator() + QString(".vcm") + QDir::separator();

	// Single loop to evaluate and register files
	for (const QFileInfo& fileInfo : diskFiles) {
		const QString absPath = fileInfo.absoluteFilePath();
		const QString fileName = fileInfo.fileName();

		// Skip internal version control directories and metadata files
		if (absPath.contains(vcmMarker) || fileName == ".vcm" || fileName == "metadata.json") {
			continue;
		}

		// Check if the file is already tracked (Using absolute path to prevent overwrite bug)
		std::string absPathStr = absPath.toStdString();
		bool alreadyTracked = std::any_of(
			trackedFiles.begin(), trackedFiles.end(),
			[&absPathStr](const TrackedFile& f) {
				return f.getFilePath() == absPathStr;
			}
		);

		// Register the file if it's new and currently exists on disk
		if (!alreadyTracked && fileInfo.exists()) {
			qDebug() << "Registering new file:" << fileName;

			m_repoManager->addNewTrackedFile(absPathStr, fileName.toStdString());
			m_fileWatcher->addPath(absPath);
		}
	}

	refreshFileTable();
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
		qDebug() << "Before getting current files.";
		std::vector<TrackedFile> files = m_repoManager->getCurrentFiles();
		
		qDebug() << "After getting current files.";
		ui.fileTable->setColumnCount(2);
		ui.fileTable->setHorizontalHeaderLabels({ "File", "Status" });

		// Loop through the tracked files
		for (size_t i = 0; i < files.size(); ++i) {
			
			const TrackedFile& file = files[i];
			qDebug() << "Tracked File Name: " << QString::fromStdString(file.getFileName());

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
//	auto& m_repoManager->getRepoCommits();
	// Clear the UI list to prevent duplicates
	historyList->clear();

	// Fetch the vector from the backend
	auto& history = m_repoManager->getRepoCommits();
	

	// Loop through the vector and add to the QListWidget
//	for (auto& entry : history) {
////		qDebug() << "History value: " << entry;
//		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(entry));
//
//		historyList->addItem(item);
//	}

	// Scroll to the bottom so the newest commit is visible
	historyList->scrollToBottom();
}

//void MainWindow::refreshAnayticsTab() {
//	if (m_repoManager) {
//		m_repoManager->getAnalytics();
//	}
//}

void MainWindow::on_commitStaged_clicked()
{
	QString commitMessage = ui.commitInput->toPlainText();
	qDebug() << "Commit Staged button clicked. Message:" << commitMessage;
	if (!commitMessage.isEmpty()) {
		
		if(!m_repoManager->commitStagedFiles(commitMessage.toStdString())){
		QMessageBox::warning(this, "No Currently Staged Files", "You must stage at least one file to perform a commit."); 
		}
		ui.commitInput->clear();
		
	} else { QMessageBox::warning(this, "Invalid Commit Message", "Please provide a valid, not empty commit message."); }

	auto& commits = m_repoManager->getRepoCommits();
	for (auto& commit : commits) {
		qDebug() << "Commit Id: " << commit->getId();
		qDebug() << "Commit Author: " << commit->getAuthor();
		qDebug() << "Commit Timestamp: " << commit->getTimestamp();
		qDebug() << "Commit Message: " << commit->getMessage();
	}
	refreshFileTable();
	//auto& std::vector<Commit> = m_repoManager->getCommits(); // Needs to get the vector of commits so I can display them and check if they exist
	
}



void MainWindow::on_stageSelected_clicked() {
	qDebug() << "Stage All button clicked.";
	if(!selectedFile.isEmpty()) m_repoManager->stageFile(selectedFile.toStdString());
	refreshFileTable();
}

void MainWindow::on_stageAll_clicked()
{
	qDebug() << "Stage All button clicked.";
	m_repoManager->stageAllFiles();
	refreshFileTable();
}

// 
void MainWindow::on_restoreToCommit_clicked() {
	
}

// The table of files
void MainWindow::on_fileTable_cellClicked(int row, int column)
{
	QString fileName = ui.fileTable->item(row, 0)->text();
	selectedFile = fileName; // set the currently selected file
	qDebug() << "Selected File: " << selectedFile;
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

	// May need a new window here to select commits
}

