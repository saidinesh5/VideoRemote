import QtQuick 2.0

Rectangle {
    id: button
    width: textElement.width + 4*textElement.width/textElement.text.length
    height: textElement.height*3

    color: 'transparent'
    border.color: icon == ''? 'grey' : 'transparent'

    clip: true

    property alias text : textElement.text
    property alias textColor: textElement.color
    property alias glowColor: glow.color
    property alias icon : image.source

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
        color: icon == ''? 'white' : 'transparent'

        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    Image {
        id: image
        anchors.centerIn: parent
        width: height
        height: mouseArea.pressed? parent.height : parent.height*0.75
    }

    Text {
        id: textElement
        anchors.centerIn: parent
        color: 'white'
        font.bold: true
        visible: icon == ''
        text: 'A'
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
    }
}

