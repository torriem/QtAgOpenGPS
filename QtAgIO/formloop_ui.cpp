#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QScreen>
#include <QGuiApplication>
#include <QQmlEngine>
#include "qmlutil.h"
#include "formloop.h"

void FormLoop::setupGUI()
{
    //Load the QML UI and display it in the main area of the GUI
    setProperty("title", "QtAgIO");

    //load the QML into a view
    engine.rootContext()->setContextProperty("screenPixelDensity",QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    engine.rootContext()->setContextProperty("mainForm", this);
    engine.rootContext()->setContextProperty("prefix", ":");
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));



    if (engine.rootObjects().isEmpty())
    {
        qDebug() << "Failed to load QML file";
        return;
    }
    else
    {
        qDebug() << "Displaying QML File";
    }
}
