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

int main(int argc, char* argv[]) {
	// Initializes the application using QT
    QApplication app(argc, argv);
	RepositoryManager repoManager;
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