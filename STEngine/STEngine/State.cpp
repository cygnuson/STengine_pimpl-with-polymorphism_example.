
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

bool State::Draw(sf::RenderWindow & win, float dt)
{
	return _self->Draw(win,dt);
}

State::Flag State::HandleInput(sf::Event & ev, float dt)
{
	return _self->HandleInput(ev,dt);
}
sf::View & State::GetView()
{
	return _self->GetView();
}
bool State::SanityCheck()
{
	return (bool)_self && _self->SanityCheck();
}

bool State::UpdateLogic(float dt)
{
	return _self->UpdateLogic(dt);
}

std::shared_ptr<State> State::GetState()
{
	return _self->GetState();
}
