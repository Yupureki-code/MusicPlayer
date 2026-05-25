#pragma once

#include <QObject>
#include <qmediaplayer.h>
#include <QAudioOutput>

class MusicPlayer;

enum class PlayMode {
	Sequential,      // 顺序播放
	Loop,            // 列表循环
	SingleLoop,      // 单曲循环
	Random           // 随机播放
};

struct SongStruct
{
	QPixmap image;
	QString song_name;
	QString songer;
	QUrl url;
	qint64 time;
	SongStruct& operator=(const SongStruct& other)
	{
		image = other.image;
		song_name = other.song_name;
		songer = other.songer;
		url = other.url;
		time = other.time;
		return *this;
	}
};

class AudioEngine  : public QObject
{
	Q_OBJECT
private:
	AudioEngine(QObject* parent = nullptr);
	AudioEngine(const AudioEngine& other);
public:
	~AudioEngine();
	static AudioEngine* GetInstance();
	void setSongList(const std::vector<SongStruct>& list);
	void setCurrentSongIndex(int x);
	void setPlayMode(PlayMode mode);
	void setVolume(float num);
	void setParent(MusicPlayer* parent) { _parent = parent; }
	float getVolume() const{return _volume;};
	qint64 getDuration() const{return _player->duration();}
	qint64 getPosition() const{return _player->position();}
	PlayMode getPlayMode()const { return _play_mode; }
	void seek(qint64 positionMs);
	void play();
	bool isPlaying();
	void pause();
	void stop();

signals:
	void songChanged(const SongStruct& song);
	void positionChanged(qint64 positionMs);

private slots:
	void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
	void durationChanged(qint64 durationMs);

private:
	static AudioEngine* _inst;
	QMediaPlayer* _player;
	QAudioOutput* _audioOutput;
	MusicPlayer* _parent;
	bool _is_playing = false;
	PlayMode _play_mode;
	std::vector<SongStruct> _song_list;
	int _current_song_index = 0;
	float _volume;
};

#define audio AudioEngine::GetInstance()
