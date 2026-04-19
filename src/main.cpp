#pragma once
#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QDebug>
#include <QMessageBox>

#include "../gui/MainWindow.h"
#include "../gui/StartingWindow.h"
#include "../gui/InitRepoDialog.h"
#include "../gui/SelectRepoDialog.h"
#include "../includes/RepositoryManager.h"

namespace fs = std::filesystem;
using json = nlohmann::json;
// Function to guarantee the data directory and JSON file exist
void initializeApplicationData() {
	fs::path dataDir = "data";
	fs::path jsonFile = dataDir / "dataHandler.json";

	// Check if the "data" directory exists, if not, create it.
	if (!fs::exists(dataDir)) {
		fs::create_directory(dataDir);
		qDebug() << "First run detected: Created missing 'data' directory.";
	}

	// Check if "dataHandler.json" exists inside the folder.
	if (!fs::exists(jsonFile)) {
		// Create the base JSON structure so your load functions have valid JSON to read
		json initialStructure = {
			{"projects", json::array()} // Sets up {"projects": []}
		};

		std::ofstream outFile(jsonFile);
		if (outFile.is_open()) {
			// Write the JSON to the file with a 4-space indent for readability
			outFile << initialStructure.dump(4);
			outFile.close();
			qDebug() << "Created and initialized 'dataHandler.json'.";
		}
		else {
			qDebug() << "Critical Error: Could not create 'dataHandler.json'.";
		}
	}
}
int main(int argc, char* argv[]) {
	// Initializes the application using QT
    QApplication app(argc, argv);
	RepositoryManager repoManager;
	initializeApplicationData(); // generates the directory for storing repo data on the first run of the .exe
    StartingWindow startWindow(&repoManager);
    QString repoName;
    QString repoPath;
	bool isNewRepo = false; // default to false

	//Repository currentRepo; // this is the main repository object, it will be either initialized or loaded with data

    // Use a lambda to capture the emitted signal data before the window closes
    QObject::connect(&startWindow, &StartingWindow::repoSelected,
        [&](const QString& name, const QString& path, bool isNew ) {
            repoName = name;
            repoPath = path;
			isNewRepo = isNew; // this boolean decides whether an existing repo is loaded or a new repo needs to be initialized

        });

    // exec() blocks until the window is closed. 
    // We only launch MainWindow if the StartingWindow was accepted (not canceled)
	// This also ensures valid input data before showing the mainWindow
    if (startWindow.exec() == QDialog::Accepted)
    {
		//QDebug() << "In Main Path: " << repoPath;
		
		// Pass a pointer of the backend object to the MainWindow
		if (isNewRepo) {
			qDebug() << "New Repo Path: " << repoPath;
			repoManager.createRepository(repoName.toStdString(), repoPath.toStdString());
		}
		else {
			repoManager.loadRepository(repoName.toStdString());
		}
		MainWindow w(&repoManager);
		w.setRepoContext(repoName, repoPath);

		
		
		w.show();
		int result = app.exec(); // The program stays here until the window is closed

		// The event loop ended, the user closed the program, we save the repository information
		if (!repoName.isEmpty()) {
			qDebug() << "The program is closing";
			repoManager.saveRepository(repoName.toStdString(), repoPath.toStdString());
		}
		return result;
    }

    // If the user simply closed the starting window, the app exits cleanly
    return 0;
}

