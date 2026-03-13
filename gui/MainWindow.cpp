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

