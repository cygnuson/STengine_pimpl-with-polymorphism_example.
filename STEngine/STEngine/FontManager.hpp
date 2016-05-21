#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP

#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "ResourceManager.hpp"
/**A manager class for loading resources only one.*/
class FontManager : public ResourceManager<std::string,sf::Font>
{
private:
	FontManager() {};
public:
	virtual ~FontManager() {};
	/**Get the single allowed instance of the font manager.
	\return The manager as a reference to a single instance.*/
	static inline FontManager& GetInstance() noexcept
	{
		static FontManager tm;
		return tm;
	}
	/**Get the name of this manager.
	\return the name of the manager, for debugging.*/
	virtual inline std::string GetName() const noexcept
	{
		return "Font Manager";
	}
	/**Add a item to the manager.
	\param name [in] the name of the resource to load.
	\param path [in] path the path to the resource to load.
	\return a reference to the newly loaded resource.*/
	virtual inline sf::Font& MakeFont(const std::string& name,
		const std::string& path)
	{
		cg::logger::log_note(2, GetName(), "(MakeFont): Making the "
			"font ", name, " from ", path);
		Emplace(name, path).loadFromFile(path);
		return this->Get(name);
	}
};

#endif // FONTMANAGER_HPP