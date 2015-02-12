#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>

#include "screenconnection.h"
#include "commandsequence.h"

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QQuickView view;

    qmlRegisterType<CommandSequence>("com.video.remote", 1, 0, "CommandSequence");
    qmlRegisterType<ScreenConnection>("com.video.remote", 1, 0, "ScreenConnection");


    view.setSource(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
