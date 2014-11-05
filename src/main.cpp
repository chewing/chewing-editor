/*
 * chewing-editor: Chewing userphrase editor
 * Copyright (C) 2014 Chewing Development Team

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"

#include <QApplication>
#include <QDebug>
#include <QLibraryInfo>
#include <QTranslator>

#include "ChewingEditor.h"

void messageHandlerHelper(QtMsgType type, const QMessageLogContext& context, const QString& message, QtMsgType level)
{
    if (type < level) {
        return;
    }

    auto msg = message.toUtf8();
    auto file = QFileInfo{context.file}.fileName().toUtf8();

    switch(type) {
    case QtDebugMsg:
        fprintf(stdout, "Debug: %s (%s %s:%d)\n", msg.constData(), context.function, file.constData(), context.line);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s %s:%d)\n", msg.constData(), context.function, file.constData(), context.line);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s %s:%d)\n", msg.constData(), context.function, file.constData(), context.line);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s %s:%d)\n", msg.constData(), context.function, file.constData(), context.line);
        abort();
        break;
    default:
        break;
    }
}

void emptyMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    messageHandlerHelper(type, context, message, QtWarningMsg);
}

void debugMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    messageHandlerHelper(type, context, message, QtDebugMsg);
}

void loadTranslation(QApplication &app, QTranslator &qtTranslator, QTranslator &chewingTranslator)
{
    QString qtFileName{"qt_" + QLocale::system().name()};
    QString qtDirectory{QLibraryInfo::location(QLibraryInfo::TranslationsPath)};

    bool qtLoaded = qtTranslator.load(qtFileName, qtDirectory);
    qDebug() << "Load" << qtFileName << qtDirectory << qtLoaded;

    app.installTranslator(&qtTranslator);

    QString chewingFileName{PROJECT_NAME "_" + QLocale::system().name()};
    QString chewingDirectory{TRANSLATION_PATH};

    bool chewingLoaded = chewingTranslator.load(chewingFileName, chewingDirectory);
    qDebug() << "Load" << chewingFileName << chewingDirectory << chewingLoaded;

    app.installTranslator(&chewingTranslator);
}

void printVersion()
{
    QString msg{QCoreApplication::translate("main", "%1\n")
        .arg(PROJECT_VERSION)};

    printf("%s", msg.toUtf8().constData());
}

void printArg(const QString &arg, const QString &description)
{
    printf("%s\n\t%s\n\n", arg.toUtf8().constData(), description.toUtf8().constData());
}

void printHelp(const QString &name)
{
    QString description{QCoreApplication::translate("main",
        "chewing-editor is a cross platform chewing user phrase editor written "
        "in Qt5. It provides a user friendly GUI to manage user phrase. With it,"
        " user can customize user phrase to enhance chewing input performance.")};

    printf("%s\n\n", description.toUtf8().constData());

    printArg(
        QCoreApplication::translate("main", "-d, --debug"),
        QCoreApplication::translate("main", "Enable debug message."));

    printArg(
        QCoreApplication::translate("main", "-v, --version"),
        QCoreApplication::translate("main", "Print program version."));

    printArg(
        QCoreApplication::translate("main", "-h, --help"),
        QCoreApplication::translate("main", "Print help message."));
}

void printUnknownArgs(const QString &unknown)
{
    printf("Unknown options: %s\n", unknown.toUtf8().constData());
}

void readArgument(QApplication &app)
{
    auto args = QCoreApplication::arguments();
    auto name = QFileInfo{args.at(0)}.fileName();

    for (int i = 1; i < args.size(); ++i) {
        auto arg = args.at(i);

        if (arg.compare("-d") == 0 || arg.compare("--debug") == 0) {
            qInstallMessageHandler(debugMessageHandler);

        } else if (arg.compare("-v") == 0 || arg.compare("--version") == 0) {
            printVersion();
            exit(0);

        } else if (arg.compare("-h") == 0 || arg.compare("--help") == 0) {
            printHelp(name);
            exit(0);

        } else {
            printUnknownArgs(arg);
            printHelp(name);
            exit(1);
        }
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(emptyMessageHandler);

    QApplication app{argc, argv};

    QTranslator qtTranslator;
    QTranslator chewingTranslator;
    loadTranslation(app, qtTranslator, chewingTranslator);

    readArgument(app);

    ChewingEditor w;
    w.show();

    return app.exec();
}
