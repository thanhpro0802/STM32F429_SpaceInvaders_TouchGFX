#include <gui/common/FrontendApplication.hpp>

#include <mvp/MVPHeap.hpp>
#include <gui/common/FrontendHeap.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{

}

void FrontendApplication::gotoGameOverScreenScreenNoTransition()
{
    transitionCallback = touchgfx::Callback<FrontendApplicationBase>(this, static_cast<void (FrontendApplicationBase::*)()>(&FrontendApplication::gotoGameOverScreenScreenNoTransitionImpl));
    pendingScreenTransitionCallback = &transitionCallback;
}

void FrontendApplication::gotoGameOverScreenScreenNoTransitionImpl()
{
    makeTransition<GameOverScreenView, GameOverScreenPresenter, touchgfx::NoTransition, Model >(&currentScreen, &currentPresenter, frontendHeap, &currentTransition, &model);
}
