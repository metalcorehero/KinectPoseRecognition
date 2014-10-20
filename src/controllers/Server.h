#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Json.h"
#include "Curl.h"
#include "cinder/ImageIo.h"
#include "cinder/Base64.h"
#include <boost/thread.hpp>
#include "Params.h"

class Server
{
	public:	
		static Server& getInstance() { static Server _server; return _server; };

		void										checkConnection();
		void										sendPhoto(ci::fs::path path);
		void										sendToMail(std::string mails);

		void										reset();
		std::string									getBuffer(){return buffer;};
		std::string									getLink(){return link;};			

		bool										isConnected, isPhotoLoaded;
		bool										isEmailSent;

		void										stopTimeout();
		int											getTimeoutSeconds();
		bool										timerIsRunning();
		void										abortLoading();

		boost::signals2::signal<void(void)>			serverLoadingPhotoEvent;
		boost::signals2::signal<void(void)>			serverCheckConnectionEvent;
		boost::signals2::signal<void(void)>			sendToMailEvent;

	private:

		std::shared_ptr<boost::thread>				sendPhotoThread;
		void										sendPhotoThreadHandler( );		
	
		std::shared_ptr<boost::thread>				checkConnectionThread;
		void										checkConnectionThreadHandler();
		

		std::shared_ptr<boost::thread>				sendToMailThread;
		void										sendToMailThreadHandler(std::string emailVector);
		

		std::string									buffer, link;
		ci::fs::path								photoPath;
		std::string									sessionId;

		ci::Timer									serverWaitingTimer;
		bool										isPhotoSendingToServer, isCheckingConnection, isMailSending;
};

inline Server&	server() { return Server::getInstance(); };