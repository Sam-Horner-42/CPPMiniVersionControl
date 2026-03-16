#include "MainWindow.h"

namespace Ui {
    class MainWindow;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

MainWindow::~MainWindow()
{}

void MainWindow::setRepoContext(const QString& name, const QString& path)
{
    // This is where the call to backend logic, should go e.g.,
    // repo = Repository(name.toStdString(), path.toStdString());
    // repo.init();
}