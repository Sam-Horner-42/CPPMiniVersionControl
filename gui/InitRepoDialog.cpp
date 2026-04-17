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

	// This handles input validation through regular expressions
	QRegularExpression rx("^[A-Za-z0-9]{1,40}$");
	QValidator* validator = new QRegularExpressionValidator(rx, this);

	ui.repoNameInput->setValidator(validator);
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

	// Validate Name using the attached Validator
	if (!ui.repoNameInput->hasAcceptableInput()) {
		QMessageBox::warning(this, "Invalid Name", "Repository name must be 1-40 alphanumeric characters.");
		ui.repoNameInput->clear();
		ui.repoNameInput->setFocus();
		return;
	}

	// Validate Path
	QDir dir(path);
	if (path.trimmed().isEmpty() || !dir.isAbsolute() || !dir.exists()) {
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
	QString homePath = QDir::homePath(); // Returns the user's home directory path
	QString dir = QFileDialog::getExistingDirectory(this,
		"Select Repository Location",
		homePath);

    if (!dir.isEmpty())
        ui.repoPathInput->setText(dir);
}