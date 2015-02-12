import QtQuick 2.0
import com.video.remote 1.0


Rectangle {
    id: selector
    border.color: 'white'
    border.width: 1

    color: 'lightgrey'

    property Dialog parentDialog
    property CommandSequence currentSequence

    property int entityId : 0
    property string entityType: 'video'
    property var availableOptions: ['video1' , 'video2']

    property alias entityName: availableOptionsCombobox.currentItem
    property int startTime: 0
    property int endTime: 100

    Connections {
        target: parentDialog
        onAccepted: {
            if(startTime < endTime)
                currentSequence.addEntity(entityType, entityId, entityName, startTime, endTime)
        }
    }


    Text {
        id: dummyText
        text: 'A'
        visible: false
    }

    property int margins: dummyText.width

    Text {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: margins*6
        anchors.leftMargin: margins
        font.bold: true

        text: "Add " + entityType
    }

    ComboBox {
        id: availableOptionsCombobox
        anchors.right: parent.right
        anchors.verticalCenter: title.verticalCenter
        anchors.topMargin: margins*6
        anchors.rightMargin: margins
        dataModel: availableOptions
    }

    Text {
        id: startTimeLabel
        anchors.left: parent.left
        anchors.top: title.bottom
        anchors.leftMargin: margins
        anchors.topMargin: 4*margins

        text:'Start time(ms)'
    }

    LineEdit {
        id: startTimeInput
        anchors.verticalCenter: startTimeLabel.verticalCenter
        anchors.right: parent.right
        anchors.margins: margins

        height: dummyText.height*3
        width: availableOptionsCombobox.width
        color: 'white'
        textColor: 'black'

        displayText: String(startTime)

        onTextAccepted: startTime = parseInt(text)
    }


    Text {
        id: endTimeLabel
        anchors.left: parent.left
        anchors.top: startTimeInput.bottom
        anchors.leftMargin: margins
        anchors.topMargin: 2*margins

        text:'End time(ms)'
    }

    LineEdit {
        id: endTimeInput
        anchors.verticalCenter: endTimeLabel.verticalCenter
        anchors.right: parent.right
        anchors.margins: margins

        height: dummyText.height*3
        width: availableOptionsCombobox.width
        color: 'white'
        textColor: 'black'

        displayText:  String(endTime)
        onTextAccepted: endTime = parseInt(text)
    }
}
