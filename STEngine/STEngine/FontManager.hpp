#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "ResourceManager.hpp"

class FontManager : public ResourceManager<std::string,sf::Font>
{
private:
	FontManager() {};
public:
	virtual ~FontManager() {};
	static inline FontManager& GetInstance()
	{
		static FontManager tm;
		return tm;
	}
};

#endif // FONTMANAGER_HPP