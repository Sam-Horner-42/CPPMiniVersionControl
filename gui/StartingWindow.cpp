#include "MainWindow.h"
#include "StartingWindow.h"
#include "InitRepoDialog.h"
#include "SelectRepoDialog.h"

StartingWindow::StartingWindow(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

StartingWindow::~StartingWindow(){}

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
			emit repoSelected(name, path, true); // true = new repository
			accept(); // Close the StartingWindow so main.cpp can proceed
		}
	}
    
}

void StartingWindow::on_selectRepo_clicked()
{
	SelectRepoDialog selectDialog(this);

	if (selectDialog.exec() == QDialog::Accepted)
	{
		QString name = selectDialog.getSelectedRepoName();
		QString path = selectDialog.getSelectedRepoPath();

		if (!name.isEmpty()) {
			emit repoSelected(name, path, false); // false = existing repository
			accept();
		}
	}
    
}



