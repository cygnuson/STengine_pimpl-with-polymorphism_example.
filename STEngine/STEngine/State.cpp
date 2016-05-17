
#include "State.hpp"

State::State()
	:
	_texMan(TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan(FontManager::GetInstance())
{

}

State::~State()
{

}

bool State::Draw(sf::RenderWindow & win)
{
	return _self->Draw(win);
}

State::StatePair State::HandleInput(sf::Event & ev)
{
	return _self->HandleInput(ev);
}
sf::View & State::GetView()
{
	return _self->GetView();
}
bool State::SanityCheck()
{
	return (bool)_self;
}
