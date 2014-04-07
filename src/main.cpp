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
#include <QLibraryInfo>
#include <QTranslator>

void loadTranslation(QApplication &app)
{
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    QTranslator chewingTranslator;
    chewingTranslator.load("chewing-editor_" + QLocale::system().name(), TRANSLATION_PATH);
    app.installTranslator(&chewingTranslator);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loadTranslation(app);

    ChewingEditor w;
    w.show();

    return app.exec();
}
