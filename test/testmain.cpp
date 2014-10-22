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
#include <QApplication>
#include <QFileInfo>

#include "gtest/gtest.h"

bool verbose = false;

void debugMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    auto msg = message.toUtf8();
    auto file = QFileInfo{context.file}.fileName().toUtf8();

    if (!verbose) {
        return;
    }

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    qInstallMessageHandler(debugMessageHandler);

    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        }

        if (strcmp(argv[i], "-s") == 0) {
            verbose = false;
        }
    }

    return RUN_ALL_TESTS();
}
