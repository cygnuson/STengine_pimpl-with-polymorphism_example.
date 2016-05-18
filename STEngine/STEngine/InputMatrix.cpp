#include "InputMatrix.hpp"



InputMatrix::InputMatrix()
{
	ClearAll();
}

InputMatrix::~InputMatrix()
{

}

bool InputMatrix::IsPressed(sf::Keyboard::Key key, bool unpress)
{
	bool retValue = _keys[key];
	if (retValue && unpress) {
		SetReleased(key);
	}
	return retValue;
}
bool InputMatrix::IsPressed(sf::Mouse::Button button, bool unpress)
{
	bool retValue = _buttons[button];
	if (retValue && unpress) {
		SetReleased(button);
	}
	return retValue;
}
void InputMatrix::SetPressed(sf::Keyboard::Key key)
{
	_keys[key] = true;
}
void InputMatrix::SetPressed(sf::Mouse::Button button)
{
	_buttons[button] = true;
}
void InputMatrix::SetReleased(sf::Keyboard::Key key)
{
	_keys[key] = false;
}
void InputMatrix::SetReleased(sf::Mouse::Button button)
{
	_buttons[button] = false;
}

bool InputMatrix::IgnoreBadInput(bool ignore)
{
	return _ignoreBadInput = ignore;
}

std::string InputMatrix::GetEventName(const sf::Event::EventType& ev)
{
	switch (ev)
	{
	case sf::Event::Closed:
		return "Closed";
	case sf::Event::Resized:
		return "Resized";
	case sf::Event::LostFocus:
		return "LostFocus";
	case sf::Event::GainedFocus:
		return "GainedFocus";
	case sf::Event::TextEntered:
		return "TextEntered";
	case sf::Event::KeyPressed:
		return "KeyPressed";
	case sf::Event::KeyReleased:
		return "KeyReleased";
	case sf::Event::MouseWheelMoved:
		return "MouseWheelMoved";
	case sf::Event::MouseButtonPressed:
		return "MouseButtonPressed";
	case sf::Event::MouseButtonReleased:
		return "MouseButtonReleased";
	case sf::Event::MouseMoved:
		return "MouseMoved";
	case sf::Event::MouseEntered:
		return "MouseEntered";
	case sf::Event::MouseLeft:
		return "MouseLeft";
	case sf::Event::JoystickButtonPressed:
		return "JoystickButtonPressed";
	case sf::Event::JoystickButtonReleased:
		return "JoystickButtonReleased";
	case sf::Event::JoystickMoved:
		return "JoystickMoved";
	case sf::Event::JoystickConnected:
		return "JoystickConnected";
	case sf::Event::JoystickDisconnected:
		return "JoystickDisconnected";
	}
}

void InputMatrix::CollectText(std::ostream& ofs)
{
	_textStream = &ofs;
	_collectText = true;
}

void InputMatrix::StopCollectText()
{
	_collectText = false;
	_textStream = nullptr;
}

bool InputMatrix::ProcessEvent(sf::Event & ev)
{
	if (ev.type == sf::Event::TextEntered && _collectText && _textStream)
	{
		*_textStream << (char)ev.text.unicode;
	}
	else if (ev.type == sf::Event::TextEntered && !_collectText)
	{
		
	}
	else if (ev.type == sf::Event::KeyPressed
		&& ev.key.code != sf::Keyboard::Unknown
		&& !_keys[ev.key.code])
	{
		_keys[ev.key.code] = true;
	}
	else if (ev.type == sf::Event::KeyReleased
		&& ev.key.code != sf::Keyboard::Unknown
		&& _keys[ev.key.code])
	{
		_keys[ev.key.code] = false;
	}
	else if (ev.type == sf::Event::MouseButtonPressed
		&& !_buttons[ev.mouseButton.button])
	{
		_buttons[ev.mouseButton.button] = true;
	}
	else if (ev.type == sf::Event::MouseButtonReleased
		&& _buttons[ev.mouseButton.button])
	{
		_buttons[ev.mouseButton.button] = false;
	}
	else if (ev.type == sf::Event::MouseMoved) {
		_mousePosition.second = ev.mouseMove.x;
		_mousePosition.first = ev.mouseMove.y;
	}
	else if (ev.type == sf::Event::MouseWheelScrolled) {
		_mouseWheel = ev.mouseWheelScroll.delta;
	}
	else if (ev.type == sf::Event::JoystickButtonPressed)
	{

	}
	else if (ev.type == sf::Event::JoystickButtonReleased)
	{

	}
	else if (ev.type == sf::Event::TouchBegan)
	{

	}
	else if (ev.type == sf::Event::TouchEnded)
	{

	}
	else if (ev.type == sf::Event::TouchMoved)
	{

	}

	else if (ev.type == sf::Event::SensorChanged)
	{

	}
	else
	{
		cg::logger::log_warning("A non-input event made it to the input",
			" an matrix. Type is `", GetEventName(ev.type), "`.");
		if (!_ignoreBadInput)
		{
			throw std::runtime_error("A non-input was sent to the input "
				"solve this by handling all those events BEFORE the input "
				" matrix is called.");
		}
		return false;
	}
	return true;
}

void InputMatrix::ClearAll()
{
	cg::logger::log_note(2, "InputMatrix (ClearAll): Keys all cleared.");
	_keys.fill(false);
	_buttons.fill(false);
}
std::string InputMatrix::GetKeyName(sf::Mouse::Button b)
{
	switch (b) {
	case sf::Mouse::Button::Left:
		return std::string("Left");
	case sf::Mouse::Button::Right:
		return std::string("Right");
	case sf::Mouse::Button::Middle:
		return std::string("Middle");
	case sf::Mouse::Button::XButton1:
		return std::string("XB1");
	case sf::Mouse::Button::XButton2:
		return std::string("XB2");
	default:
		return std::string("Unknown");
	}
}

//This helper function is from BlackHC at the forum.  I have modified it.
std::string InputMatrix::GetKeyName(sf::Keyboard::Key key)
{
	switch (key) {
	default:
	case sf::Keyboard::Unknown:
		return std::string("Unknown");
	case sf::Keyboard::A:
		return std::string("A");
	case sf::Keyboard::B:
		return std::string("B");
	case sf::Keyboard::C:
		return std::string("C");
	case sf::Keyboard::D:
		return std::string("D");
	case sf::Keyboard::E:
		return std::string("E");
	case sf::Keyboard::F:
		return std::string("F");
	case sf::Keyboard::G:
		return std::string("G");
	case sf::Keyboard::H:
		return std::string("H");
	case sf::Keyboard::I:
		return std::string("I");
	case sf::Keyboard::J:
		return std::string("J");
	case sf::Keyboard::K:
		return std::string("K");
	case sf::Keyboard::L:
		return std::string("L");
	case sf::Keyboard::M:
		return std::string("M");
	case sf::Keyboard::N:
		return std::string("N");
	case sf::Keyboard::O:
		return std::string("O");
	case sf::Keyboard::P:
		return std::string("P");
	case sf::Keyboard::Q:
		return std::string("Q");
	case sf::Keyboard::R:
		return std::string("R");
	case sf::Keyboard::S:
		return std::string("S");
	case sf::Keyboard::T:
		return std::string("T");
	case sf::Keyboard::U:
		return std::string("U");
	case sf::Keyboard::V:
		return std::string("V");
	case sf::Keyboard::W:
		return std::string("W");
	case sf::Keyboard::X:
		return std::string("X");
	case sf::Keyboard::Y:
		return std::string("Y");
	case sf::Keyboard::Z:
		return std::string("Z");
	case sf::Keyboard::Num0:
		return std::string("Num0");
	case sf::Keyboard::Num1:
		return std::string("Num1");
	case sf::Keyboard::Num2:
		return std::string("Num2");
	case sf::Keyboard::Num3:
		return std::string("Num3");
	case sf::Keyboard::Num4:
		return std::string("Num4");
	case sf::Keyboard::Num5:
		return std::string("Num5");
	case sf::Keyboard::Num6:
		return std::string("Num6");
	case sf::Keyboard::Num7:
		return std::string("Num7");
	case sf::Keyboard::Num8:
		return std::string("Num8");
	case sf::Keyboard::Num9:
		return std::string("Num9");
	case sf::Keyboard::Escape:
		return std::string("Escape");
	case sf::Keyboard::LControl:
		return std::string("LControl");
	case sf::Keyboard::LShift:
		return std::string("LShift");
	case sf::Keyboard::LAlt:
		return std::string("LAlt");
	case sf::Keyboard::LSystem:
		return std::string("LSystem");
	case sf::Keyboard::RControl:
		return std::string("RControl");
	case sf::Keyboard::RShift:
		return std::string("RShift");
	case sf::Keyboard::RAlt:
		return std::string("RAlt");
	case sf::Keyboard::RSystem:
		return std::string("RSystem");
	case sf::Keyboard::Menu:
		return std::string("Menu");
	case sf::Keyboard::LBracket:
		return std::string("LBracket");
	case sf::Keyboard::RBracket:
		return std::string("RBracket");
	case sf::Keyboard::SemiColon:
		return std::string("SemiColon");
	case sf::Keyboard::Comma:
		return std::string("Comma");
	case sf::Keyboard::Period:
		return std::string("Period");
	case sf::Keyboard::Quote:
		return std::string("Quote");
	case sf::Keyboard::Slash:
		return std::string("Slash");
	case sf::Keyboard::BackSlash:
		return std::string("BackSlash");
	case sf::Keyboard::Tilde:
		return std::string("Tilde");
	case sf::Keyboard::Equal:
		return std::string("Equal");
	case sf::Keyboard::Dash:
		return std::string("Dash");
	case sf::Keyboard::Space:
		return std::string("Space");
	case sf::Keyboard::Return:
		return std::string("Return");
	case sf::Keyboard::BackSpace:
		return std::string("BackSpace");
	case sf::Keyboard::Tab:
		return std::string("Tab");
	case sf::Keyboard::PageUp:
		return std::string("PageUp");
	case sf::Keyboard::PageDown:
		return std::string("PageDown");
	case sf::Keyboard::End:
		return std::string("End");
	case sf::Keyboard::Home:
		return std::string("Home");
	case sf::Keyboard::Insert:
		return std::string("Insert");
	case sf::Keyboard::Delete:
		return std::string("Delete");
	case sf::Keyboard::Add:
		return std::string("Add");
	case sf::Keyboard::Subtract:
		return std::string("Subtract");
	case sf::Keyboard::Multiply:
		return std::string("Multiply");
	case sf::Keyboard::Divide:
		return std::string("Divide");
	case sf::Keyboard::Left:
		return std::string("Left");
	case sf::Keyboard::Right:
		return std::string("Right");
	case sf::Keyboard::Up:
		return std::string("Up");
	case sf::Keyboard::Down:
		return std::string("Down");
	case sf::Keyboard::Numpad0:
		return std::string("Numpad0");
	case sf::Keyboard::Numpad1:
		return std::string("Numpad1");
	case sf::Keyboard::Numpad2:
		return std::string("Numpad2");
	case sf::Keyboard::Numpad3:
		return std::string("Numpad3");
	case sf::Keyboard::Numpad4:
		return std::string("Numpad4");
	case sf::Keyboard::Numpad5:
		return std::string("Numpad5");
	case sf::Keyboard::Numpad6:
		return std::string("Numpad6");
	case sf::Keyboard::Numpad7:
		return std::string("Numpad7");
	case sf::Keyboard::Numpad8:
		return std::string("Numpad8");
	case sf::Keyboard::Numpad9:
		return std::string("Numpad9");
	case sf::Keyboard::F1:
		return std::string("F1");
	case sf::Keyboard::F2:
		return std::string("F2");
	case sf::Keyboard::F3:
		return std::string("F3");
	case sf::Keyboard::F4:
		return std::string("F4");
	case sf::Keyboard::F5:
		return std::string("F5");
	case sf::Keyboard::F6:
		return std::string("F6");
	case sf::Keyboard::F7:
		return std::string("F7");
	case sf::Keyboard::F8:
		return std::string("F8");
	case sf::Keyboard::F9:
		return std::string("F9");
	case sf::Keyboard::F10:
		return std::string("F10");
	case sf::Keyboard::F11:
		return std::string("F11");
	case sf::Keyboard::F12:
		return std::string("F12");
	case sf::Keyboard::F13:
		return std::string("F13");
	case sf::Keyboard::F14:
		return std::string("F14");
	case sf::Keyboard::F15:
		return std::string("F15");
	case sf::Keyboard::Pause:
		return std::string("Pause");
	}
}