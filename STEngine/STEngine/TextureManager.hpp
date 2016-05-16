#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "ResourceManager.hpp"

class TextureManager : public ResourceManager<std::string,sf::Texture>
{
private:
	TextureManager() {};
public:
	virtual ~TextureManager() {};
	static inline TextureManager& GetInstance()
	{
		static TextureManager tm;
		return tm;
	}
	/**Get the name of this manager.*/
	virtual inline std::string GetName()
	{
		return "Texture Manager";
	}
	/**Add a item to the manager.*/
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