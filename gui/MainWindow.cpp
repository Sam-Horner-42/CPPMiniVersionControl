#include "MainWindow.h"

namespace Ui {
    class MainWindow;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	setupTabWidgets();
}

MainWindow::~MainWindow()
{}


//void MainWindow::setRepoContext(Repository* repo, const QString& name, const QString& path)
void MainWindow::setRepoContext(const QString& name, const QString& path)
{
	//this->currentRepo = repo; // connects the repository pointer in main to this window

	// Populate elements using the currentRepo now that is defined for mainWindow.cpp
	//if (currentRepo != nullptr) {
	//	this->setWindowTitle(QString("MiniVersionControl - " + QString::fromStdString(currentRepo->getRepoName())));
	//}
	this->setWindowTitle(QString("MiniVersionControl - " + name));

	// Clear and populate history
	historyList->clear();
	historyList->addItem("2026-04-06: Initial Commit - Sam Horner");
	historyList->addItem("2026-04-06: Added UI Framework");
}

void MainWindow::setupTabWidgets() {
	// build history tab
	QVBoxLayout* historyLayout = new QVBoxLayout(ui.history);
	historyList = new QListWidget(ui.history);
	historyLayout->addWidget(historyList);
	ui.history->setLayout(historyLayout);

	// Build diff tab
	QVBoxLayout* diffLayout = new QVBoxLayout(ui.diffTab);
	diffView = new QTextEdit(ui.diffTab);
	diffView->setReadOnly(true);

	// Setting a mono font for code/diffs
	QFont monoFont("Courier New", 10);
	diffView->setFont(monoFont);
	diffLayout->addWidget(diffView);
	ui.diffTab->setLayout(diffLayout);

	// Build Analytics tab
	QVBoxLayout* analyticsLayout = new QVBoxLayout(ui.analytics);
	statsLabel = new QLabel("Select a repository to view analytics.", ui.analytics);
	statsLabel->setAlignment(Qt::AlignCenter);
	analyticsLayout->addWidget(statsLabel);
	ui.analytics->setLayout(analyticsLayout);
}

//void MainWindow::refreshFileTable(const std::vector<TrackedFile*>& allFiles) {
//	ui.fileTable->setRowCount(0); // Clear existing rows
//
//	for (TrackedFile* file : allFiles) {
//		int row = ui.fileTable->rowCount();
//		ui.fileTable->insertRow(row);
//
//		// Get the info vector
//		std::vector<std::string> info = file->displayFileInfo();
//
//		// info[0] is "File: name", info[2] is "Status: status"
//		// Strip the prefix for the table cells
//		QString fileName = QString::fromStdString(info[0]).remove("File: ");
//		QString status = QString::fromStdString(info[2]).remove("Status: ");
//
//		ui.fileTable->setItem(row, 0, new QTableWidgetItem(fileName));
//		ui.fileTable->setItem(row, 1, new QTableWidgetItem(status));
//
//		// Color code based on status
//		if (status == "Modified")
//			ui.fileTable->item(row, 1)->setForeground(Qt::yellow);
//	}
//}

//void MainWindow::refreshHistoryTab(Repository* repo) {
//	// Clear the UI list to prevent duplicates
//	ui.historyList->clear();
//
//	// Fetch the vector from the backend
//	std::vector<std::string> history = repo->getCommitHistory();
//
//	// Loop through the vector and add to the QListWidget
//	for (const std::string& entry : history) {
//		QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(entry));
//
//		ui.historyList->addItem(item);
//	}
//
//	// Scroll to the bottom so the newest commit is visible
//	ui.historyList->scrollToBottom();
//}

void MainWindow::on_commitStaged_clicked()
{
	QString commitMessage = ui.commitInput->toPlainText();
	qDebug() << "Commit Staged button clicked. Message:" << commitMessage;
	// if (!commitMessage.isEmpty() && currentRepo) {
	//     currentRepo->commit(commitMessage.toStdString());
	//     ui.commitInput->clear();
	//     // refreshTable();
	// }
}

void MainWindow::on_discardChanges_clicked()
{
	qDebug() << "Discard Changes button clicked.";
	// Backend hook to discard unstaged changes
}

void MainWindow::on_stageAll_clicked()
{
	qDebug() << "Stage All button clicked.";

	// Backend hook to stage all modified/untracked files
}

// The table of files
void MainWindow::on_fileTable_cellClicked(int row, int column)
{
	QString fileName = ui.fileTable->item(row, 0)->text();

	// Update the Diff View (Mock logic)
	// diffView->setPlainText(diffEngine->displayDiff()); // Assuming this gets a String of the whole diff
	diffView->setPlainText("--- a/" + fileName + "\n+++ b/" + fileName +
		"\n@@ -1,1 +1,2 @@\n- Old Line\n+ New Line added by Sam");

	// Switch to the Diff Tab
	ui.infoTab->setCurrentWidget(ui.diffTab);
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
void MainWindow::on_actionInitialize_Local_Repository_triggered()
{
	qDebug() << "Initialize Local Repository action triggered.";
}

void MainWindow::on_actionOpen_Repository_2_triggered()
{
	qDebug() << "Open Repository action triggered.";
}

void MainWindow::on_actionClone_Repository_2_triggered()
{
	qDebug() << "Clone Repository action triggered.";
}

void MainWindow::on_actionStage_Files_triggered()
{
	qDebug() << "Stage Files action triggered.";
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

