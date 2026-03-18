// SelectRepoDialog.cpp
#include "SelectRepoDialog.h"
#include "MainWindow.h"
#include <QDebug>

SelectRepoDialog::SelectRepoDialog(QWidget* parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}

SelectRepoDialog::~SelectRepoDialog() {}


void SelectRepoDialog::on_okButton_clicked()
{
    // What happens when okay button is pressed
    // Need to switch to main window and load repo information
}