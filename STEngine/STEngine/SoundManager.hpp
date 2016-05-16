#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <string>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "ResourceManager.hpp"

class SoundManager : public ResourceManager<std::string, sf::Sound>
{
private:
	SoundManager() {};
public:
	virtual ~SoundManager() {};
	static inline SoundManager& GetInstance()
	{
		static SoundManager tm;
		return tm;
	}
};

#endif //SOUNDMANAGER_HPP