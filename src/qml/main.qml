import QtQuick 2.0
import com.video.remote 1.0

Rectangle {
    width: 480
    height: 800
    visible: true
    color: 'black'

    ScreenConnection {
        id: screen
    }

    SequencesView {
        id: sequencesView
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: currentSequenceId >= 0? contentHeight : parent.height

        Behavior on height { NumberAnimation{ duration: 200 } }

        address: screen.address
        port: screen.port

        onAddressChanged: screen.address = address
        onPortChanged: screen.port = port
        onCurrentSequenceIdChanged: screen.currentSequenceId = currentSequenceId

        model: screen

        Connections {
            target: screen
            onAddressChanged: if(sequencesView.address !== screen.address)
                                  sequencesView.address = screen.address

            onPortChanged: if(sequencesView.port !== screen.port)
                               sequencesView.port = screen.port

            onCurrentSequenceIdChanged: if(screen.currentSequenceId != sequencesView.currentSequenceId)
                                            sequencesView.currentSequenceId = screen.currentSequenceId

        }
    }

    Sequencer {
        id: sequencer

        anchors.top: sequencesView.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentSequence: screen.currentCommandSequence? screen.currentCommandSequence : null

        running: screen.running

        onTogglePlayPauseClicked: screen.running? screen.pause() : screen.start()
        onResetClicked: screen.reset()
    }
}
