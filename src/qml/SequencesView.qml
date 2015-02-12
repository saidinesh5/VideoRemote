import QtQuick 2.0

Rectangle {
    id: sequencesView
    width: 480
    height: contentHeight

    color: 'transparent'
    border.color: 'grey'

    property int contentHeight: addressRow.height + sequencesListView.height

    property string address : hostAddress.text
    property int port: hostPort.text

    property alias model: sequencesListView.model
    property alias currentSequenceId: sequencesListView.currentIndex

    property real currentCommand: 0
    property real totalCommands: 1

    property real progress: currentCommand/totalCommands

    Text {
        id: dummyText
        visible: false
        text:'A'
    }

    // This is here instead of main.qml because the address determines what sequences are there
    Item {
        id: addressRow
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: dummyText.height*3

        LineEdit {
            id: host

            height: parent.height
            width: 2*parent.width/3

            displayText: address + ":" + port

            onTextAccepted: {
                var h = text.split(':')[0]
                var p = text.split(':')[1]

                if(sequencesView.address !== h)
                    sequencesView.address = h

                var n = parseInt(p)
                if(n >= 0 && n <= 65536 && sequencesView.port !== n)
                    sequencesView.port = n
            }
        }

        Button {
            id: updateAddress
            width: parent.width/3
            height: parent.height
            anchors.left: host.right
            anchors.right: parent.right

            text: qsTr("Update")

            onClicked: host.acceptText()
        }
    }

    ListView {
        id: sequencesListView

        clip: true

        currentIndex: -1
        anchors.top: addressRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Math.min( count, 3 )*dummyText.height*3

        delegate: Rectangle {

            property bool isCurrent : index === sequencesListView.currentIndex

            width: parent.width
            height: sequenceNameText.height*3
            color: isCurrent? 'orange' : 'lightgrey'

            Rectangle {
                id: progressRectangle
                height: parent.height
                width: totalCommands > 0? progress*parent.width : 0.0
                color: 'lightgreen'
                visible: isCurrent
            }

            Text {
                id: sequenceNameText
                text: sequenceName

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.bold: isCurrent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: sequencesListView.currentIndex = index
            }
        }
    }
}
