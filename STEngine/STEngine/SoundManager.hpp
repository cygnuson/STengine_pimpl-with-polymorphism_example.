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
	/**Get the name of this manager.*/
	virtual inline std::string GetName()
	{
		return "Sound Manager";
	}
};

#endif //SOUNDMANAGER_HPP