#include "../include/SoundManager.h"

SoundManager *SoundManager::m_instance = 0;

SoundManager::SoundManager()
{
	
}

SoundManager::~SoundManager()
{
}


bool SoundManager::init()
{
	loadSounds();
	return true;
}

void SoundManager::loadSounds()
{
	m_startbuffer.loadFromFile("assets/sounds/start.wav");
	m_wallbuffer.loadFromFile("assets/sounds/wall.wav");
	m_goalbuffer.loadFromFile("assets/sounds/Goal.wav");
	
}

void SoundManager::playSound(SoundType soundID)
{
	switch (soundID)
	{
	case SoundType::START:
		m_setStartSound.setBuffer(m_startbuffer);
		m_setStartSound.play();
		break;
	case SoundType::WALL:
		m_setWallSound.setBuffer(m_wallbuffer);
		m_setWallSound.play();
		break;
	case SoundType::GOAL:
		m_setGoalSound.setBuffer(m_goalbuffer);
		m_setGoalSound.play();
		break;
	}
}






