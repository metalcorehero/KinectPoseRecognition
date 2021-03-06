#include "GameControlScreen.h"

using namespace ci;
using namespace gl;
using namespace ci::app;
using namespace std;

void GameControlScreen::setup()
	{
		sidePlashka = *AssetManager::getInstance()->getTexture("images/diz/sidePlashka.jpg");
		matchingPopup = *AssetManager::getInstance()->getTexture("images/diz/poseAlreadyBubble.png");
		palka = *AssetManager::getInstance()->getTexture("images/diz/kolbasa.png");
		timer_bubble = *AssetManager::getInstance()->getTexture("images/diz/time_bubble.png");
		popka = *AssetManager::getInstance()->getTexture("images/diz/popka.jpg");

		timerVec = Vec2f(1458.0f, 1080.0f);
		plashkaVec = Vec2f(1920.0f, 0.0f);
		silhouetteAlpha = 0.0f;

		secondsFont = Font(loadFile(getAssetPath("fonts/maestroc.ttf")), 120);
		percentFont = Font(loadFile(getAssetPath("fonts/maestroc.ttf")), 190);
		debugFontText = Font(loadFile(getAssetPath("fonts/MyriadPro-BlackSemiExt.ttf")), 26);

		Font percentTexureFont = Font(loadFile(getAssetPath("fonts/maestroc.ttf")), 70);
		percentTexure = Utils::getTextField("%", &percentTexureFont, Color::white());

		setDetentionPercent(0.0f);

		purpurColor.r = 196.0f / 255.0f;
		purpurColor.g = 47.0f / 255.0f;
		purpurColor.b = 57.0f / 255.0f;

		greyColor.r = 223.0f / 255.0f;
		greyColor.g = greyColor.r;
		greyColor.b = greyColor.r;

		state = HIDING;
	}

void GameControlScreen::init()
	{
		state = HIDING;
		timerVec = Vec2f(1458.0f, 1080.0f);
		plashkaVec = Vec2f(1920.0f, 0.0f);
		silhouetteAlpha = 0.0f;
	}

void GameControlScreen::draw()
	{
		drawYellowCountur();
		drawCountDownSeconds();
		drawLeftPercentPanel();
		drawMatchingPopup();

		gl::color(Color::white());
	}

void GameControlScreen::drawYellowCountur()
	{
		if (currentPose)
		{
			//float part = (1440 - 1080) * 0.5 * poseScale;
			gl::pushMatrices();
			gl::translate(kinect().viewShiftX, kinect().viewShiftY);
			gl::translate(poseShiftVec * 3);
			gl::scale(poseScale, poseScale);

			gl::color(ColorA(1.0f, 1.0f, 1.0f, silhouetteAlpha));
			currentPose->draw();
			gl::popMatrices();
		}
	}

void GameControlScreen::drawCountDownSeconds()
	{
		gl::color(Color::white());

		gl::pushMatrices();
		gl::translate(timerVec);
		gl::draw(timer_bubble);
		string sec = to_string(showingSeconds);
		if (sec.size() < 2)
			sec = "0" + sec;
		gl::Texture time = Utils::getTextField("00:" + sec, &secondsFont, Color::hex(0xc42f39));
		gl::translate(Vec2f(46.0f, 13.0f));
		gl::draw(time);
		gl::Texture percent = Utils::getTextField(to_string(percentMatching), &percentFont, Color::white());
		gl::color(Color::hex(0xc42f39));
		gl::translate(0.0f, -200.0f);
		//gl::draw(percent);
		gl::popMatrices();
	}

void GameControlScreen::drawLeftPercentPanel()
	{
		gl::color(Color::white());
		gl::pushMatrices();
		gl::translate(plashkaVec);
		gl::draw(sidePlashka);
		gl::pushMatrices();
		gl::translate(5.0f, palkaPosition);
		gl::draw(palka);
		gl::popMatrices();
		gl::translate(5.0f, 1023.0f);
		gl::draw(popka);
		gl::popMatrices();
	}

void GameControlScreen::drawMatchingPopup()
	{
		if (matchingPopupAlpha > 0.0f)
		{
			gl::pushMatrices();
			gl::color(ColorA(1.0f, 1.0f, 1.0f, matchingPopupAlpha));
			gl::translate(Vec2f(1267.0f, 53.0f));
			gl::draw(matchingPopup);
			gl::translate(240.0f, 347.0f);

			drawMatchingBubbles();

			gl::popMatrices();
		}
	}

void GameControlScreen::drawMatchingBubbles()
	{
		int numActiveCircles = circlesNum * matchingProgress;
		for (int i = 0; i < circlesNum; i++)
		{
			gl::pushMatrices();
			if (i + 1 <= numActiveCircles)
			{
				gl::translate(13.0f + 68.0f*i, 0.0f);
				gl::color(ColorA(purpurColor.r, purpurColor.g, purpurColor.b, matchingPopupAlpha));
				gl::drawSolidCircle(Vec2f::zero(), 24.0f, 40);
			}
			else
			{
				gl::translate(13.0f + 68.0f*i, 0.0f);
				gl::color(ColorA(greyColor.r, greyColor.g, greyColor.b, matchingPopupAlpha));
				gl::drawSolidCircle(Vec2f::zero(), 13.0f, 40);
			}
			gl::popMatrices();
		}
	}

void GameControlScreen::setTimeForAnimation(int seconds)
	{
		showingSeconds = seconds;
	}

void GameControlScreen::show()
	{
		if (state == HIDING)
		{
			silhouetteAlpha = 0.0f;
			state = STATIC_SHOWING;
			initStartAnimation();
		}
	}

void GameControlScreen::show1()
	{
		if (state == HIDING)
		{
			silhouetteAlpha = 0.0f;
			state = STATIC_SHOWING;
			initStartAnimation1();
		}
	}

void GameControlScreen::show2()
	{
		timeline().apply(&timerVec, Vec2f(1458.0f, 1080.0f), Vec2f(1458.0f, 868.0f), 1.2f, EaseOutCubic());
	}

void GameControlScreen::initStartAnimation()
	{
		timeline().apply(&timerVec, Vec2f(1458.0f, 1080.0f), Vec2f(1458.0f, 868.0f), 1.2f, EaseOutCubic());
		timeline().apply(&plashkaVec, Vec2f(1920.0f, 0.0f), Vec2f(1831.0f, 0.0f), 1.2f, EaseOutCubic());
	}

void GameControlScreen::initStartAnimation1()
	{
		timeline().apply(&plashkaVec, Vec2f(1920.0f, 0.0f), Vec2f(1831.0f, 0.0f), 1.2f, EaseOutCubic());
	}

void GameControlScreen::quickAnimation(int timeToQuickAnimate)
	{
		if (state == QUICK_ANIMATION) return;
		state = QUICK_ANIMATION;
	}

void GameControlScreen::quickAnimationSetProgress(float percent)
	{
		if (percent > 1.0f) percent = 1.0f;
		if (percent < 0.0f) percent = 0.0f;

		showingSeconds = startingTime + (int)((quickAnimTime - startingTime) * (1.0f - percent));
		setDetentionPercent(quickAnimPosePercent * (1 - percent));

		state = RUNNING;
	}

void GameControlScreen::setTime(int seconds)
	{
		startingTime = seconds;
	}

void GameControlScreen::setShowingTime(int seconds)
	{
		showingSeconds = seconds;
	}

void GameControlScreen::setQuickAnimTime(int time)
	{
		quickAnimTime = time;
		showingSeconds = time;
	}

void GameControlScreen::setQuickAnimPosePercent(float percent)
	{
		quickAnimPosePercent = percent;
		percentMatching = (int)percent;
		setDetentionPercent(percent);
	}

void GameControlScreen::run()
	{
		initStartAnimation();
		start();
	}

	void GameControlScreen::start()
	{
		state = RUNNING;
		setDetentionPercent(0.0f);
	}

	void GameControlScreen::hide()
	{
		state = HIDING;
		timeline().apply(&timerVec, Vec2f(1458.0f, 1080.0f), 1.2f, EaseOutCubic());
		timeline().apply(&plashkaVec, Vec2f(1920.0f, 0.0f), 1.2f, EaseOutCubic());
		timeline().apply(&silhouetteAlpha, 1.0f, 0.0f, 0.4f, EaseInCubic());
		timeline().apply(&matchingPopupAlpha, 0.0f, 0.3f, EaseInCubic());
	}

	void GameControlScreen::setDetentionPercent(float percent)
	{
		if (percent > 1.0f) percent = 1.0f;
		if (percent < 0.0f) percent = 0.0f;

		percentMatching = (int)(percent * 100.0f);

		if (percentMatching >= 100.0f)
			percentMatching -= 1;

		palkaPosition = 884.0f + (8.0f - 884.0f) * percent;
	}

	void GameControlScreen::showSilhouette()
	{
		timeline().apply(&silhouetteAlpha, 0.0f, 1.0f, 0.4f, EaseInCubic());
	}

	void GameControlScreen::setCurrentPose(Pose *pose)
	{
		currentPose = pose;
	}

	void GameControlScreen::showMatching(float  progressInt)
	{
		matchingPopupAlpha = (progressInt <= 0.01f ? 0.0f : 1.0f);
		matchingProgress = progressInt;
	}

	void GameControlScreen::setPoseScale(float  _poseScale)
	{
		poseScale = _poseScale;
	}

	void GameControlScreen::setPoseShift(const Vec2f&  poseShiftVec)
	{
		this->poseShiftVec = poseShiftVec;
	}