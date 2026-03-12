#include <QApplication>
#include <QLabel>
#include <QWidget>

#include "../gui/MainWindow.h"
#include "../gui/SelectRepo.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SelectRepo select;

    select.show();

    // Instance of the main window class
    //MainWindow window;

    // Display the main window
    //window.show();

    return app.exec();
}