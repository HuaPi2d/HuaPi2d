import QtQuick 2.3
import Qt5Compat.GraphicalEffects
import QMLVideoInfoListModel 1.0


Item {
    anchors.fill: parent

    Flickable{
        id: flickable
        anchors.fill: parent
        contentWidth: parent.width

        interactive: false

        signal scrollToBottom();

        Flow{
            id: flow
            width: parent.width

            Repeater{
                id: videoList
                model: videoInfoListModel

                signal itemClicked(int index)

                Rectangle{
                    width: 300
                    height: 265

                    Rectangle{
                        id: video
                        anchors.fill: parent
                        anchors.margins: 10

                        Image{
                            id: cover
                            visible: false

                            width: parent.width
                            height: parent.width / 1.75

                            source: pic

                            Image{
                                width: 50
                                height: 50
                                id: previewIcon
                                anchors.centerIn: parent
                                source: "qrc:/icon/resources/icons/bilibili.svg"
                            }

                            onStatusChanged: {
                                if(cover.status === Image.Loading){
                                    previewIcon.visible = visible
                                }
                                else{
                                    previewIcon.visible = false
                                }
                            }
                        }

                        Rectangle{
                            id: maskRec
                            visible: false

                            width: parent.width
                            height: parent.width / 1.75

                            radius: 7
                        }

                        OpacityMask{
                            id: finalImage

                            anchors.fill: cover
                            source: cover
                            maskSource: maskRec

                            Behavior on scale{
                                NumberAnimation{
                                    duration: 200
                                    easing.type: Easing.InQutQuad
                                }
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    itemClicked.connect(videoInfoListModel.itemClicked)
                }
            }
        }
    }

    VideoInfoListModel{
        id: videoInfoListModel
        objectName: "videoInfoListModel"
    }
}
