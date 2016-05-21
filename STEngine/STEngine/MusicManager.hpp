#ifndef MUSICMANAGER_HPP
#define MUSICMANAGER_HPP

#include <string>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include "ResourceManager.hpp"
/**A manager class for loading resources only one.*/
class MusicManager : public ResourceManager<std::string, sf::Music>
{
private:
	MusicManager() {};
public:
	virtual ~MusicManager() {};
	/**Get the single allowed instance of the sound manager.
	\return The manager as a reference to a single instance.*/
	static inline MusicManager& GetInstance()
	{
		static MusicManager tm;
		return tm;
	}
	/**Get the name of this manager.
	\return the name of the manager, for debugging.*/
	virtual inline std::string GetName()
	{
		return "Music Manager";
	}
	/**Add a item to the manager.
	\param name [in] the name of the resource to load.
	\param path [in] path the path to the resource to load.
	\return a reference to the newly loaded resource.*/
	virtual inline sf::Music& MakeMusic(const std::string& name,
		const std::string& path)
	{
		cg::logger::log_note(2, GetName(), "(MakeMusic): Making the "
			"sound ", name, " from ", path);
		Emplace(name, path).openFromFile(path);
		return this->Get(name);
	}
};

#endif //MUSICMANAGER_HPP