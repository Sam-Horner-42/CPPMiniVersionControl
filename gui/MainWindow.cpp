// This file is the main application window for the mini version control application
// The repository is routed here via starting window after being either loaded or initialized
// 

// Author: Sam Horner 040935005
#include "MainWindow.h"
namespace Ui {
    class MainWindow;
}

/*
This is the main window that the applicaiton runs within
Once a repository is created either by loading or initializing a new repo
this window loads given the RepositoryManager object created main
*/
MainWindow::MainWindow(RepositoryManager* manager, QWidget *parent)
    : QMainWindow(parent)
	, m_repoManager(manager)
{
    ui.setupUi(this);

	setupTabWidgets(); // Create the different widgets for displaying data from the backend
	m_fileWatcher = new QFileSystemWatcher(this); // Watches the directory for changes to updated file status/remove files
	// Watch for specific file modifications (saves, edits)
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged,
        this, &MainWindow::onFileModified);

    // Watch for directory-level changes (new files added, files deleted)
    connect(m_fileWatcher, &QFileSystemWatcher::directoryChanged,
        this, &MainWindow::onDirectoryChanged);
}

MainWindow::~MainWindow()
{}

/* 
Defines the name and path for the current repository 
This sets up the title for the window based on the name of the Repository
It also adds the specified path for the repository to the file watcher so when files
are updated in the directory the GUI is aware and updates correctly
*/
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

	// Directory watcher loop
	// Iterates over all subdirectories recursively, including hidden ones
	QDirIterator dirIt(path, QDir::Dirs | QDir::Hidden | QDir::NoDotAndDotDot,
		QDirIterator::Subdirectories);
	while (dirIt.hasNext()) {
		QString dirPath = dirIt.next();
		// Skips .vcm internals to avoid watching version control data
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

	// Refresh the analytics tab for this repo
	refreshAnalyticsTab();
	
}

void MainWindow::setupTabWidgets() {
	// build history tab
	QVBoxLayout* historyLayout = new QVBoxLayout(ui.history);
	historyList = new QListWidget(ui.history);
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
	refreshAnalyticsTab();
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
			qDebug() << "Registering new file: " << fileName;

			m_repoManager->addNewTrackedFile(absPathStr, fileName.toStdString());
			m_fileWatcher->addPath(absPath);
		}
	}

	refreshFileTable();
	refreshAnalyticsTab();
}

// Refresh the file table with the currently tracked files, name | status
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

// Refresh the history tab with the commit history retrieved from Repository.cpp
void MainWindow::refreshHistoryTab() {
	qDebug() << "Refresh history tab called.";
	
	// Clear the UI list to prevent duplicates
	historyList->clear();

	// Fetch the vectors from the backend
	auto commitIds = m_repoManager->getAllCommitIds();
	auto history = m_repoManager->getCommitHistory();

	// Safety check: Ensure both vectors are the same size to prevent crashes
	if (commitIds.size() != history.size()) {
		qDebug() << "Error: Mismatch between number of commit IDs and history entries.";
		return;
	}

	// Use an index-based loop to access both vectors simultaneously
	for (size_t i = 0; i < history.size(); ++i) {
		std::string currentEntry = history[i];
		std::string currentId = commitIds[i];

		qDebug() << "History value: " << QString::fromStdString(currentEntry);

		// Create a blank QListWidgetItem
		QListWidgetItem* item = new QListWidgetItem(historyList);

		// Create the custom row widget and layout
		QWidget* rowWidget = new QWidget();
		QHBoxLayout* layout = new QHBoxLayout(rowWidget);
		layout->setContentsMargins(5, 2, 5, 2); // Added a little horizontal padding

		// Create a Label for the history text
		QLabel* historyLabel = new QLabel(QString::fromStdString(currentEntry));

		// Create the "Restore" button
		QPushButton* restoreButton = new QPushButton("Restore");
		restoreButton->setCursor(Qt::PointingHandCursor);

		// Connect the button, passing the currentId to the lambda
		connect(restoreButton, &QPushButton::clicked, this, [this, currentId]() {
			qDebug() << "Restoring to commit ID:" << QString::fromStdString(currentId);

			// restore to the id passed in
			m_repoManager->restore(currentId);
			refreshHistoryTab();
			});

		// Add widgets to the layout
		// The '1' adds a stretch factor to the label, pushing the button completely to the right
		layout->addWidget(historyLabel, 1);
		layout->addWidget(restoreButton);

		rowWidget->setLayout(layout);

		// Embed the row widget into the list
		item->setSizeHint(rowWidget->sizeHint());
		historyList->setItemWidget(item, rowWidget);
	}

	// Scroll to the bottom so the newest commit is visible
	historyList->scrollToBottom();
}


// Calls differentiation on the currently selected file to display differences between them
void MainWindow::refreshDiffTab()
{
	if(!m_repoManager) return;
	// Clear the view if no file is selected
	if (selectedFile.isEmpty()) {
		diffView->clear();
		return;
	}

	// Fetch the diff string from the repository manager.
	std::string diffStr = m_repoManager->callParentDifferentiation(selectedFile.toStdString());

	// Set the text in the diffView
	diffView->setPlainText(QString::fromStdString(diffStr));
}

// Refresh the analytics tab with the newly computed analytics
void MainWindow::refreshAnalyticsTab() {
	if(!m_repoManager) return;
	// Fetch the vector of strings from the backend
	std::vector<std::string> analytics = m_repoManager->getAnalytics();

	// Start building the HTML string with a header
	QString displayText = "<h2>Repository Analytics</h2><br>";

	// Loop through each string in the vector
	for (const std::string& stat : analytics) {
		// Convert to QString, add a line break, and append to the main text
		displayText += QString::fromStdString(stat) + "<br>";
	}

	// Apply the formatted text to the label
	statsLabel->setText(displayText);
}

// When the commit staged button is pressed
// The backend handles checking for staged files and changing their status, and then creating a new commit and populating it
void MainWindow::on_commitStaged_clicked()
{
	// Get the commit message
	QString commitMessage = ui.commitInput->toPlainText().trimmed();
	qDebug() << "Commit Staged button clicked. Message:" << commitMessage;

	// 1-200 characters, Alphanumeric only
	QRegularExpression rx("^[A-Za-z0-9 ]{1,200}$"); // Added space ' ' to allowed chars
	QRegularExpressionMatch match = rx.match(commitMessage);
	// Commit message must have information and be aplahnumeric
	if (match.hasMatch() && !commitMessage.isEmpty()) {
		
		if(!m_repoManager->commitStagedFiles(commitMessage.toStdString())){
		QMessageBox::warning(this, "No Currently Staged Files", "You must stage at least one file to perform a commit."); 
		}
		else {
			ui.commitInput->clear(); // clear the valid commit message
		}
		
		
	} else { 
		QMessageBox::warning(this, "Invalid Commit Message", "Commit messages can only containt alphanumeric values.\nPlease provide a valid, not empty commit message."); 
		ui.commitInput->clear(); // clear the invalid message
	}

	auto& commits = m_repoManager->getRepoCommits();
	for (auto& commit : commits) {
		qDebug() << "Commit Id: " << commit->getId();
		qDebug() << "Commit Author: " << commit->getAuthor();
		qDebug() << "Commit Timestamp: " << commit->getTimestamp();
		qDebug() << "Commit Message: " << commit->getMessage();
	}
	refreshFileTable();
	refreshHistoryTab();
}


// Stages a file that has been selected by clicking on it in the file table
void MainWindow::on_stageSelected_clicked() {
	qDebug() << "Stage All button clicked.";
	if(!selectedFile.isEmpty()) m_repoManager->stageFile(selectedFile.toStdString());
	refreshFileTable();
}

// Stages all files, backend checks if the file needs to be staged first
void MainWindow::on_stageAll_clicked()
{
	qDebug() << "Stage All button clicked.";
	m_repoManager->stageAllFiles();
	refreshFileTable();
}


// Trigged when a cell is clicked in the file table
// Sets the global selectedFile to be the file name at the row clicked
void MainWindow::on_fileTable_cellClicked(int row, int column)
{
	QString fileName = ui.fileTable->item(row, 0)->text();
	selectedFile = fileName; // set the currently selected file
	qDebug() << "Selected File: " << selectedFile;
	refreshDiffTab();
}

// Exit the program, clicking this or the x will save the program automatically because of the logic defined in main.cpp
void MainWindow::on_actionExit_2_triggered()
{
	qDebug() << "Exit menu action triggered.";
	this->close(); // This triggers the window's close event
}