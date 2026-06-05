import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    width: 1280
    height: 720

    // ═══════════════════════════════════════════
    //  动态壁纸
    // ═══════════════════════════════════════════

    DynamicWallpaper {
        id: wallpaper
        anchors.fill: parent
        videoSource: "file:///E:/C C++ Files/test/Test/background_1080p.mp4"
        blurRadius: blurSlider.value
        brightness: brightnessSlider.value
        zoomFactor: zoomSlider.value
    }

    // ═══════════════════════════════════════════
    //  性能监控面板
    // ═══════════════════════════════════════════

    Rectangle {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        width: 180
        height: perfColumn.height + 20
        radius: 8
        color: Qt.rgba(0.1, 0.1, 0.15, 0.9)
        border.color: Qt.rgba(1, 1, 1, 0.15)
        border.width: 1
        z: 100

        ColumnLayout {
            id: perfColumn
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 10
            spacing: 6

            Text {
                text: "性能监控"
                font.pixelSize: 14
                font.bold: true
                color: "#4fc3f7"
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.2)
            }

            RowLayout {
                Layout.fillWidth: true
                Text { text: "FPS:"; color: "#aaa"; font.pixelSize: 12; font.family: "Consolas" }
                Item { Layout.fillWidth: true }
                Text { 
                    text: wallpaper.fps.toString()
                    color: wallpaper.fps >= 50 ? "#4caf50" : wallpaper.fps >= 30 ? "#ff9800" : "#f44336"
                    font.pixelSize: 14 
                    font.bold: true
                    font.family: "Consolas"
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Text { text: "帧时间:"; color: "#aaa"; font.pixelSize: 12; font.family: "Consolas" }
                Item { Layout.fillWidth: true }
                Text { 
                    text: wallpaper.frameTime.toFixed(1) + "ms"
                    color: wallpaper.frameTime <= 16.7 ? "#4caf50" : wallpaper.frameTime <= 33 ? "#ff9800" : "#f44336"
                    font.pixelSize: 12 
                    font.family: "Consolas"
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Text { text: "模糊:"; color: "#aaa"; font.pixelSize: 12; font.family: "Consolas" }
                Item { Layout.fillWidth: true }
                Text { 
                    text: (wallpaper.blurRadius * 100).toFixed(0) + "%"
                    color: "#e91e63"
                    font.pixelSize: 12 
                    font.family: "Consolas"
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Text { text: "明度:"; color: "#aaa"; font.pixelSize: 12; font.family: "Consolas" }
                Item { Layout.fillWidth: true }
                Text { 
                    text: (wallpaper.brightness * 100).toFixed(0) + "%"
                    color: "#8bc34a"
                    font.pixelSize: 12 
                    font.family: "Consolas"
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Text { text: "缩放:"; color: "#aaa"; font.pixelSize: 12; font.family: "Consolas" }
                Item { Layout.fillWidth: true }
                Text { 
                    text: (wallpaper.zoomFactor * 100).toFixed(0) + "%"
                    color: "#ff9800"
                    font.pixelSize: 12 
                    font.family: "Consolas"
                }
            }
        }
    }

    // ═══════════════════════════════════════════
    //  控制面板
    // ═══════════════════════════════════════════

    Rectangle {
        id: controlPanel
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        width: 300
        height: controlColumn.height + 40
        radius: 12
        color: Qt.rgba(0.1, 0.1, 0.15, 0.9)
        border.color: Qt.rgba(1, 1, 1, 0.15)
        border.width: 1
        z: 100

        ColumnLayout {
            id: controlColumn
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 20
            spacing: 16

            Text {
                text: "动态壁纸控制"
                font.pixelSize: 18
                font.bold: true
                color: "white"
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.2)
            }

            // 模糊度
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                RowLayout {
                    Layout.fillWidth: true
                    Text { text: "模糊度 (毛玻璃)"; color: "white"; font.pixelSize: 13 }
                    Item { Layout.fillWidth: true }
                    Text { text: (blurSlider.value * 100).toFixed(0) + "%"; color: "#e91e63"; font.pixelSize: 13; font.bold: true }
                }

                Slider {
                    id: blurSlider
                    Layout.fillWidth: true
                    from: 0.0
                    to: 1.0
                    value: 0.5
                    stepSize: 0.01

                    background: Rectangle {
                        x: blurSlider.leftPadding
                        y: blurSlider.topPadding + blurSlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: blurSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: Qt.rgba(1, 1, 1, 0.15)

                        Rectangle {
                            width: blurSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#e91e63"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: blurSlider.leftPadding + blurSlider.visualPosition * (blurSlider.availableWidth - width)
                        y: blurSlider.topPadding + blurSlider.availableHeight / 2 - height / 2
                        implicitWidth: 18
                        implicitHeight: 18
                        radius: 9
                        color: blurSlider.pressed ? "#ffffff" : "#e91e63"
                        border.color: "#c2185b"
                    }
                }
            }

            // 明度
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                RowLayout {
                    Layout.fillWidth: true
                    Text { text: "明度"; color: "white"; font.pixelSize: 13 }
                    Item { Layout.fillWidth: true }
                    Text { text: (brightnessSlider.value * 100).toFixed(0) + "%"; color: "#8bc34a"; font.pixelSize: 13; font.bold: true }
                }

                Slider {
                    id: brightnessSlider
                    Layout.fillWidth: true
                    from: 0.0
                    to: 2.0
                    value: 1.0
                    stepSize: 0.01

                    background: Rectangle {
                        x: brightnessSlider.leftPadding
                        y: brightnessSlider.topPadding + brightnessSlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: brightnessSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: Qt.rgba(1, 1, 1, 0.15)

                        Rectangle {
                            width: brightnessSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#8bc34a"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: brightnessSlider.leftPadding + brightnessSlider.visualPosition * (brightnessSlider.availableWidth - width)
                        y: brightnessSlider.topPadding + brightnessSlider.availableHeight / 2 - height / 2
                        implicitWidth: 18
                        implicitHeight: 18
                        radius: 9
                        color: brightnessSlider.pressed ? "#ffffff" : "#8bc34a"
                        border.color: "#689f38"
                    }
                }
            }

            // 缩放
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                RowLayout {
                    Layout.fillWidth: true
                    Text { text: "缩放"; color: "white"; font.pixelSize: 13 }
                    Item { Layout.fillWidth: true }
                    Text { text: (zoomSlider.value * 100).toFixed(0) + "%"; color: "#ff9800"; font.pixelSize: 13; font.bold: true }
                }

                Slider {
                    id: zoomSlider
                    Layout.fillWidth: true
                    from: 1.0
                    to: 1.5
                    value: 1.0
                    stepSize: 0.01

                    background: Rectangle {
                        x: zoomSlider.leftPadding
                        y: zoomSlider.topPadding + zoomSlider.availableHeight / 2 - height / 2
                        implicitWidth: 200
                        implicitHeight: 4
                        width: zoomSlider.availableWidth
                        height: implicitHeight
                        radius: 2
                        color: Qt.rgba(1, 1, 1, 0.15)

                        Rectangle {
                            width: zoomSlider.visualPosition * parent.width
                            height: parent.height
                            color: "#ff9800"
                            radius: 2
                        }
                    }

                    handle: Rectangle {
                        x: zoomSlider.leftPadding + zoomSlider.visualPosition * (zoomSlider.availableWidth - width)
                        y: zoomSlider.topPadding + zoomSlider.availableHeight / 2 - height / 2
                        implicitWidth: 18
                        implicitHeight: 18
                        radius: 9
                        color: zoomSlider.pressed ? "#ffffff" : "#ff9800"
                        border.color: "#f57c00"
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Qt.rgba(1, 1, 1, 0.2)
            }

            // 预设按钮
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Button {
                    text: "重置"
                    Layout.fillWidth: true
                    onClicked: {
                        blurSlider.value = 0
                        brightnessSlider.value = 1.0
                        zoomSlider.value = 1.0
                    }

                    background: Rectangle {
                        implicitHeight: 34
                        radius: 6
                        color: parent.pressed ? Qt.rgba(1, 1, 1, 0.2) : Qt.rgba(1, 1, 1, 0.1)
                        border.color: Qt.rgba(1, 1, 1, 0.3)
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    text: "毛玻璃"
                    Layout.fillWidth: true
                    onClicked: {
                        blurSlider.value = 0.7
                        brightnessSlider.value = 0.85
                        zoomSlider.value = 1.05
                    }

                    background: Rectangle {
                        implicitHeight: 34
                        radius: 6
                        color: parent.pressed ? Qt.rgba(1, 1, 1, 0.2) : Qt.rgba(1, 1, 1, 0.1)
                        border.color: Qt.rgba(1, 1, 1, 0.3)
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    text: "暗色"
                    Layout.fillWidth: true
                    onClicked: {
                        blurSlider.value = 0.4
                        brightnessSlider.value = 0.5
                        zoomSlider.value = 1.1
                    }

                    background: Rectangle {
                        implicitHeight: 34
                        radius: 6
                        color: parent.pressed ? Qt.rgba(1, 1, 1, 0.2) : Qt.rgba(1, 1, 1, 0.1)
                        border.color: Qt.rgba(1, 1, 1, 0.3)
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "white"
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
    }
}
