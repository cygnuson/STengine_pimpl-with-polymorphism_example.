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
};

#endif //TEXTUREMANAGER_HPP