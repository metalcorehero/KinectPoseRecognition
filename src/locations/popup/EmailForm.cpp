#include "EmailForm.h"
#include <ctype.h>
#include <boost/algorithm/string.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace mndl::curl;

void EmailForm::setup()
{	
	keyBoardMainBgTex		= AssetManager::getInstance()->getTexture(  "images/06_podl.jpg" );
	emailLineTex			= AssetManager::getInstance()->getTexture( "keyboard/emailLine.png"  );
	closeEmailTex			= gl::Texture( loadImage( loadAsset("keyboard/closeEmail.png"  )));
	deleteAllTex			= gl::Texture( loadImage(getAssetPath("keyboard/deleteAll.png"  )));
	addEmailTex				= gl::Texture( loadImage( loadAsset( "images/06_add.png"  )));	

	emailInputFont			= Font( loadFile(getAssetPath("fonts/ToyotaDisplay_Bd.ttf")), 70 );
	emailAddFont			= Font( loadFile(getAssetPath("fonts/ToyotaDisplay_Bd.ttf")), 20 );

	addEmailBtn = new ButtonTex(addEmailTex, "addEmail", getWindow());
	addEmailBtn->setScreenField(Vec2f(389.0f, 260.0f));	

	deleteAllLettersBtn = new ButtonTex(deleteAllTex, "deleteAll", getWindow());	
	deleteAllLettersBtn->setScreenField(Vec2f(1463.0f, 135.0f));	

	closeEmailBtn= new ButtonTex(closeEmailTex, "closeEmail", getWindow());
	closeEmailBtn->setScreenField( Vec2f(1778.0f, 88.0f));	
		
	touchKeyBoard.setup( getWindow(), Vec2f(360.0f, getWindowHeight() - 504.0f));	
	touchKeyBoard.initKeyboard();
}

void EmailForm::show()
{
	currentEmail = "";
	emailVector.clear();
	bgPosition =  Vec2f(0.0f, 0.0f);
	timeline().apply( &bgPosition, Vec2f(0.0f, 0.0f), Vec2f(0.0f,  getWindowHeight() - 1754.0f), 0.6f, EaseOutQuart()).delay(0.4f).finishFn( bind( &EmailForm::initHandlers, this ) );
	timeline().apply( &bgColor, ColorA(1.0f, 1.0f, 1.0f, 0.0f), ColorA(1.0f, 1.0f, 1.0f, 0.98f), 0.5f, EaseOutQuart());
}

void EmailForm::initHandlers()
{
	keyboardTouchSignal = touchKeyBoard.keyboardTouchSignal.connect(boost::bind(&EmailForm::keyboardTouchSignalHandler, this));
	closeBtnSignal		=  closeEmailBtn->mouseDownEvent.connect(boost::bind(&EmailForm::closeSignalHandler, this));
	deleteAllLettersSignal =  deleteAllLettersBtn->mouseDownEvent.connect(boost::bind(&EmailForm::deleteAllLettersHandler, this));
	addEmailSignal		=  addEmailBtn->mouseDownEvent.connect(boost::bind(&EmailForm::addEmailHandler, this));
}

void EmailForm::closeSignalHandler()
{
	mode = "closeMail";
	hide();
}

void EmailForm::keyboardTouchSignalHandler()
{
	string lastCode = touchKeyBoard.getLastCode();		

	if (touchKeyBoard.isBackCode())
	{
		currentEmail = currentEmail.substr(0, currentEmail.size() - 1);	
		return;
	}

	if (currentEmail.size() > 30)   return;

	if (touchKeyBoard.isMailCode())
	{
		if (currentEmail.size() == 0) return;	
				
		if(currentEmail[currentEmail.size() - 1] =='@')
		{
			currentEmail = currentEmail + lastCode;
			boost::to_upper(currentEmail);
		}
		else
		{
			currentEmail = currentEmail + "@" + lastCode;
			boost::to_upper(currentEmail);
		}
		return;
	}
	else if (touchKeyBoard.isSendCode())
	{
		if ((currentEmail.size() == 0 && emailVector.size() == 0 ) ||(currentEmail.size() && !Utils::isValidEmail(currentEmail)))
		{	
			return;
		}
		if (currentEmail.size() != 0)
			addCurrentEmail(currentEmail);	

		mode = "sendMail";
	
		hide();
		return;
	}

	currentEmail = currentEmail + lastCode;
	boost::to_upper(currentEmail);
}

std::vector<std::string> EmailForm::getEmails()
{
	return emailVector;
}

std::string EmailForm::getEmailsInString()
{
	string allEmails = "";
	for (size_t i = 0; i < emailVector.size(); i++)
	{
		if ( i != emailVector.size()-1)
		{
			allEmails +=emailVector[i] +",";
		}
		else allEmails +=emailVector[i];		
	}

	return allEmails;
}

void EmailForm::disconnectAll()
{
	keyboardTouchSignal.disconnect();
	closeBtnSignal.disconnect();
	deleteAllLettersSignal.disconnect();
	addEmailSignal.disconnect();
}

void EmailForm::hide()
{
	timeline().apply( &bgPosition,Vec2f(0.0f, 0.0f), 0.6f, EaseInQuart()).finishFn( bind( &EmailForm::closedHandler, this ) );		
	timeline().apply( &bgColor, ColorA(1.0f, 1.0f, 1.0f, 0.4f), 0.5f, EaseInQuart()).delay(0.4f);
}

void EmailForm::closedHandler()
{
	if (mode == "sendMail")
	{
		sendEvent();
	}
	else if (mode == "closeMail")
	{
		closeEvent();
	}
}

void EmailForm::deleteAllLettersHandler()
{
	currentEmail = "";	
}

void EmailForm::addEmailHandler()
{
	if (currentEmail.size() == 0)
	{
	
	}
	else if (Utils::isValidEmail(currentEmail) == false)
	{
		
	}
	else
	{
		addCurrentEmail(currentEmail);
	}	
}

void EmailForm::drawEmailInput()
{
	TextLayout _text;
	_text.setFont( emailInputFont );
	_text.setColor( Color::white());	
	_text.addLine(currentEmail);	
	 gl::Texture textTexture = gl::Texture( _text.render( true, false ) );

	 Vec2f _scale = Vec2f(1.0f,1.0f);
	
	 if (textTexture.getWidth() > 1050.0f)
	{
		float sc = 1050.0f / textTexture.getWidth();
		_scale= Vec2f(sc, sc);		
	}

	float coordy = deleteAllLettersBtn->getY() - _scale.x*textTexture.getHeight()*0.5f;

	gl::color(ColorA(1.0f, 1.0f, 1.0f, 1.0f));
	gl::pushMatrices();		
		gl::translate(Vec2f(391.0f, coordy));
		gl::scale(_scale);
		gl::draw(textTexture);
		gl::translate(Vec2f(textTexture.getWidth(),0.0f));
		if ((int)getElapsedSeconds() % 2 == 0 )
		gl::drawSolidRect(Rectf(0.0f, 0.0f, 8.0f, textTexture.getHeight()));
	gl::popMatrices();
}

void EmailForm::drawAdditionEmails()
{
	if (emailVector.size() == 0 ) return;

	for (size_t i = 0, ilen = emailVector.size(); i < ilen; i++)
	{
		TextLayout simple;
		simple.setFont( emailAddFont );
		simple.setColor( Color::white());
		simple.addLine(emailVector[i]);	

		gl::Texture textTexture = gl::Texture( simple.render( true, false ) );

		gl::color(Color::white());

		gl::pushMatrices();
				gl::translate(Vec2f(393.0f + 1108.0f -textTexture.getWidth() ,246.0f + i *(textTexture.getHeight() + 3.0f)));				
				gl::draw(textTexture);
		gl::popMatrices();				
	}
}

void EmailForm::addCurrentEmail(string _email)
{
	if (emailVector.size() >=3) return;//noMoreAdd = true;

	emailVector.push_back(_email);
	currentEmail = "";	
}

void EmailForm::draw()
{
	gl::color(Color(1.0f, 1.0f, 1.0f));

	gl::pushMatrices();
		gl::translate(bgPosition);
		gl::color(bgColor);
		gl::draw(*keyBoardMainBgTex);		
		gl::translate(0.0f, 1754.0f - 1080.0f);
		drawEmailInput();
		drawAdditionEmails();
	gl::popMatrices();

	gl::pushMatrices();
			gl::translate(0.0f, 674.0f);
			gl::translate(bgPosition);
			touchKeyBoard.draw();
	gl::popMatrices();	

	gl::pushMatrices();
		gl::translate(bgPosition);	
		gl::translate(Vec2f(0.0f, 674.0f));	
		gl::color(ColorA(1.0f, 1.0f, 1.0f, 1.0f));
		addEmailBtn->draw();
		
		gl::color(Color::white());
		gl::pushMatrices();
				gl::translate(Vec2f(393.0f, 206.0f));
				gl::draw(*emailLineTex);
		gl::popMatrices();				

		gl::color(ColorA(1.0f, 1.0f, 1.0f, 1.0f));

		if (currentEmail != "")
			deleteAllLettersBtn->draw();

		closeEmailBtn->draw();
	gl::popMatrices();	
}