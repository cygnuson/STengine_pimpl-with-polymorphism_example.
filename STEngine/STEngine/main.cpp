#ifdef _WIN32
#include "../LinkLibraries.hpp"
#endif //_WIN32

#include <fstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "FontManager.hpp"
#include "SoundManager.hpp"
#include "TextureManager.hpp"

#include "SFMLApplication.hpp"
#include "MobileAnimation.hpp"


class TestState : public InputMatrix
{
public:
	enum Direction : uint64_t
	{
		Up = 1,
		Down,
		Left,
		Right,
	};
	TestState(const std::string& tex)
	{
		_test.SetDirectionAnimation(Direction::Down,
			Animation (tex, { 32,32 }, 10.f, { 0,0 }, { 3,1 }));
		_test.SetDirectionAnimation(Direction::Up,
			Animation (tex, { 32,32 }, 10.f, { 0,3 }, { 3,1 }));
		_test.SetDirectionAnimation(Direction::Left,
			Animation (tex, { 32,32 }, 10.f, { 0,1 }, { 3,1 }));
		_test.SetDirectionAnimation(Direction::Right,
			Animation (tex, { 32,32 }, 10.f, { 0,2 }, { 3,1 }));
		_direction = Direction::Up;
		_test.SetStillFrameIndex(1);
		_view.move(-300, -300);
		_moving = false;
		IgnoreBadInput(true);
	}
	virtual ~TestState()
	{

	}
	void Freeze()
	{
		InputMatrix::ClearAll();
	}
	void Unfreeze()
	{

	}
	bool Draw(sf::RenderWindow& win, sf::Time dt)
	{
		win.setView(_view);
		_test.Draw(win,_direction,!_moving);
		return true;
	}
	bool SanityCheck() {
		return true;
	}
	bool UpdateLogic(sf::Time dt)
	{
		if (IsPressed(sf::Keyboard::Up))
		{
			if (!IsPressed(sf::Keyboard::Left) 
				&& !IsPressed(sf::Keyboard::Right))
			{
				_direction = Direction::Up;
			}
			_test.Move({ 0, -100 * dt.asSeconds() });
			_moving = true;
			//_test.Rotate(dt.asSeconds() * 60);
		}
		else if (IsPressed(sf::Keyboard::Down))
		{
			if (!IsPressed(sf::Keyboard::Left)
				&& !IsPressed(sf::Keyboard::Right))
			{
				_direction = Direction::Down;
			}
			_test.Move({0, 100 * dt.asSeconds() });
			_moving = true;
			//_test.Rotate(dt.asSeconds() * 60);
		}
		if (IsPressed(sf::Keyboard::Left))
		{
			_direction = Direction::Left;
			_test.Move({ -100 * dt.asSeconds(), 0});
			_moving = true;
			//_test.Rotate(dt.asSeconds() * 60);
		}
		else if (IsPressed(sf::Keyboard::Right))
		{
			_direction = Direction::Right;
			_test.Move({ 100 * dt.asSeconds(), 0 });
			_moving = true;
			//_test.Rotate(dt.asSeconds() * 60);
		}
		if(_moving
			&& !IsPressed(sf::Keyboard::Right)
			&& !IsPressed(sf::Keyboard::Left)
			&& !IsPressed(sf::Keyboard::Down)
			&& !IsPressed(sf::Keyboard::Right))
		{
			_moving = false;
		}
		return true;
	}
	void Resize(sf::Event& ev)
	{
		_view.setSize(float(ev.size.width), float(ev.size.height));
	}
	State::Flag HandleInput(sf::Event& ev, sf::Time dt)
	{
		if (ev.type == sf::Event::Closed)
		{
			return State::Flag::Exit;
		}
		if (ev.type == sf::Event::Resized)
		{
			Resize(ev);
			return State::Flag::None;
		}
		
		ProcessEvent(ev);
		return State::Flag::None;
	}
	std::shared_ptr<State> GetState()
	{
		return _stateInQuestion;
	}
	sf::View& GetView()
	{
		return _view;
	}
	MobileAnimation        _test;
	Direction              _direction;
	sf::View               _view;
	std::shared_ptr<State> _stateInQuestion;
	bool                   _moving;
};

int main()
{
	std::ofstream logStream("log.txt");
	cg::logger::init(cg::logger::all & ~cg::logger::note1, std::cout);
	TextureManager& tm = TextureManager::GetInstance();
	FontManager& fm = FontManager::GetInstance();
	SoundManager& sm = SoundManager::GetInstance();

	tm.MakeTexture("test", "tex/test.jpg");
	tm.MakeTexture("test2", "tex/test2.jpg");
	tm.MakeTexture("testanim", "tex/test_texture.png");
	tm.MakeTexture("testship", "tex/ship_texture_trans.png");
	SFMLApplication::Config config;
	config._title = "Test Title";
	config._keyRepeat = false;
	config._renderSeperateThread = true;
	config._initialState = State::MakeState<TestState>("testship");
	SFMLApplication app(config);

	app.Start();
	app.Wait();
	logStream.close();
}