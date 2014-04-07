/*
 * chewing-editor: Chewing userphrase editor
 * Copyright (C) 2014 ChangZhuo Chen

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

#include "ChewingEditor.h"

#include <QApplication>
#include <QDebug>
#include <QLibraryInfo>
#include <QTranslator>

void emptyMessageHandler(QtMsgType, const QMessageLogContext&, const QString&)
{
}

void debugMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
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

void loadTranslation(QApplication &app)
{
    QTranslator qtTranslator;

    QString qtFileName{"qt_" + QLocale::system().name()};
    QString qtDirectory{QLibraryInfo::location(QLibraryInfo::TranslationsPath)};

    qDebug() << "Load " << qtFileName << qtDirectory;
    qtTranslator.load(qtFileName, qtDirectory);

    app.installTranslator(&qtTranslator);


    QTranslator chewingTranslator;
    QString chewingFileName{"chewing-editor_" + QLocale::system().name()};
    QString chewingDirectory{TRANSLATION_PATH};

    qDebug() << "Load " << chewingFileName << chewingDirectory;
    chewingTranslator.load(chewingFileName, chewingDirectory);

    app.installTranslator(&chewingTranslator);
}

void readArgument(QApplication &app)
{
    foreach (QString arg, QCoreApplication::arguments()) {
        if (arg.compare("-d") == 0) {
            qInstallMessageHandler(debugMessageHandler);
        }
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(emptyMessageHandler);

    QApplication app{argc, argv};

    readArgument(app);
    loadTranslation(app);

    ChewingEditor w;
    w.show();

    return app.exec();
}
