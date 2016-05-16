#ifndef SFMLAPPLICATION_HPP
#define SFMLAPPLICATION_HPP

#include <functional>
#include <memory>
#include <stack>

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"
#include "KeyName.hpp"
#include "State.hpp"

class SFMLApplication
{
public:
	SFMLApplication(std::shared_ptr<sf::RenderWindow> target, 
		std::shared_ptr<State> initialState);
	virtual ~SFMLApplication();	
private:
	/**Determine if the state is sane.  Will also pop states that are qued for
	recycling.*/
	bool StateOk();
	/**Determine if the system is in a state that is drawable.*/
	bool DrawOk();
	void Start();
	/** Handle input is for handling system related input.  Keyboard/Mouse
	input should be handled by a call to _inputMatrix.ProcessEvent;
	\param ev [in] a reference to a polled event.
	\return true if the event was consumed.
	*/
	bool WindowEvent(sf::Event& ev);
	bool InputEvent(sf::Event& ev);
	bool OnResize(sf::Event& ev);
	bool OnClose(sf::Event& ev);
	bool OnMouseEnter(sf::Event& ev);
	bool OnMouseLeave(sf::Event& ev);
	bool OnFocusLost(sf::Event& ev);
	bool OnFocusGained(sf::Event& ev);
	/**Do system draws and then call the top of the stack.*/
	bool Draw();
	/**Short hand to grab the top state.*/
	std::shared_ptr<State>& TopState();
	/**Try to pop a state.*/
	void PopState();

	/**The actual window of the app.*/
	std::shared_ptr<sf::RenderWindow>  _target;
	/**The texture manager*/
	TextureManager&                    _texMan;
	/**The sound manager*/
	SoundManager&                      _soundMan;
	/**The font manager*/
	FontManager&                       _fontMan;
	/**The state stack*/
	std::stack<std::shared_ptr<State>> _states;
};

#endif //SFMLAPPLICATION_HPP