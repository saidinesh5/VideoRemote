import QtQuick 2.0
import com.video.remote 1.0

Rectangle {
    id: sequencer

    width: 480
    height: 600

    clip: true

    property bool running

    property CommandSequence currentSequence

    signal togglePlayPauseClicked()
    signal resetClicked()

    TimelineEntityModel {
        id: effectsModel
    }

    TimelineEntityModel {
        id: videosModel
    }

    onCurrentSequenceChanged: {
        //Populate effectsModel and videosModel
        console.log("currentSequenceChanged")
        effectsModel.clear()
        videosModel.clear()
        timelineLength = 0

        var deletedIndices = []

        for(var i = 0; i < currentSequence.count; i++)
        {

            var commandGroup = currentSequence.get(i, CommandSequence.CommandGroup )
            var commandId = currentSequence.get(i, CommandSequence.CommandId )
            var commandName = currentSequence.get(i, CommandSequence.CommandOptions )
            var startTime = currentSequence.get(i, CommandSequence.Time )
            var endTime = -1

            if(deletedIndices.indexOf(commandId) !== -1)
                continue;

            for(var j = i + 1; j < currentSequence.count; j++)
            {
                var currentId = currentSequence.get(j, CommandSequence.CommandId)
                if( deletedIndices.indexOf(j) === -1 && currentId === commandId  &&
                    currentSequence.get(j, CommandSequence.CommandGroup) === commandGroup )
                {
                    endTime = currentSequence.get(j, CommandSequence.Time);
                    deletedIndices.push(commandId)
                    break;
                }
            }

            if(endTime < startTime)
                continue;

            if(commandGroup === 'video') videosModel.addEntity(commandId, commandName, startTime, endTime)
            else effectsModel.addEntity(commandId, commandName, startTime, endTime)
        }
    }

    Connections {
        target: currentSequence
        onVideoAdded: videosModel.addEntity(videoId, videoName, startTime, endTime)
        onEffectAdded: effectsModel.addEntity(effectId, effectName, startTime, endTime)
        onVideoDeleted: videosModel.deleteEntity(videoId)
        onEffectDeleted: effectsModel.deleteEntity(effectId)
    }

    property real timelineScale: 1
    property real timelineOffset: 0
    property real timelineLength: 0

    Item {
        id: timelineHeader
        width: effectsTitle.height*3
        anchors.top: parent.top
        anchors.bottom: controlButtons.top

        Rectangle {
            id: effectsHeader
            anchors.top: parent.top
            height: parent.height/2
            width: parent.width
            color: 'darkgrey'
            Text {
                id: effectsTitle
                rotation: -90
                anchors.centerIn: parent
                text: 'Effects'
            }
        }

        Rectangle {
            id: videosHeader
            anchors.bottom: parent.bottom
            height: parent.height/2
            width: parent.width
            color: 'grey'
            Text {
                id: videosTitle
                rotation: -90
                anchors.centerIn: parent
                text: 'Videos'
            }
        }

        Button {
            id: addEffectButton
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            text: '+'
            color: 'lightgreen'

            onClicked: {
                dialog.loader.setSource( 'TimelineEntitySelector.qml',
                                        {
                                            parentDialog: dialog,
                                            currentSequence: currentSequence,
                                            entityType: 'effect',
                                            entityId: effectsModel.nextId,
                                            availableOptions: currentSequence.availableEffects
                                        } )
            }
        }

        Button {
            id: addVideoButton
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: parent.height/2
            text: '+'
            color: 'lightgreen'
            onClicked: dialog.loader.setSource( 'TimelineEntitySelector.qml',
                                               {
                                                   parentDialog: dialog,
                                                   currentSequence: currentSequence,
                                                   entityType: 'video',
                                                   entityId: videosModel.nextId,
                                                   availableOptions: currentSequence.availableVideos
                                               } )
        }
    }


    Flickable {
        id: timeline
        clip: true
        anchors.top: parent.top
        anchors.left: timelineHeader.right
        anchors.right: parent.right
        height: timelineHeader.height

        interactive: true

        contentX: timelineOffset*timelineScale

        contentWidth: timelineLength*timelineScale
        contentHeight: parent.height
        flickableDirection: Flickable.HorizontalFlick

        Repeater {
            id: effectsRepeater

            model: effectsModel

            delegate: Rectangle {
                property real rightEdge: endTime

                x: startTime*timelineScale
                width: (endTime - startTime)*timelineScale
                height: timelineHeader.height/2

                color: 'black'

                clip: true

                Text {
                    anchors.centerIn: parent
                    color: 'white'
                    text: entityName
                }

                Button {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    text: 'x'
                    color: 'red'
                    border.color: 'transparent'
                    onClicked: currentSequence.deleteEffect(entityId)
                }

                onRightEdgeChanged: if(rightEdge > timelineLength) timelineLength = rightEdge
            }
        }

        Repeater {
            id: videosRepeater

            model: videosModel

            delegate: Rectangle {
                property real rightEdge: endTime

                x: startTime*timelineScale
                width: (endTime - startTime)*timelineScale
                height: timelineHeader.height/2
                y: height

                clip: true

                color: entityName === 'video1'? 'lightblue':
                       entityName === 'video2'? 'orange':
                                                'lightpink'
                Text {
                    anchors.centerIn: parent
                    color: 'black'
                    text: entityName
                }

                Button {
                    anchors.top: parent.top
                    anchors.right: parent.right
                    text: 'x'
                    color: 'red'
                    border.color: 'transparent'
                    onClicked: currentSequence.deleteVideo(entityId)
                }

                onRightEdgeChanged: if(rightEdge > timelineLength) timelineLength = rightEdge
            }
        }

        Repeater {
            id: ticksView

            model: timelineLength/10

            delegate: Item {
                id: tick
                x: index*width
                width: 10*timelineScale
                height: index%10 === 0? timelineHeader.width : timelineHeader.width/2
                y: timelineHeader.height/2 - height/2

                Rectangle {

                    anchors.verticalCenter: parent.verticalCenter
                    color: 'black'
                    width: 1
                    height: parent.height

                    Text {
                        visible: index % 10 === 0
                        text: index*10
                        y: parent.height - height
                    }
                }
            }
        }
    }

    Rectangle {
        id: timelineSeparator
        y: timelineHeader.height/2
        width: parent.width
        height: 1
        color: 'black'
    }

    Rectangle {
        id: controlButtons
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: playButton.height
        color: 'black'

        Button {
            id: playButton
            width: parent.width/2
            text: running? qsTr('Pause') : qsTr('Play')
            onClicked: togglePlayPauseClicked()
        }

        Button {
            id: resetButton
            anchors.left: playButton.right
            width: parent.width/2
            text: qsTr('Reset')
            onClicked: resetClicked()
        }
    }

    Dialog {
        id: dialog

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: isShown? parent.height : 0

        acceptButtonText: 'Add'
        dismissButtonText: 'Cancel'
    }

 }

