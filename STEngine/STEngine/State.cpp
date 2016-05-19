
#include "State.hpp"

State::State()
	:
	_texMan  (TextureManager::GetInstance()),
	_soundMan(SoundManager::GetInstance()),
	_fontMan (FontManager::GetInstance())
{

}

State::~State()
{

}

bool State::Draw(sf::RenderWindow & win) const noexcept
{
	return _self->Draw(win);
}

State::Flag State::HandleInput(sf::Event & ev)
{
	return _self->HandleInput(ev);
}
sf::View & State::GetView()
{
	return _self->GetView();
}
bool State::SanityCheck()
{
	return (bool)_self && _self->SanityCheck();
}

bool State::UpdateLogic()
{
	return _self->UpdateLogic();
}

std::shared_ptr<State> State::GetState()
{
	return _self->GetState();
}
