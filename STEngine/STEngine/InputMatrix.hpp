#ifndef INPUTMATRIX_HPP
#define INPUTMATRIX_HPP

#include <array>

#include <SFML\Graphics.hpp>

#include "logger.hpp"

class InputMatrix
{
public:
	virtual ~InputMatrix();
	/**Get the string name of a key.*/
	std::string GetKeyName(sf::Keyboard::Key key);
	/*This is a singleton. Get the single instance.*/
	static InputMatrix& GetInstance();
	/*Process events into the matrix. Retrun true if an event was consumed.*/
	bool ProcessEvent(sf::Event& ev);
	/*Clear all key states and mosue states.*/
	void ClearAll();
	/**Access the keys.*/
	bool operator[](sf::Keyboard::Key key);
	/**Access the mouse buttons.*/
	bool operator[](sf::Mouse::Button button);

	std::array<bool, sf::Keyboard::KeyCount> _keys;
	std::array<bool, sf::Mouse::ButtonCount> _buttons;
	std::pair<int, int>                      _mousePosition = { 0,0 };
	int                                      _mouseWheel = 0;
private:
	/*private constructor. this is a singleton.*/
	InputMatrix();
	/*Delete all the copy/move members.*/
	InputMatrix(const InputMatrix& other) = delete;
	InputMatrix(InputMatrix&& other) = delete;
	void operator=(const InputMatrix& other) = delete;
	void operator=(InputMatrix&& other) = delete;
};

#endif //INPUTMATRIX_HPP