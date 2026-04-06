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
//#include "../includes/Repository.h"
//#include "../includes/RepositoryManager.h"

int main(int argc, char* argv[]) {
	// Initializes the application using QT
    QApplication app(argc, argv);

    StartingWindow startWindow;
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
		
		//if (isNewRepo) {
		//	// Call the init function 
		//	if (!currentRepo.initRepository(repoName.toStdString(), repoPath.toStdString())) {
		//		QMessageBox::critical(nullptr, "Error", "Failed to initialize the repository on disk.");
		//		return -1; // Exit app if it fails
		//	}
		//} else {
			// Call the load repository function
			//if (!currentRepo.loadRepository(repoName.toStdString(), repoPath.toStdString())) {
			//     QMessageBox::critical(nullptr, "Error", "Failed to load the repository from disk.");
			//     return -1;
			//}
		//}
		

		MainWindow w;
		// Pass a pointer of the backend object to the MainWindow

		//w.setRepoContext(&currentRepo, repoName, repoPath); 
		w.setRepoContext(repoName, repoPath);

		w.show();
		return app.exec();
    }

    // If the user simply closed the starting window, the app exits cleanly
    return 0;
}