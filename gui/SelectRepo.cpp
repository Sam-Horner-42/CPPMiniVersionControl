#include "MainWindow.h"
#include "SelectRepo.h"
#include "InitRepoDialog.h"

SelectRepo::SelectRepo(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

SelectRepo::~SelectRepo(){}

// called automatically — no connect() required
void SelectRepo::on_initRepo_clicked()
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



