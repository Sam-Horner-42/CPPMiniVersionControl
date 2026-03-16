#include <QApplication>
#include <QLabel>
#include <QWidget>

#include "../gui/MainWindow.h"
#include "../gui/SelectRepo.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SelectRepo select;
    MainWindow* window = nullptr;

    QObject::connect(&select, &SelectRepo::repoSelected,
        [&](const QString& name, const QString& path) {
            select.hide();
            window = new MainWindow();
            window->setRepoContext(name, path); // pass to your backend logic
            window->show();
        });

    select.show();
    return app.exec();
}