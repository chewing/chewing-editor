import QtQuick 2.0
import QtQuick.Layouts 1.1

GridLayout {
    columns: 2

    width: 800
    height: 600

    Text {
        id: filter
        text: qsTr("Filter")
        font.pixelSize: 12
    }

    TextEdit {
        id: userphraseFilter
        font.pixelSize: 12
    }
}
