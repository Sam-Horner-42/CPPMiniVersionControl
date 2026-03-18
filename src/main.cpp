#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QDebug>

#include "../gui/MainWindow.h"
#include "../gui/StartingWindow.h"
#include "../gui/InitRepoDialog.h"
#include "../gui/SelectRepoDialog.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    StartingWindow startWindow;
    QString repoName;
    QString repoPath;

    // Use a lambda to capture the emitted signal data before the window closes
    QObject::connect(&startWindow, &StartingWindow::repoSelected,
        [&](const QString& name, const QString& path) {
            repoName = name;
            repoPath = path;
        });

    // exec() blocks until the window is closed. 
    // We only launch MainWindow if the StartingWindow was accepted (not canceled)
    if (startWindow.exec() == QDialog::Accepted)
    {
        // Repo name should be passed forward to here when the dialog window is accepted
        qDebug() << "Repo Name: " << repoName << " Repo Path: " << repoPath;
        MainWindow w;
        w.setRepoContext(repoName, repoPath); // Pass the captured data
        w.show();
        return app.exec(); // Start the main event loop
    }

    // If the user simply closed the starting window, the app exits cleanly
    return 0;
}