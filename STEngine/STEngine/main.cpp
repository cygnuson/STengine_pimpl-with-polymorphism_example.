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

//TODO: file not found checking for resource manager. (make part of base.)

class TestState : public InputMatrix
{
public:
	TestState(const std::string& tex)
	{
		_test = sf::Sprite(TextureManager::GetInstance()[tex]);
		_view.move(-300, -300);
		IgnoreBadInput(true);
		InputMatrix::CollectText(_oss);
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
		win.draw(_test);
		static sf::Text txt("", FontManager::GetInstance()["default"]);;
		txt.setCharacterSize(30);
		txt.setPosition(0, 0);

		if (IsPressed(sf::Keyboard::Return,true))
		{
			txt = sf::Text(_oss.str(), 
				FontManager::GetInstance()["default"]);;
			_oss.str(std::string());
			//StopCollectText();
		}
		win.draw(txt);
		return true;
	}
	bool SanityCheck() {
		return true;
	}
	bool UpdateLogic(sf::Time dt)
	{
		if (IsPressed(sf::Keyboard::Left))
		{
			_test.move(100*dt.asSeconds(), 100 * dt.asSeconds());
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
		if (ev.type == sf::Event::MouseButtonReleased)
		{
			_stateInQuestion = State::MakeState<TestState>("test2");
			return State::Flag::Push;
		}
		if (ev.type == sf::Event::KeyReleased)
		{
			return State::Flag::Pop;
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
	sf::Sprite             _test;
	sf::View               _view;
	std::shared_ptr<State> _stateInQuestion;
	std::ostringstream     _oss;
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
	SFMLApplication::Config config;
	config._title = "Test Title";
	config._keyRepeat = false;
	config._renderSeperateThread = true;
	config._freezeOnMouseLeave = false;
	config._initialState = State::MakeState<TestState>("test");
	SFMLApplication app(config);

	app.Start();
	app.Wait();
	logStream.close();
}