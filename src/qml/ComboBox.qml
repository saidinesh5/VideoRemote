import QtQuick 2.0

Rectangle {
    id: comboBox

    property var dataModel: ["Hello", "Worlds"]
    property bool expanded: false

    property alias currentIndex: listView.currentIndex
    property var currentItem: dataModel[currentIndex]

    enabled: dataModel && dataModel.length > 0

    color: 'grey'
    border.color: 'black'
    border.width: 1

    clip:true

    Text {
        id: dummyText
        text: 'A'
        visible: false
    }
    property int componentHeight: dummyText.height*3

    width: dummyText.width*20
    height: expanded? componentHeight*dataModel.length : componentHeight

    z: expanded? 1 : 0

    Behavior on height { NumberAnimation { duration: 200 } }

    ListView {
        id: listView
        model: comboBox.dataModel

        anchors.fill: parent

        delegate: Rectangle {
            width: comboBox.width
            height: componentHeight
            color: comboBox.expanded && index == listView.currentIndex ? '#007AFF' : 'transparent'

            Text {
                text: modelData
                anchors.left: parent.left
                anchors.leftMargin: dummyText.width
                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 10
            }

            MouseArea {
                anchors.fill: parent
                onClicked:
                {
                    listView.currentIndex = index
                    comboBox.expanded = !comboBox.expanded
                }
            }
        }
    }

    Text
    {
        font.pointSize: 20
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.bottom: parent.bottom
        text: '\u25E2'
        color: '#434447'
        opacity: expanded? 0.0 : 1
        Behavior on opacity { NumberAnimation { duration: 100 } }
    }
}
