#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "canvas.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app{ argc, argv };
    QQmlApplicationEngine engine{};

    qmlRegisterType<sk::Canvas>("Backend.Canvas", 1, 0, "SkCanvas");

    engine.addImportPath("qrc:/qml");
    engine.load(QUrl{ QStringLiteral("qrc:/qml/main.qml") });

    return QGuiApplication::exec();
}
