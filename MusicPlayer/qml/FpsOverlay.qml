import QtQuick

Item {
    id: root
    width: 120
    height: 40

    property int fps: 0
    property int frameCount: 0

    FrameAnimation {
        id: frameAnim
        running: true
        onTriggered: root.frameCount++
    }

    Timer {
        id: fpsTimer
        interval: 500
        running: true
        repeat: true
        onTriggered: {
            root.fps = root.frameCount * 2
            root.frameCount = 0
        }
    }

    Rectangle {
        anchors.fill: parent
        radius: 4
        color: Qt.rgba(0, 0, 0, 0.7)
        border.color: Qt.rgba(1, 1, 1, 0.2)
        border.width: 1

        Text {
            anchors.centerIn: parent
            text: "FPS: " + root.fps
            color: root.fps >= 50 ? "#4caf50" : root.fps >= 30 ? "#ff9800" : "#f44336"
            font.pixelSize: 14
            font.bold: true
            font.family: "Consolas"
        }
    }
}