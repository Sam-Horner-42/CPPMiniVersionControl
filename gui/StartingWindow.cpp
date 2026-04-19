// This is the first window the user sees when the application runs
// This handles constructing the dialog windows for selecting a repo and initializing a new one
// The information is passed through here to the main.cpp so the MainWindow can populate its data correctly
// Author: Sam Horner 040935005

#include "MainWindow.h"
#include "StartingWindow.h"
#include "InitRepoDialog.h"
#include "SelectRepoDialog.h"
#include <QRegularExpression> // Used for input validation

StartingWindow::StartingWindow(RepositoryManager* repoManager, QWidget* parent)
    : QDialog(parent), m_repoManager(repoManager)
{
    ui.setupUi(this);
}

StartingWindow::~StartingWindow(){}

QRegularExpression reSpecChars("[^\\w]"); // This checks for special characters

// called automatically — no connect() required
void StartingWindow::on_initRepo_clicked()
{
	InitRepoDialog initDialog(this); // create the window

	// Okay is pressed
	if (initDialog.exec() == QDialog::Accepted)
	{
		QString name = initDialog.getRepoName();
		QString path = initDialog.getRepoPath();

		if (!name.isEmpty() && !path.isEmpty()) {
			qDebug() << "Repo Path: " << path;
			emit repoSelected(name, path, true); // true = new repository
			accept(); // Close the StartingWindow so main.cpp can proceed
		}
	}
    
}

// This function handles opening the select window so the user can select a preexisting repository
void StartingWindow::on_selectRepo_clicked()
{
	SelectRepoDialog selectDialog(m_repoManager, this);

	if (selectDialog.exec() == QDialog::Accepted)
	{
		QString name = selectDialog.getSelectedRepoName();
		QString path = selectDialog.getSelectedRepoPath();

		if (!name.contains(reSpecChars) && !name.isEmpty() && !(name.size() > 40)) {
			emit repoSelected(name, path, false);
			accept();
		}
	}
    
}



