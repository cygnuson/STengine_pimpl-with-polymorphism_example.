#ifndef SFMLAPPLICATION_HPP
#define SFMLAPPLICATION_HPP

#include <functional>

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"
#include "KeyName.hpp"
#include "State.hpp"

class SFMLApplication
{
public:
	/**The settings struct is for making an app with special settings.*/
	struct Config
	{
		std::shared_ptr<State> _initialState;
		sf::Uint16             _width = 800;
		sf::Uint16             _height = 600;
		sf::Uint16             _bitPerPixel = 32;
		bool		           _keyRepeat = false;
		std::string            _title;
		sf::Uint32             _style = sf::Style::Default;
		sf::Uint32             _depthBufferBits = 0;
		sf::Uint32             _stencilBufferBits = 0;
		sf::Uint32             _antiAliasing = 0;
		sf::Uint32             _majorVersion = 1;
		sf::Uint32             _minorVersion = 1;
		sf::Uint32             _attributes = sf::ContextSettings::Default;
	};
	SFMLApplication(std::shared_ptr<sf::RenderWindow> target, 
		std::shared_ptr<State> initialState);
	SFMLApplication(const SFMLApplication::Config & config);
	virtual ~SFMLApplication();	
	void Start();
private:
	/**Make sure that the app is sane.*/
	bool SanityCheck();
	/**Determine if the state is sane.  Will also pop states that are qued for
	recycling.*/
	bool StateOk();
	/**Determine if the system is in a state that is drawable.*/
	bool DrawOk();
	/**Determine what to do with the stack pair that the state returned.*/
	void HandleEventFlag(State::Flag pair);
	/**Pop a state of fthe stack. All current input will bedumped to the 
	current states que.*/
	void PushState(std::shared_ptr<State> state);
	/**Push a state to the stack. All current input will bedumped to the 
	current states que.*/
	void PopState();
	/**Get the target, while also making sure its set properly.*/
	bool WindowOk();
	/**Update the internal logic of the application.*/
	bool UpdateLogic();
	/** Handle input is for handling system related input.  Keyboard/Mouse
	input should be handled by a call to _inputMatrix.ProcessEvent;
	\param ev [in] a reference to a polled event.
	\return true if the event was consumed.
	*/
	bool InputEvent(sf::Event& ev);
	/**Do system draws and then call the top of the stack.*/
	bool Draw();

	/**The context settings of this app.*/
	sf::ContextSettings                 _contextSettings;
	/**The actual window of the app.*/
	std::shared_ptr<sf::RenderWindow>   _target;
	/**The texture manager*/
	TextureManager&                     _texMan;
	/**The sound manager*/
	SoundManager&                       _soundMan;
	/**The font manager*/
	FontManager&                        _fontMan;
	/**The current state.*/
	std::stack<std::shared_ptr<State> > _stack;
};




#endif //SFMLAPPLICATION_HPP