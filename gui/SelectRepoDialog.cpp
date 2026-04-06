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

SelectRepoDialog::SelectRepoDialog(QWidget* parent) :
	QDialog(parent),
	model(new QStandardItemModel(this)) // Initialize the tree model
{
	ui.setupUi(this);

	// Set headers for the tree
	model->setHorizontalHeaderLabels({ "Repository Information" });

	// Load data into the map
	loadJsonIntoMap();

	// Load the data from JSON into the map for display
	for (auto it = repoMap.begin(); it != repoMap.end(); ++it) {

		// Get the struct from the map
		const RepoInfo& info = it.value();

		// Create the Top-Level Item (The Repo Name)
		QStandardItem* parentItem = new QStandardItem(info.name);

		// Add child rows (ID and Path)
		parentItem->appendRow(new QStandardItem("ID: " + info.id));
		parentItem->appendRow(new QStandardItem("Path: " + info.path));

		// Add the parent item to the visible model
		model->appendRow(parentItem);
	}

	// Bind to the TreeView 
	ui.repoList->setModel(model);
	ui.repoList->expandAll();
}

SelectRepoDialog::~SelectRepoDialog() {}

// Loads the data from the JSON file to the map
void SelectRepoDialog::loadJsonIntoMap() {
	// Open the file, need to fix absolute path problem, but for now it works
	std::ifstream file("C:/AlgonquinCollege/CST8219/FinalProject/MiniVersionControl/data/dataHandler.json");
	if (!file.is_open()) {
		qDebug() << "Failed to open JSON file.";
		return;
	}

	// Parse the JSON
	json data;
	file >> data;
	file.close();

	// Loop through the "projects" array
	if (data.contains("projects") && data["projects"].is_array()) {
		for (const auto& project : data["projects"]) {

			// Extract the strings (with fallbacks if missing)
			RepoInfo info;
			info.name = QString::fromStdString(project.value("name", ""));
			info.id = QString::fromStdString(project.value("id", ""));
			info.path = QString::fromStdString(project.value("path", ""));

			// Insert into the map
			// The name is the key for now but it could be changed to the id if we prefer
			if (!info.name.isEmpty()) {
				repoMap.insert(info.name, info);
			}
		}
	}
}

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