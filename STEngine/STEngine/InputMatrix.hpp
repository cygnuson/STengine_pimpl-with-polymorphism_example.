#ifndef INPUTMATRIX_HPP
#define INPUTMATRIX_HPP

#include <array>
#include <queue>

#include <SFML\Graphics.hpp>

#include "logger.hpp"

class InputMatrix
{
public:
	InputMatrix();
	virtual ~InputMatrix();
	/**Get the string name of a key.*/
	std::string GetKeyName(sf::Keyboard::Key key);
	/**Key names for Mouse buttons.*/
	std::string GetKeyName(sf::Mouse::Button b);
	/*This is a singleton. Get the single instance.*/
	bool ProcessInputIntoMatrix(sf::Event& ev);
	/*Clear all key states and mosue states.*/
	void ClearAll();
	/*return true if the key is down. Unpress = true will force release the 
	key.*/
	bool IsPressed(sf::Keyboard::Key key, bool unpress = false);
	/*Set a key as beign pressed.*/
	void SetPressed(sf::Keyboard::Key key);
	/*Set a key as being released.*/
	void SetReleased(sf::Keyboard::Key key);
	/**mouse overloads.*/
	bool IsPressed(sf::Mouse::Button button, bool unpress = false);
	void SetPressed(sf::Mouse::Button button);
	void SetReleased(sf::Mouse::Button button);
protected:
	std::array<bool, sf::Keyboard::KeyCount> _keys;
	std::array<bool, sf::Mouse::ButtonCount> _buttons;
	std::pair<int, int>                      _mousePosition = { 0,0 };
	float                                    _mouseWheel = 0;

};

#endif //INPUTMATRIX_HPP