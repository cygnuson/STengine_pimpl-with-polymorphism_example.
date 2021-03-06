#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <string>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "ResourceManager.hpp"
/**A manager class for loading resources only one.*/
class SoundManager : public ResourceManager<std::string, sf::SoundBuffer>
{
private:
	SoundManager() {};
public:
	virtual ~SoundManager() {};
	/**Get the single allowed instance of the sound manager.
	\return The manager as a reference to a single instance.*/
	static inline SoundManager& GetInstance()
	{
		static SoundManager tm;
		return tm;
	}
	/**Get the name of this manager.
	\return the name of the manager, for debugging.*/
	virtual inline std::string GetName()
	{
		return "Sound Manager";
	}
	/**Add a item to the manager.
	\param name [in] the name of the resource to load.
	\param path [in] path the path to the resource to load.
	\return a reference to the newly loaded resource.*/
	virtual inline sf::SoundBuffer& MakeSound(const std::string& name,
		const std::string& path)
	{
		cg::logger::log_note(2, GetName(), "(MakeSound): Making the "
			"sound ", name, " from ", path);
		Emplace(name, path).loadFromFile(path);
		return this->Get(name);
	}
};

#endif //SOUNDMANAGER_HPP