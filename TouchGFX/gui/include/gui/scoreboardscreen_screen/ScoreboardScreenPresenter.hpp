#ifndef SCOREBOARDSCREENPRESENTER_HPP
#define SCOREBOARDSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScoreboardScreenView;

class ScoreboardScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScoreboardScreenPresenter(ScoreboardScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ScoreboardScreenPresenter() {}

    int16_t getHighScoreAt(uint8_t index) const { return model->getHighScoreAt(index); }

private:
    ScoreboardScreenPresenter();

    ScoreboardScreenView& view;
};

#endif // SCOREBOARDSCREENPRESENTER_HPP
