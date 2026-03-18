// SelectRepoDialog.cpp
#include "SelectRepoDialog.h"
#include "MainWindow.h"

SelectRepoDialog::SelectRepoDialog(QWidget* parent) :
    QDialog(parent),
    model(new QStandardItemModel(this)) // Initialize the tree model
{
    ui.setupUi(this);

    // 1. Set headers for the tree
    model->setHorizontalHeaderLabels({ "Repository Information" });

    // 2. Mock Data
    repoMap.insert("Repo 1", "C:/Users/Sam/Documents/Repo1");
    repoMap.insert("Repo 2", "C:/Users/Sam/Documents/Repo2");
    repoMap.insert("Repo 3", "C:/Users/Sam/Documents/Repo3");

    for (auto it = repoMap.begin(); it != repoMap.end(); ++it) {
        // Create the Top-Level Item (The Repo Name)
        QStandardItem* parentItem = new QStandardItem(it.key());

        // Add child rows (the "down a level" data)
        parentItem->appendRow(new QStandardItem("Path: " + it.value()));
        parentItem->appendRow(new QStandardItem("Last Commit: a7b2c3d"));
        parentItem->appendRow(new QStandardItem("Date: 2023-10-27"));

        // Add the parent to the root of the model
        model->appendRow(parentItem);
    }

    // 3. Bind to the TreeView
    ui.repoList->setModel(model);
    ui.repoList->expandAll();
}

SelectRepoDialog::~SelectRepoDialog() {}

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
    // Now that we have the Parent Name, we just look up the path in our Map
    QString name = getSelectedRepoName();
    return repoMap.value(name);
}

void SelectRepoDialog::on_okButton_clicked()
{
    // Simply accept the dialog. 
    // StartingWindow will see this, emit the signal, and close itself.
    accept();
    // What happens when okay button is pressed
    // Need to switch to main window and load repo information

}