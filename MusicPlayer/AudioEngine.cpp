#include "stdafx.h"
#include "AudioEngine.h"

AudioEngine* AudioEngine::_inst = nullptr;

AudioEngine* AudioEngine::GetInstance()
{
    if (!_inst)
        _inst = new AudioEngine(qApp);
    return _inst;
}

AudioEngine::AudioEngine(QObject *parent)
	: QObject(parent), _player(new QMediaPlayer(this)), _audioOutput(new QAudioOutput(this))
{
	_player->setAudioOutput(_audioOutput);
	setVolume(1.0f);
	// 媒体状态变化
	connect(_player, &QMediaPlayer::mediaStatusChanged,
		this, &AudioEngine::onMediaStatusChanged);
	// 时长变化
	connect(_player, &QMediaPlayer::durationChanged,
		this, &AudioEngine::durationChanged);
	// 播放进度更新
	connect(_player, &QMediaPlayer::positionChanged,
		this, &AudioEngine::positionChanged);
}

AudioEngine::~AudioEngine()
{
	stop();
}

void AudioEngine::setSong(const SongStruct& song)
{
	if (_song.url == song.url) return;
	_song.url = song.url;
	_song.image = song.image;
	_song.song_name = song.song_name;
	_song.songer = song.songer;
	_player->stop();
	_player->setSource(song.url);
}

void AudioEngine::setVolume(float volume)
{
	volume = qBound(0.0f, volume, 1.0f);
	if (qFuzzyCompare(_volume, volume)) return;
	_volume = volume;
	_audioOutput->setVolume(_volume);
}

void AudioEngine::seek(qint64 positionMs)
{
}

void AudioEngine::play()
{
	if (_song.url.isEmpty()) return;
	_is_playing = true;
	_player->play();
}

bool AudioEngine::isPlaying()
{
	return _is_playing;
}

void AudioEngine::pause()
{
	if (_song.url.isEmpty()) return;
	_is_playing = false;
	_player->pause();
}

void AudioEngine::stop()
{
	if (_song.url.isEmpty()) return;
	_is_playing = false;
	_player->stop();
}

void AudioEngine::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
	if (status == QMediaPlayer::MediaStatus::EndOfMedia)
	{
		
	}
}

void AudioEngine::durationChanged(qint64 durationMs)
{
	_song.time = durationMs;
	_parent->on_song_changed(_song);
}
void AudioEngine::positionChanged(qint64 positionMs)
{

}