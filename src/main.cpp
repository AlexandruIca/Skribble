#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app{ argc, argv };
    QQmlApplicationEngine engine{};

    engine.addImportPath("qrc:/qml");
    engine.load(QUrl{ QStringLiteral("qrc:/qml/main.qml") });

    return QGuiApplication::exec();
}
