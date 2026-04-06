// InitRepoDialog.cpp
#pragma once
#include "InitRepoDialog.h"
#include "MainWindow.h"
#include <QDebug>
#include <QMessageBox>

InitRepoDialog::InitRepoDialog(QWidget* parent) : 
    QDialog(parent)
{
    ui.setupUi(this);
}

InitRepoDialog::~InitRepoDialog() {}

QString InitRepoDialog::getRepoName() const
{
    return ui.repoNameInput->text();
}

QString InitRepoDialog::getRepoPath() const
{
    return ui.repoPathInput->text();
}

void InitRepoDialog::on_okButton_clicked()
{
	QString name = ui.repoNameInput->text();
	QString path = ui.repoPathInput->text();

	// Validate Name
	if (name.trimmed().isEmpty()) {
		QMessageBox::warning(this, "Invalid Name", "Repository name cannot be empty.");
		ui.repoNameInput->clear();   // Reset the field
		ui.repoNameInput->setFocus(); // Put the text cursor back in this box
		return; // Stop execution here
	}

	// Validate Path
	QDir dir(path);
	if (path.trimmed().isEmpty() || !dir.isAbsolute()) {
		QMessageBox::warning(this, "Invalid Path", "Please provide a valid, absolute directory path.");
		ui.repoPathInput->clear();    // Reset the field
		ui.repoPathInput->setFocus(); // Put the text cursor back in this box
		return; // Stop execution here
	}

	// The name and path are both valid
	qDebug() << "Path:" << path;
	qDebug() << "Name:" << name;
	accept(); // Manually close the dialog and return QDialog::Accepted
}

// This opens file explorer so the user can select a path
void InitRepoDialog::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Repository Location");
    if (!dir.isEmpty())
        ui.repoPathInput->setText(dir);
}