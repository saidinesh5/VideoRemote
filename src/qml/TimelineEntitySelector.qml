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

    property int margins: title.height

    Text {
        id: title
        anchors.verticalCenter: availableOptionsCombobox.verticalCenter
        anchors.left: parent.left
        anchors.margins: margins
        font.bold: true
        //                font.pointSize: 15
        text: "Add " + entityType
    }

    ComboBox {
        id: availableOptionsCombobox
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: margins
        dataModel: availableOptions
    }

    Text {
        id: startTimeLabel
        anchors.left: parent.left
        anchors.top: title.bottom
        anchors.leftMargin: margins
        anchors.topMargin: 2*margins

        text:'Start time(ms)'
    }

    LineEdit {
        id: startTimeInput
        anchors.verticalCenter: startTimeLabel.verticalCenter
        anchors.right: parent.right
        anchors.margins: margins

        height: 3*margins
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

        text:'End time'
    }

    LineEdit {
        id: endTimeInput
        anchors.verticalCenter: endTimeLabel.verticalCenter
        anchors.right: parent.right
        anchors.margins: margins

        height: 3*margins
        width: availableOptionsCombobox.width
        color: 'white'
        textColor: 'black'

        displayText:  String(endTime)
        onTextAccepted: endTime = parseInt(text)
    }
}
