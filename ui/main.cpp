//
// Created by Hemal Patel on 1/4/26.
//
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Use Fusion style for consistent cross-platform look and customization support
    QQuickStyle::setStyle("Fusion");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/SmartFileOrganizer/qml/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}