#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringList>

#include "canvas.hpp"
#include "network_config.hpp"

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app{ argc, argv };
    QGuiApplication::setApplicationName("Skribble");
    QQmlApplicationEngine engine{};

    QCommandLineParser parser{};
    parser.setApplicationDescription("What mode to run Skribble in?");
    parser.addHelpOption();

    QCommandLineOption serverMode{ QStringList{} << "s"
                                                 << "server-mode",
                                   QGuiApplication::translate(
                                       "main", "Start in server mode") };

    parser.addOption(serverMode);

    QCommandLineOption clientMode{
        QStringList{} << "c"
                      << "client-mode",
        QGuiApplication::translate(
            "main", "Start in client mode AKA connect to <server>"),
        QGuiApplication::translate("main", "server")
    };

    parser.addOption(clientMode);
    parser.process(app);

    if(parser.isSet(serverMode)) {
        qDebug() << "Server mode!";
        sk::networkMode = sk::NetworkModes::SERVER;
    }
    else if(parser.isSet(clientMode)) {
        qDebug() << "Client mode: " << parser.value(clientMode);
        sk::networkMode = sk::NetworkModes::CLIENT;
    }
    else {
        sk::networkMode = sk::NetworkModes::SINGLE_USER;
    }

    qmlRegisterType<sk::Canvas>("Backend.Canvas", 1, 0, "SkCanvas");

    engine.addImportPath("qrc:/qml");
    engine.load(QUrl{ QStringLiteral("qrc:/qml/main.qml") });

    return QGuiApplication::exec();
}
