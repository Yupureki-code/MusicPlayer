#include "stdafx.h"
#include "AudioEngine.h"
#include "MusicPlayer.h"
#include <QRandomGenerator>

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

void AudioEngine::setSongList(const std::vector<SongStruct>& list)
{
	_song_list = list;
	_current_song_index = 0;
}

void AudioEngine::setCurrentSongIndex(int x)
{
	_current_song_index = x;
}

void AudioEngine::setPlayMode(PlayMode mode)
{
	_play_mode = mode;
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
	if (_player->isSeekable())
		_player->setPosition(positionMs);
}

void AudioEngine::play()
{
	if (_current_song_index < _song_list.size() && _song_list[_current_song_index].url.isEmpty()) return;
	_is_playing = true;
	_player->setSource(_song_list[_current_song_index].url);
	_player->play();
}

bool AudioEngine::isPlaying()
{
	return _is_playing;
}

void AudioEngine::pause()
{
	if (_song_list.empty() || _current_song_index >= (int)_song_list.size()) return;
	if (_song_list[_current_song_index].url.isEmpty()) return;
	_is_playing = false;
	_player->pause();
}

void AudioEngine::stop()
{
	if (_song_list.empty() || _current_song_index >= (int)_song_list.size()) return;
	if (_song_list[_current_song_index].url.isEmpty()) return;
	_is_playing = false;
	_player->stop();
}

void AudioEngine::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
	if (status == QMediaPlayer::MediaStatus::EndOfMedia)
	{
		if (_song_list.empty()) return;
		if (_play_mode == PlayMode::Sequential && _current_song_index < _song_list.size() - 1)
		{
			_current_song_index++;
			play();
		}
		else if (_play_mode == PlayMode::Loop)
		{
			_current_song_index = (_current_song_index + 1) % _song_list.size();
			play();
		}
		else if (_play_mode == PlayMode::SingleLoop)
		{
			play();
		}
		else if (_play_mode == PlayMode::Random)
		{
			_current_song_index = QRandomGenerator::global()->bounded(_song_list.size());
			play();
		}
	}
}

void AudioEngine::durationChanged(qint64 durationMs)
{
	if (_song_list.empty() || _current_song_index >= (int)_song_list.size()) return;
	_song_list[_current_song_index].time = durationMs;
	emit songChanged(_song_list[_current_song_index]);
}