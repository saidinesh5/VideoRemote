import QtQuick 2.0

Rectangle {
    id: lineEdit

    property string displayText: ''
    property string editText: displayText

    property alias textColor: editBox.color
    property alias maximumLength: editBox.maximumLength

    signal textAccepted( string text );

    border.color: 'black'
    color: 'transparent'

    clip:true

    height: 80
    width: 100

    function acceptText(){
        lineEdit.textAccepted(editBox.text)
        editBox.focus = false
        Qt.inputMethod.hide()
    }

    TextInput{
        id: editBox
        anchors.fill: parent

        color: 'white'

        horizontalAlignment: TextInput.AlignHCenter
        verticalAlignment: TextInput.AlignVCenter
        inputMethodHints: Qt.ImhNoPredictiveText

        text: lineEdit.displayText
        onAccepted: acceptText()
    }

    state : editBox.focus? 'editing' : ''

    states : [
        State {
            name: "editing"
            PropertyChanges {
                target: editBox
                text: editText
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "editing"
            ScriptAction {
                script: editBox.cursorPosition = editText.length
            }
        },
        Transition {
            from: "editing"
            to: ""
            ScriptAction {
                script: acceptText()
            }
        }
    ]
}
