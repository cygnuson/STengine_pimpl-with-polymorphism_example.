#include "SFMLApplication.hpp"



SFMLApplication::SFMLApplication(std::shared_ptr<sf::RenderWindow> target,
	std::shared_ptr<State> initialState)
	:
	_target(target),
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance())
{
	PushState(initialState);
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
	TopState()->HandleInput(ev);
	return true;
}

bool SFMLApplication::OnResize(sf::Event & ev)
{
	cg::logger::log_note(1, "Window was resized.");
	return false;
}

bool SFMLApplication::OnClose(sf::Event & ev)
{
	_target->close();
	cg::logger::log_note(1, "Window was closed.");
	return false;
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
	auto s = TopState();
	return s->Draw(*_target);
}

std::shared_ptr<State>& SFMLApplication::TopState()
{
	if (StateOk())
	{
		return _states.top();
	}
	else
	{
		_target->close();
		cg::logger::log_warning("There are no states.");
		throw std::runtime_error("There are no states.");
	}
}

void SFMLApplication::PopState()
{
	/*called from StateOk() so it there should be state on the stack.*/
	auto lastState = _states.top(); /*Keep alive if someone wants it befor its
									destroyed.*/
	_states.pop();

}

std::shared_ptr<State> SFMLApplication::PushState(std::shared_ptr<State> state)
{
	_states.push(state);
	_view = &state->_view;
}

bool SFMLApplication::StateOk()
{
	if (_states.empty())
	{
		cg::logger::log_error("There is no state available.");
		return false;
	}
	if (_states.top()->_popMe)
	{
		PopState();
		if (_states.empty())
		{
			cg::logger::log_warning("The last state was popped.");
			_target->close();
			return false;
		}
		else
		{
			return true;
		}
	}
	return true;
}

bool SFMLApplication::DrawOk()
{
	bool a = _target->isOpen();
	bool b = StateOk();
	return a
		&& b;
}
