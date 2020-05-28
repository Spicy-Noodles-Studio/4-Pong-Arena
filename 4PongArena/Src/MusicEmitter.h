#pragma once
#ifndef MUSIC_EMITTER_H
#define MUSIC_EMITTER_H
#include <UserComponent.h>

class SoundEmitter;

class MusicEmitter : public UserComponent
{
public:

private:
	SoundEmitter* soundEmitter;

public:
	MusicEmitter(GameObject* gameObject);
	virtual ~MusicEmitter();
	void playMusic(const std::string& sound);
	void stopMusic(const std::string& sound);
	void stopMusicAll();
	void setVolume(float volume);

protected:
	virtual void start();
};

#endif