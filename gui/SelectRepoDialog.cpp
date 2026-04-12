// SelectRepoDialog.cpp
#include "SelectRepoDialog.h"
#include "MainWindow.h"
#include "../includes/nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <QDebug> // Added missing QDebug include

using json = nlohmann::json;
namespace fs = std::filesystem;

SelectRepoDialog::SelectRepoDialog(RepositoryManager* repoManager, QWidget* parent) :
	QDialog(parent),
	model(new QStandardItemModel(this)) // Initialize the tree model
{
	ui.setupUi(this);

	// Set headers for the tree
	model->setHorizontalHeaderLabels({ "Repository Information" });
	if (m_repoManager) {
		std::vector<RepositoryManager::Project> projects = m_repoManager->getProjectInfo();

		// Loop through the vector and populate the UI
		for (const auto& project : projects) {
			QStandardItem* parentItem = new QStandardItem(QString::fromStdString(project.name));
			parentItem->appendRow(new QStandardItem("ID: " + QString::fromStdString(project.id)));
			parentItem->appendRow(new QStandardItem("Path: " + QString::fromStdString(project.filePath)));
			model->appendRow(parentItem);
		}
	}

	ui.repoList->setModel(model);
}

SelectRepoDialog::~SelectRepoDialog() {}

// Getters
QString SelectRepoDialog::getSelectedRepoName() const
{
	QModelIndex index = ui.repoList->currentIndex();
	if (!index.isValid()) return QString();

	// If the user clicked a child, index.parent() will be valid.
	// We want the text of the top-most parent.
	if (index.parent().isValid()) {
		return index.parent().data(Qt::DisplayRole).toString();
	}

	return index.data(Qt::DisplayRole).toString();
}

QString SelectRepoDialog::getSelectedRepoPath() const
{
	QString name = getSelectedRepoName();

	// Check if it exists, then return the specific .path string from the struct
	if (repoMap.contains(name)) {
		return repoMap.value(name).path;
	}

	return QString(); // Return empty string if not found
}

void SelectRepoDialog::on_okButton_clicked()
{
	// Simply accept the dialog. 
	// StartingWindow will see this, emit the signal, and close itself.
	// Need to setup logic to refuse access if name is wrong
	accept();
}