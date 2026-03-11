#include <QApplication>
#include <QLabel>
#include <QWidget>

int main(int argc, char* argv[]) {
    // Initialize the application
    QApplication app(argc, argv);

    // Create a basic window/widget
    QWidget window;
    window.setFixedSize(300, 150);
    window.setWindowTitle("Qt Test - MiniVersionControl");

    // Add a simple label
    QLabel* label = new QLabel("Qt is working!", &window);
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(0, 0, 300, 150);

    // Show the window
    window.show();

    // Start the event loop
    return app.exec();
}