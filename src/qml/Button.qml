import QtQuick 2.0

Rectangle {
    id: button
    width: textElement.width + 4*textElement.width/textElement.text.length
    height: textElement.height*3

    border.color: 'grey'
    color: 'transparent'

    clip: true

    property alias text : textElement.text
    property alias textColor: textElement.color
    property alias glowColor: glow.color

    signal clicked()

    Rectangle {
        id: glow
        anchors.fill: parent
        opacity: mouseArea.pressed? 1.0 : 0
//        gradient: Gradient {
//            GradientStop { position: 0.0; color: Qt.rgba(0,0,0,0) }
//            GradientStop { position: 0.5; color: Qt.rgba(0,0,1.0,1.0) }
//            GradientStop { position: 1.0; color: Qt.rgba(0,0,0,0) }
//        }
        color: 'white'

        Behavior on opacity { NumberAnimation { duration: 200 } }
    }


    Text {
        id: textElement
        anchors.centerIn: parent
        color: 'white'
        font.bold: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
    }
}

