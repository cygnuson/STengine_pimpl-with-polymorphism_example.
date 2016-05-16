#ifndef SFMLAPPLICATION_HPP
#define SFMLAPPLICATION_HPP

#include <memory>

#include <SFML/Graphics.hpp>

#include "InputMatrix.hpp"
#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"

class SFMLApplication
{
public:
	SFMLApplication(std::shared_ptr<sf::RenderWindow> target);
	virtual ~SFMLApplication();
	void Start();
	/** Handle input is for handling system related input.  Keyboard/Mouse 
	input should be handled by a call to _inputMatrix.ProcessEvent;
	\param ev [in] a reference to a polled event.
	\return true if the event was consumed.
	*/
	bool HandleInput(sf::Event& ev);
	void Draw();
private:
	/**The actual window of the app.*/
	std::shared_ptr<sf::RenderWindow> _target;
	/**The input matrix*/
	InputMatrix&                      _inputMatrix;
	/**The texture manager*/
	TextureManager&                   _texMan;
	/**The sound manager*/
	SoundManager&                     _soundMan;
	/**The font manager*/
	FontManager&                      _fontMan;
};

#endif //SFMLAPPLICATION_HPP