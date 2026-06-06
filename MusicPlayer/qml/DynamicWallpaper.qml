import QtQuick
import QtMultimedia
import Qt5Compat.GraphicalEffects

Item {
    id: root

    property url videoSource: ""
    property real blurRadius: 0.0        // 0.0 ~ 1.0
    property real brightness: 1.0        // 0.0 ~ 2.0
    property real zoomFactor: 1.0        // 1.0 ~ 2.0
    property bool playing: true

    // 性能监控属性
    property int fps: 0
    property real frameTime: 0
    property int frameCount: 0

    onVideoSourceChanged: console.log("Video source:", videoSource)

    // ═══════════════════════════════════════════
    //  FPS 计数器
    // ═══════════════════════════════════════════

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
            root.fps = root.frameCount * 2  // 0.5秒采样，乘以2得到每秒帧数
            root.frameCount = 0
            if (root.fps > 0) root.frameTime = 1000.0 / root.fps
        }
    }

    // ═══════════════════════════════════════════
    //  视频播放器
    // ═══════════════════════════════════════════

    MediaPlayer {
        id: player
        source: root.videoSource
        videoOutput: videoOutput
        audioOutput: AudioOutput { volume: 0 }
        loops: MediaPlayer.Infinite
        autoPlay: true
        
        onMediaStatusChanged: function(status) {
            if (status === MediaPlayer.LoadedMedia) {
                console.log("Video loaded, duration:", duration)
            }
        }
        onErrorOccurred: function(error, errorString) {
            console.log("Media error:", error, errorString)
        }
    }

    // 主视频输出
    VideoOutput {
        id: videoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
    }

    // ═══════════════════════════════════════════
    //  模糊效果 - 使用 FastBlur 优化性能
    //  FastBlur 比 GaussianBlur 快很多，质量略低但足够好
    // ═══════════════════════════════════════════

    ShaderEffectSource {
        id: videoSource
        sourceItem: videoOutput
        visible: false
        live: true
        recursive: false
    }

    // 使用 FastBlur 替代 GaussianBlur，性能提升 2-3 倍
    FastBlur {
        id: blurEffect
        anchors.fill: parent
        source: videoSource
        radius: blurRadius * 64
        visible: blurRadius > 0.01
        cached: true
    }

    // ═══════════════════════════════════════════
    //  缩放效果
    // ═══════════════════════════════════════════

    transform: [
        Scale {
            origin.x: root.width / 2
            origin.y: root.height / 2
            xScale: root.zoomFactor
            yScale: root.zoomFactor
        }
    ]

    // ═══════════════════════════════════════════
    //  亮度效果
    // ═══════════════════════════════════════════

    Rectangle {
        id: darkenOverlay
        anchors.fill: parent
        color: "black"
        visible: root.brightness < 1.0
        opacity: 1.0 - root.brightness
        z: 10
    }

    Rectangle {
        id: lightenOverlay
        anchors.fill: parent
        color: Qt.rgba(1, 1, 1, Math.min(1, (root.brightness - 1.0) * 0.5))
        visible: root.brightness > 1.0
        z: 10
    }

    // ═══════════════════════════════════════════
    //  播放控制
    // ═══════════════════════════════════════════

    function play() { player.play() }
    function pause() { player.pause() }
    function stop() { player.stop() }

    onPlayingChanged: {
        if (playing) play()
        else pause()
    }

    // ═══════════════════════════════════════════
    //  平滑动画
    // ═══════════════════════════════════════════

    Behavior on blurRadius {
        NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
    }
    Behavior on brightness {
        NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
    }
    Behavior on zoomFactor {
        NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
    }

    // ═══════════════════════════════════════════
    //  帧率显示（每 0.5 秒更新）
    // ═══════════════════════════════════════════

    Rectangle {
        id: fpsOverlay
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 10
        width: fpsText.width + 16
        height: fpsText.height + 10
        radius: 4
        color: Qt.rgba(0, 0, 0, 0.7)
        z: 1000
        visible: true

        Text {
            id: fpsText
            anchors.centerIn: parent
            text: "FPS: " + root.fps
            color: root.fps >= 50 ? "#4caf50" : root.fps >= 30 ? "#ff9800" : "#f44336"
            font.pixelSize: 14
            font.bold: true
            font.family: "Consolas"
        }
    }

    Component.onCompleted: {
        console.log("DynamicWallpaper QML loaded (FastBlur)")
        console.log("Video source:", videoSource)
    }
}
