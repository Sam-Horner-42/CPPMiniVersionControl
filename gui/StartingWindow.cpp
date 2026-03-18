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
    InitRepoDialog initDialog(this);
    

    if (initDialog.exec() == QDialog::Accepted) // stay on this screen until user confirms/cancels
    {
        QString name = initDialog.getRepoName();
        QString path = initDialog.getRepoPath();

        if (!name.isEmpty() && !path.isEmpty())
            emit repoSelected(name, path);
    }
    
}

void StartingWindow::on_selectRepo_clicked()
{
    SelectRepoDialog selectDialog(this);

    if (selectDialog.exec() == QDialog::Accepted) {
        
    }
    
}



