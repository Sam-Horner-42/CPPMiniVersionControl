#include "MainWindow.h"
#include "StartingWindow.h"
#include "InitRepoDialog.h"

StartingWindow::StartingWindow(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

StartingWindow::~StartingWindow(){}

// called automatically — no connect() required
void StartingWindow::on_initRepo_clicked()
{
    InitRepoDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) // blocks until user confirms/cancels
    {
        QString name = dialog.getRepoName();
        QString path = dialog.getRepoPath();

        if (!name.isEmpty() && !path.isEmpty())
            emit repoSelected(name, path);
    }
    
}



