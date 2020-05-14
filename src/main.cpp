///
/// \file
///
/// \brief Driver for Skribble
///
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QStringList>

#include "canvas.hpp"
#include "network_config.hpp"

///
/// \mainpage Skribble
///
/// \section sec_about What is it?
///
/// Skribble tries to be a collaborative app made with Qt. At most two users
/// can draw on the same canvas. It was made as part of an assignment at our
/// University.
///
/// \section sec_download Download
///
/// There is a binary release available for Windows
/// [here](https://github.com/AlexandruIca/Skribble/releases).
///
/// \section sec_usage Usage
///
/// Starting `./Skribble` gives you the single user mode of the app, a normal
/// painting app as you expect.
///
/// Starting `./Skribble -s` starts the app and the server, which clients can
/// connect to with `./Skribble -c <ip>` where ip is, in theory, one's public
/// IP. In practice, there is no way to connect over the network if the IP
/// is dynamic or for other reasons we don't know. For sure is, to test the app
/// in collaborative mode you can do `./Skribble -s` from one terminal and
/// `./Skribble -c 127.0.0.1` from another terminal which connects to the local
/// host, thus emulating a two user scenario.
///
/// \section sec_building Build
///
/// The straightforward way:
/// ```sh
/// mkdir build && cd build
/// cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/5.x.x/lib/cmake \
///       -DCMAKE_BUILD_TYPE=Release \
///       ..
/// cmake --build .
/// ```
///
/// For development:
/// ```sh
/// # Other than above parameters
/// cmake -DENABLE_SANITIZER_ADDRESS=ON \
///       -DENABLE_SANITIZER_UNDEFINED=ON \
///       -DBUILD_TESTS=ON \
///       ..
/// ```
///
/// Obviously Qt5 needs to be installed.
///
/// To run the tests go in the build folder and run:
/// ```
/// ctest -V
/// ```
///

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
        sk::host_ip = parser.value(clientMode);
    }
    else {
        sk::networkMode = sk::NetworkModes::SINGLE_USER;
    }

    qmlRegisterType<sk::Canvas>("Backend.Canvas", 1, 0, "SkCanvas");

    engine.addImportPath("qrc:/qml");
    engine.load(QUrl{ "qrc:/qml/main.qml" });

    return QGuiApplication::exec();
}
