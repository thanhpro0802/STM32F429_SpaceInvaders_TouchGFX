#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::updateGameState(const GameState& state)
{
    view.updateGameState(state);
    if (state.isGameOver)
    {
        view.gotoGameOverScreen();
    }
}

void Screen1Presenter::setPlayerX(int16_t x)
{
    model->setPlayerX(x);
}

void Screen1Presenter::fireBullet()
{
    model->fireBullet();
}

void Screen1Presenter::resetGame()
{
    model->resetGame();
}
