//
// Created by Hemal Patel on 1/4/26.
//
#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Smart File Organizer");
    window.resize(800, 600);

    QWidget *centralWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *label = new QLabel("Smart File Organizer");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    window.setCentralWidget(centralWidget);
    window.show();

    return app.exec();
}