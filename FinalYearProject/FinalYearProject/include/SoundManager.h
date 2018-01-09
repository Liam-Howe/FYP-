#pragma once
#include "SFML\Audio.hpp"
#include "SoundType.h"
class SoundManager
{
private:
	//sound manger instance
	static SoundManager *m_instance;
	//sound buffers
	sf::SoundBuffer m_startbuffer;
	sf::SoundBuffer m_goalbuffer;
	sf::SoundBuffer m_wallbuffer;
	//sounds
	sf::Sound m_setStartSound;
	sf::Sound m_setGoalSound;
	sf::Sound m_setWallSound;


public:
	SoundManager();
	~SoundManager();
	bool init();
	//loads sounds
	void loadSounds();
	//plays specified sound
	void playSound(SoundType);

	

	static SoundManager *getInstance()
	{
		if (!m_instance)
		{
			m_instance = new SoundManager();
		}
		return m_instance;
	}

};

#pragma once
