#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"
/**A manager class for loading resources only one.*/
class TextureManager : public ResourceManager<std::string,sf::Texture>
{
private:
	TextureManager() {};
public:
	virtual ~TextureManager() {};
	/**Get the single allowed instance of the texture manager.
	\return The manager as a reference to a single instance.*/
	static inline TextureManager& GetInstance()
	{
		static TextureManager tm;
		return tm;
	}
	/**Get the name of this manager.
	\return a string that is the naem of the manger for logging.*/
	virtual inline std::string GetName()
	{
		return "Texture Manager";
	}
	/**Add a item to the manager.
	\param name [in] the name of the resource to load.
	\param path [in] path the path to the resource to load.
	\return a reference to the newly loaded resource.*/
	virtual inline sf::Texture& MakeTexture(const std::string& name,
		const std::string& path)
	{
		cg::logger::log_note(2, GetName(), "(MakeTexture): Making the "
			"texture ", name, " from ", path);
		Emplace(name, path).loadFromFile(path);
		return this->Get(name);
	}
};

#endif //TEXTUREMANAGER_HPP