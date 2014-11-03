import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

GridLayout {
    columns: 2

    width: 800
    height: 600

    Label {
        text: qsTr("Filter")
        font.pixelSize: 12
    }

    TextEdit {
        id: userphraseFilter
        text: qsTr("userphraseFilter")
        font.pixelSize: 12
    }

    Label {
        Layout.columnSpan: 2
        id: notification
        text: qsTr("Notification")
        font.pixelSize: 12
    }
}
