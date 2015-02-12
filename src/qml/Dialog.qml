import QtQuick 2.0

Item {
    id: dialog

    clip: true

    property alias loader: componentLoader

    property alias acceptButtonText: acceptButton.text
    property alias dismissButtonText: dismissButton.text
    property alias currentComponent: componentLoader.item

    signal accepted()
    signal dismissed()

    property bool isShown: loader.source != ''

    Behavior on height { NumberAnimation { duration: 200 } }

    Loader {
        id: componentLoader
        anchors.fill: parent
        sourceComponent: parent.delegate
    }


    Button {
        id: acceptButton
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: parent.width/2

        visible: text !== '' && isShown
        onClicked: {
            loader.source = ''
            dialog.accepted()
        }

        textColor: 'black'
        border.color: 'black'
        glowColor: 'darkgrey'
    }

    Button {
        id: dismissButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: parent.width/2

        visible: text !== '' && isShown
        onClicked: {
            loader.source = ''
            dialog.dismissed()
        }

        textColor: 'black'
        border.color: 'black'
        glowColor: 'darkgrey'
    }
}
