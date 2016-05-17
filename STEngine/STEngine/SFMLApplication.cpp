#include "SFMLApplication.hpp"



SFMLApplication::SFMLApplication(std::shared_ptr<sf::RenderWindow> target,
	std::shared_ptr<State> initialState)
	:
	_target(target),
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance()),
	_view(initialState->GetView())
{
	if (!initialState)
	{
		cg::logger::log_error("The initial state is invalid.");
		throw std::runtime_error("The initial state is invalid.");
	}
	_stack.push(initialState);
}


SFMLApplication::~SFMLApplication()
{

}

void SFMLApplication::Start()
{
	while (_target->isOpen())
	{
		sf::Event ev;
		while (_target->pollEvent(ev))
		{
			if (WindowEvent(ev))
			{
				/*system input was handled by the function, no need to keep going.*/
				continue;
			}
			else if (InputEvent(ev))
			{
				/*Input was consumed so we continue here.*/
				continue;
			}
			else
			{
				/*Other eevnt handling.*/
			}
		}
		if (DrawOk())
		{
			_target->clear();
			Draw();
			_target->display();
		}
		else
		{
			cg::logger::log_error("The app is not in a drawable state.");
		}

	}
}
bool SFMLApplication::WindowEvent(sf::Event& ev)
{
	switch (ev.type)
	{
	case sf::Event::Closed:
		OnClose(ev);
		return true;
	case sf::Event::Resized:
		OnResize(ev);
		return true;
	case sf::Event::LostFocus:
		OnFocusLost(ev);
		return true;
	case sf::Event::GainedFocus:
		OnFocusGained(ev);
		return true;
	case sf::Event::MouseEntered:
		OnMouseEnter(ev);
		return true;
	case sf::Event::MouseLeft:
		OnMouseLeave(ev);
		return true;
	default:
		cg::logger::log_note(1, "A non-system event has been skipped by the",
			"system HandleInput.");
		return false;
	}
}

bool SFMLApplication::InputEvent(sf::Event & ev)
{
	if (ev.type == sf::Event::KeyPressed
		&& ev.key.code != sf::Keyboard::Unknown)
	{
		cg::logger::log_note(2, "Pressed key: ", GetKeyName(ev.key.code));
	}
	else if (ev.type == sf::Event::KeyReleased
		&& ev.key.code != sf::Keyboard::Unknown)
	{
		cg::logger::log_note(2, "Released key: ", GetKeyName(ev.key.code));
	}
	else if (ev.type == sf::Event::MouseButtonPressed)
	{
		cg::logger::log_note(2, "Pressed button: ",
			GetKeyName(ev.mouseButton.button), " @ [y,x] : [",
			ev.mouseButton.y, ",", ev.mouseButton.x, "]");
	}
	else if (ev.type == sf::Event::MouseButtonReleased)
	{
		cg::logger::log_note(2, "Released button: ",
			GetKeyName(ev.mouseButton.button), " @ [y,x] : [",
			ev.mouseButton.y, ",", ev.mouseButton.x, "]");
	}
	else if (ev.type == sf::Event::MouseMoved) {
		cg::logger::log_note(1, "Mouse moved to [y,x]: [",
			ev.mouseMove.y, ",", ev.mouseMove.x, "]");
	}
	else if (ev.type == sf::Event::MouseWheelScrolled) {
		cg::logger::log_note(2, "Mouse wheel scrolled: ",
			ev.mouseWheelScroll.delta);
	}
	else
	{
		return false;
	}
	StateOk();
	HandleStatePair(_stack.top()->HandleInput(ev));
	return true;
}

bool SFMLApplication::OnResize(sf::Event & ev)
{
	cg::logger::log_note(1, "Window was resized.");
	return false;
}

bool SFMLApplication::OnClose(sf::Event & ev)
{
	Close();
	return false;
}
void SFMLApplication::Close()
{
	while (_stack.size() > 1)
	{
		_stack.pop();
	}
	_target->close();
	cg::logger::log_note(1, "Window was closed.");
}
bool SFMLApplication::OnMouseEnter(sf::Event & ev)
{
	cg::logger::log_note(1, "The mouse enetered the window.");
	return false;
}

bool SFMLApplication::OnMouseLeave(sf::Event & ev)
{
	cg::logger::log_note(1, "The mouse left the window.");
	return false;
}

bool SFMLApplication::OnFocusLost(sf::Event & ev)
{
	cg::logger::log_note(1, "Window lost focus.");
	return false;
}

bool SFMLApplication::OnFocusGained(sf::Event & ev)
{
	cg::logger::log_note(1, "Window gained focus.");
	return false;
}

bool SFMLApplication::Draw()
{
	_stack.top()->Draw(*_target);
	return true; 
}

bool SFMLApplication::StateOk()
{
	if (_stack.empty())
	{
		cg::logger::log_error("The states stack is empty.");
		return false;
	}
	return (bool) _stack.top();
}

bool SFMLApplication::DrawOk()
{
	bool a = _target->isOpen();
	bool b = StateOk();
	return a
		&& b;
}

void SFMLApplication::HandleStatePair(State::StatePair& pair)
{
	switch (pair.first)
	{
	case State::Flag::Exit:
		cg::logger::log_note(3, "Got the exit signal from the state.");
		Close();
		break;
	case State::Flag::Pop:
		cg::logger::log_note(3, "Got the POP signal from the state.");
		PopState();
		break;
	case State::Flag::Push:
		cg::logger::log_note(3, "Got the PUSH signal from the state.");
		PushState(pair.second);
		break;
	default: //will include the None flag.

		break;
	}
}

void SFMLApplication::PushState(std::shared_ptr<State> state)
{
	_stack.push(state);
	StateOk();
}

void SFMLApplication::PopState()
{
	if (_stack.size() == 1)
	{
		cg::logger::log_error("Trying to pop the last state.");
	}
	_stack.pop();
	StateOk();
}
