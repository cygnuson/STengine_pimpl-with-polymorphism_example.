#ifndef INPUTMATRIX_HPP
#define INPUTMATRIX_HPP

#include <array>
#include <queue>

#include <SFML\Graphics.hpp>

#include "logger.hpp"
/**The input matrix` sole job is to record user input from the keyboard, the
mouse, and the joystick.

The InputMatrix is designed to be called LAST after all other input has been
checked for. This makes sure that all event are handled.  If you (the dev) want
the non-input events to be ignored, use IgnoreBadInput(true);
*/
class InputMatrix
{
public:
	InputMatrix() noexcept;
	virtual ~InputMatrix() noexcept;
	/**Get the string name of a key.*/
	static std::string GetKeyName(sf::Keyboard::Key key)noexcept;
	/**Key names for Mouse buttons.*/
	static std::string GetKeyName(sf::Mouse::Button b)noexcept;
	/*This is a singleton. Get the single instance.*/
	bool ProcessEvent(sf::Event& ev);
	/*Clear all key states and mosue states.*/
	void ClearAll();
	/*return true if the key is down. Unpress = true will force release the 
	key.*/
	bool IsPressed(sf::Keyboard::Key key, bool unpress);
	bool IsPressed(sf::Keyboard::Key key) const;
	/*Set a key as beign pressed.*/
	void SetPressed(sf::Keyboard::Key key);
	/*Set a key as being released.*/
	void SetReleased(sf::Keyboard::Key key);
	/**mouse overloads.*/
	bool IsPressed(sf::Mouse::Button button, bool unpress);
	bool IsPressed(sf::Mouse::Button button) const;
	void SetPressed(sf::Mouse::Button button);
	void SetReleased(sf::Mouse::Button button);
	/**Start or stop ignoring bad input.*/
	bool IgnoreBadInput(bool ignore);
	/**Get events name for logging.*/
	static std::string GetEventName(const sf::Event::EventType& ev) noexcept;
	/**start collecting text instead of collecting key presses.  The text is
	piped to ofs.*/
	void CollectText(std::ostream& ofs);
	/**Stop collecting text.*/
	void StopCollectText();
protected:
	std::array<bool, sf::Keyboard::KeyCount> _keys;
	std::array<bool, sf::Mouse::ButtonCount> _buttons;
	std::pair<int, int>                      _mousePosition = { 0,0 };
	float                                    _mouseWheel = 0;
	/**If true, the matrix will not throw up exceptions for bad events (events
	that are not key/mouse/joystick events.  this is usually false, as it will 
	require the developer to implement all the event handlers.*/
	bool                                     _ignoreBadInput = false;
	/**If this is true, all key board input will go straigh to the tex buffer
	and not the key press array.*/
	bool                                     _collectText = false;
	/**the stream that will get the text collected.*/
	std::ostream*                            _textStream = nullptr;

};

#endif //INPUTMATRIX_HPP