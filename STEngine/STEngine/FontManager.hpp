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
	/**Get the name of this manager.*/
	virtual inline std::string GetName()
	{
		return "Font Manager";
	}
};

#endif // FONTMANAGER_HPP