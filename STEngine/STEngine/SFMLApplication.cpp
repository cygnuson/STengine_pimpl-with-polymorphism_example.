#include "SFMLApplication.hpp"


SFMLApplication::SFMLApplication(const SFMLApplication::Config & config)
try
	:
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance()),
	_settings(config)
{
	_contextSettings = sf::ContextSettings(
		config._depthBufferBits,
		config._stencilBufferBits,
		config._antiAliasing,
		config._majorVersion,
		config._minorVersion,
		config._attributes);
	_renderSeperateThread = config._renderSeperateThread;
	_stack.push(config._initialState);
	_stack.top()->GetView().setSize(config._width, config._height);
}
catch (const std::exception& e) /*Catch an exception the may throw with the
								initial state not being set.*/
{
	cg::logger::log_error("An exception was thrown. Its likly that the ",
		"initial state was not set in the supplied config file. E:", e.what());
}


SFMLApplication::~SFMLApplication()
{
	if (_renderSeperateThread && _renderThread)
	{
		cg::logger::log_note(3, "Waiting for the draw loop thread to stop.");
		_renderThread->wait();
	}
	if (_mainLoopThread)
	{
		cg::logger::log_note(3, "Waiting for the main loop thread to stop.");
		_mainLoopThread->wait();
	}
}

void SFMLApplication::Start()
{
	if (!SanityCheck())
	{
		cg::logger::log_error("Sanity check failed.");
	}
	_mainLoopThread = std::make_shared<sf::Thread>
		(&SFMLApplication::MainLoop, this);
	_mainLoopThread->launch();
	while (!_windowCreated)
	{
		sf::sleep(sf::microseconds(500));
	}
	StartConsole();
}

void SFMLApplication::MainLoop()
{
	_target = std::make_shared<sf::RenderWindow>(
		sf::VideoMode(
			_settings._width,
			_settings._height,
			_settings._bitPerPixel),
		_settings._title,
		_settings._style,
		_contextSettings);
	_target->setKeyRepeatEnabled(_settings._keyRepeat);
	_windowCreated = true;
	if (_renderSeperateThread)
	{
		auto drawMiniLoop = [&]()
		{
			_target->setActive(true);
			while (_target->isOpen())
			{
				DrawSection();
			}
			cg::logger::log_note(3, "Stopping the drawing miniloop.");
		};
		_target->setActive(false);
		_renderThread = std::make_shared<sf::Thread>(drawMiniLoop);
		_renderThread->launch();
	}

	while (_target->isOpen())
	{
		sf::Event ev;
		while (_target->pollEvent(ev))
		{
			if (InputEvent(ev))
			{
				/*Input was consumed so we continue here.*/
				continue;
			}
			else
			{
				/*Other eevnt handling.*/
			}
		}
		UpdateLogic();
		if (!_renderSeperateThread)
		{
			DrawSection();
		}

	}
	cg::logger::log_note(3, "Closing the main loop thread.");
}
void SFMLApplication::DrawSection()
{
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
void SFMLApplication::StartConsole()
{
	cg::logger::log_note(3, "Starting the command console.");
	while (_target->isOpen())
	{
		/**just sleep for now.*/
		sf::sleep(sf::seconds(1));
	}
}
bool SFMLApplication::SanityCheck()
{
	return StateOk() && DrawOk();
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
		/**other event.*/
	}
	StateOk();
	HandleEventFlag(_stack.top()->HandleInput(ev));
	return true;
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
	return (bool)_stack.top();
}

bool SFMLApplication::DrawOk()
{
	bool a = WindowOk();
	bool b = StateOk();
	return a
		&& b;
}

void SFMLApplication::HandleEventFlag(State::Flag flag)
{
	switch (flag)
	{
	case State::Flag::Exit:
		cg::logger::log_note(3, "Got the exit signal from the state.");
		_target->close();
		break;
	case State::Flag::Pop:
		cg::logger::log_note(3, "Got the POP signal from the state.");
		PopState();
		break;
	case State::Flag::Push:
		cg::logger::log_note(3, "Got the PUSH signal from the state.");
		StateOk();
		PushState(_stack.top()->GetState());
		break;
	default: //will include the None flag.

		break;
	}
}

void SFMLApplication::PushState(std::shared_ptr<State> state)
{
	_stack.push(state);
	auto size = _target->getSize();
	state->GetView().setSize((float)size.x, (float)size.y);
}

void SFMLApplication::PopState()
{
	if (_stack.size() == 1)
	{
		cg::logger::log_error("Trying to pop the last state.");
		return;
	}
	_stack.pop();
}

bool SFMLApplication::WindowOk()
{
	if (!_target || !_target->isOpen())
	{
		cg::logger::log_error("The render window is not setup, but it was "
			, "accessed.");
		return false;
	}
	else
	{
		return true;
	}
}

bool SFMLApplication::UpdateLogic()
{
	_stack.top()->UpdateLogic();
	return true;
}
