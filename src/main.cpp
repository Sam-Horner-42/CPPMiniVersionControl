#include <QApplication>
#include <QLabel>
#include <QWidget>

#include "../gui/MainWindow.h"
#include "../gui/StartingWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    StartingWindow start;
    MainWindow* window = nullptr;

    QObject::connect(&start, &StartingWindow::repoSelected,
        [&](const QString& name, const QString& path) {
            start.hide();
            window = new MainWindow();
            window->setRepoContext(name, path); // pass to your backend logic
            window->show();
        });

    start.show();
    return app.exec();
}