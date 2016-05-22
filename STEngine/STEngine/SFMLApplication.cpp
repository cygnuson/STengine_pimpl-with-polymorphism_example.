#include "SFMLApplication.hpp"


SFMLApplication::SFMLApplication(const SFMLApplication::Config & config)
try
	:
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance()),
	_settings(config)
{
	/**Create a context for the window. Is saved as a member for reference*/
	_contextSettings = sf::ContextSettings(
		config._depthBufferBits,
		config._stencilBufferBits,
		config._antiAliasing,
		config._majorVersion,
		config._minorVersion,
		config._attributes);
	/*Render the window in a seperate thread from the main loop.*/
	_renderSeperateThread = config._renderSeperateThread;
	/*Push the initial state to the stack.*/
	_stack.push(config._initialState);
	/*resize the first view to the windows size.*/
	TopState()->GetView().setSize(config._width, config._height);
	/*set the default font.*/
	FontManager::GetInstance().MakeFont("default", config._defaultFontPath);
}
catch (const std::exception& e) /*Catch an exception the may throw with the
								initial state not being set.*/
{
	cg::logger::log_error("An exception was thrown. Its likly that the ",
		"initial state was not set in the supplied config file. E:", e.what());
}


SFMLApplication::~SFMLApplication()
{
	Stop();
}

void SFMLApplication::Start()
{
	/*Create the main loop thread.*/
	_mainLoopThread = std::make_shared<sf::Thread>
		(&SFMLApplication::MainLoop, this);
	_mainLoopThread->launch();
	/*make sure the window is created before starting the console.*/
	while (!_windowCreated)
	{
		sf::sleep(sf::microseconds(500));
	}
}

void SFMLApplication::MainLoop()
{
	std::lock_guard<std::recursive_mutex> lock(_windowLock);
	/*The window is created in the thread the main loop is intended to be run
	inside of.*/
	_window = std::make_shared<sf::RenderWindow>(
		sf::VideoMode(
			_settings._width,
			_settings._height,
			_settings._bitPerPixel),
		_settings._title,
		_settings._style,
		_contextSettings);
	/*allow, or disallow the key repeating for the window.*/
	_window->setKeyRepeatEnabled(_settings._keyRepeat);
	_windowCreated = true;
	_running = true;
	if (!SanityCheck())
	{
		cg::logger::log_error("Sanity check failed.");
	}
	if (_renderSeperateThread)
	{
		/*this mini loop will just call draw over and over again.*/
		auto drawMiniLoop = [&]()
		{
			_window->setActive(true);
			while (_running)
			{
				/*the small portion of the loop that deal with drawing.*/
				DrawSection();
			}
			cg::logger::log_note(3, "Stopping the drawing miniloop.");
		};
		/*The window needs to be made inactive so that it can be activated in
		the other thread that it supposed to run inside of.*/
		_window->setActive(false);
		_renderThread = std::make_shared<sf::Thread>(drawMiniLoop);
		_renderThread->launch();
	}
	/*The actuall event loop for the window.*/
	while (_running)
	{
		sf::Event ev;
		while (_window->pollEvent(ev))
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
		/*The logic poriton is sperate so that its FPS can be controlled
		seperatly.*/
		UpdateLogic();
		if (!_renderSeperateThread)
		{
			/*do the small draw section, only if its not running already in
			another thread.*/
			DrawSection();
		}

	}
	cg::logger::log_note(3, "Closing the main loop thread.");
}
void SFMLApplication::DrawSection()
{
	/*Draw only if the env is sane.*/
	if (DrawOk())
	{
		_window->clear();
		Draw();
		_window->display();
	}
	else
	{
		cg::logger::log_error("The app is not in a drawable state.");
	}
}
void SFMLApplication::StartConsole()
{
	cg::logger::log_note(3, "Starting the command console.");
	while (_window->isOpen())
	{
		/**just sleep for now.*/
		sf::sleep(sf::seconds(1));
	}
}
void SFMLApplication::Wait(sf::Time pause)
{
	while (_running)
	{
		sf::sleep(pause);
	}
}
void SFMLApplication::Stop()
{
	std::lock_guard<std::recursive_mutex> lock(_windowLock);
	_running = false;
	_window->close();
	/*wait for the rendering thread to stop.*/
	if (_renderSeperateThread && _renderThread)
	{
		cg::logger::log_note(3, "Waiting for the draw loop thread to stop.");
		_renderThread->wait();
	}
	/*wait for the main loop to stop.*/
	if (_mainLoopThread)
	{
		cg::logger::log_note(3, "Waiting for the main loop thread to stop.");
		_mainLoopThread->wait();
	}
	_window.reset();
	_windowCreated = false;
}
bool SFMLApplication::InputEvent(sf::Event & ev)
{
	if (ev.type == sf::Event::KeyPressed
		&& ev.key.code != sf::Keyboard::Unknown)
	{
		cg::logger::log_note(2, "Pressed key: ",
			InputMatrix::GetKeyName(ev.key.code));
	}
	else if (ev.type == sf::Event::KeyReleased
		&& ev.key.code != sf::Keyboard::Unknown)
	{
		cg::logger::log_note(2, "Released key: ",
			InputMatrix::GetKeyName(ev.key.code));
	}
	else if (ev.type == sf::Event::MouseButtonPressed)
	{
		cg::logger::log_note(2, "Pressed button: ",
			InputMatrix::GetKeyName(ev.mouseButton.button), " @ [y,x] : [",
			ev.mouseButton.y, ",", ev.mouseButton.x, "]");
	}
	else if (ev.type == sf::Event::MouseButtonReleased)
	{
		cg::logger::log_note(2, "Released button: ",
			InputMatrix::GetKeyName(ev.mouseButton.button), " @ [y,x] : [",
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
	HandleEventFlag(TopState()->HandleInput(ev));
	return true;
}
bool SFMLApplication::Draw()
{
	/*draw the top of the stack.*/
	TopState()->Draw(*_window);
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
	bool c = _windowCreated;
	return a
		&& b
		&& c;
}

void SFMLApplication::HandleEventFlag(State::Flag flag)
{
	switch (flag)
	{
	case State::Flag::Exit:
		cg::logger::log_note(3, "Got the exit signal from the state.");
		/*set _running to false so that the app can soft-close*/
		_running = false;
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
	std::lock_guard<std::recursive_mutex> lock(_stackLock);
	cg::logger::log_note(2, "Pushing a state to the app.");
	if (state)
	{
		TopState()->Freeze();
		_stack.push(state);
		auto size = _window->getSize();
		/*set the top states view to be the same size as the window.*/
		state->GetView().setSize((float)size.x, (float)size.y);
	}
	else
	{
		cg::logger::log_error("The state expected to be pushed to the stack",
			" was invalid.");
		throw std::runtime_error("The state pushed was invalid.");
	}
}

void SFMLApplication::PopState()
{
	std::lock_guard<std::recursive_mutex> lock(_stackLock);
	cg::logger::log_note(2, "Poping a state from the app.");
	if (_stack.size() == 1)
	{
		cg::logger::log_error("Trying to pop the last state.");
		return;
	}
	TopState()->Unfreeze();
	_stack.pop();
}

bool SFMLApplication::WindowOk()
{
	if (!_window || !_window->isOpen())
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
	TopState()->UpdateLogic();
	return true;
}
