// InitRepoDialog.cpp
#include "InitRepoDialog.h"
#include "MainWindow.h"
#include <QDebug>

InitRepoDialog::InitRepoDialog(QWidget* parent) : 
    QDialog(parent)
{
    ui.setupUi(this);
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
    qDebug() << "Path:" << ui.repoPathInput->text();;
    qDebug() << "Name" << ui.repoNameInput->text();;
    accept(); // closes dialog, returns QDialog::Accepted
}

void InitRepoDialog::on_browseButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Repo Location");
    if (!dir.isEmpty())
        ui.repoPathInput->setText(dir);
}