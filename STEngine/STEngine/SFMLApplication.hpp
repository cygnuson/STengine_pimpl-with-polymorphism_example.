#ifndef SFMLAPPLICATION_HPP
#define SFMLAPPLICATION_HPP

#include <functional>
#include <thread>
#include <mutex>

#include <SFML/Graphics.hpp>

#include "TextureManager.hpp"
#include "SoundManager.hpp"
#include "FontManager.hpp"
#include "State.hpp"

class SFMLApplication
{
public:
	/**The settings struct is for making an app with special settings.*/
	struct Config
	{
		/**The initial state of the app. The app will throw an exception if
		the initial state is invalid or null.*/
		std::shared_ptr<State> _initialState;
		/**The width of the window for the app.*/
		sf::Uint16             _width = 800;
		/**The height.*/
		sf::Uint16             _height = 600;
		/**The color depth*/
		sf::Uint16             _bitPerPixel = 32;
		/**The key repeat will allow or prevent the app from unpressing a key
		after its been pushed and held.*/
		bool		           _keyRepeat = false;
		bool                   _renderSeperateThread = false;
		bool                   _freezeOnMouseLeave = true;
		bool                   _unfreezeOnMouseEnter = true;
		std::string            _defaultFontPath = "fonts/Mechanical.otf";
		sf::Keyboard::Key      _unfreezeKey = sf::Keyboard::Escape;
		std::string            _title;
		sf::Uint32             _style = sf::Style::Default;
		sf::Uint32             _depthBufferBits = 0;
		sf::Uint32             _stencilBufferBits = 0;
		sf::Uint32             _antiAliasing = 0;
		sf::Uint32             _majorVersion = 1;
		sf::Uint32             _minorVersion = 1;
		sf::Uint32             _attributes = sf::ContextSettings::Default;
	};
	SFMLApplication(const SFMLApplication::Config & config);
	~SFMLApplication();	
	void Start();
	/**The command console.*/
	void StartConsole();
	/**Wait for the window to finish.*/
	void Wait(sf::Time pause = sf::milliseconds(500));
	/**Turn the window off.*/
	void Stop();
protected:
	/**Make sure that the app is sane.*/
	inline bool SanityCheck();
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
	/**The main loop.*/
	void MainLoop();
	/**The drawing specific part of the app loop.*/
	void DrawSection();
	/**Get the top of the stack. Inline, because its called frequently.*/
	inline std::shared_ptr<State> TopState();
	

	/**The context settings of this app.*/
	sf::ContextSettings                 _contextSettings;
	/**The actual window of the app.*/
	std::shared_ptr<sf::RenderWindow>   _window;
	/**The texture manager*/
	TextureManager&                     _texMan;
	/**The sound manager*/
	SoundManager&                       _soundMan;
	/**The font manager*/
	FontManager&                        _fontMan;
	/**The current state.*/
	std::stack<std::shared_ptr<State> > _stack;
	/**True to render in a separate thread.*/
	bool                                _renderSeperateThread;
	/**The separate rendering thread. will only exist if the option was passed
	in during construction.*/
	std::shared_ptr<sf::Thread>         _renderThread;
	/**the thread that runs the main loop.*/
	std::shared_ptr<sf::Thread>         _mainLoopThread;
	/**The stored config settings.*/
	const Config                        _settings;
	/**true if the window has been created.*/
	bool                                _windowCreated;
	/**a lock for accessing the mutex.*/
	std::recursive_mutex                _stackLock;
	/**a lock for changing the the window.*/
	std::recursive_mutex                _windowLock;
	/**a bool for faster determination of weather or not the window is running.
	*/
	bool                                _running;
};

bool SFMLApplication::SanityCheck()
{
	return _windowCreated && StateOk() && DrawOk();
}

inline std::shared_ptr<State> SFMLApplication::TopState()
{
	std::lock_guard<std::recursive_mutex> lock(_stackLock);
	StateOk();
	return _stack.top();
}



#endif //SFMLAPPLICATION_HPP