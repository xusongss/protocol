#include <utils/Errors.h>
#include <foundation/ABuffer.h>
#include <foundation/AMessage.h>
#include <foundation/AHandler.h>
#include <proto/BLEControlHandler.h>
#include "foundation/InspiryLog.h"

#undef LOG_TAG
#define LOG_TAG "BLEControlHandler"
namespace Inspiry{

BLEControlHandler::BLEControlHandler():
	mCb(NULL)
{
}
BLEControlHandler::~BLEControlHandler(){
	}

void BLEControlHandler::setControlType(sp<AMessage>& msg, ControlType_t type){
	msg->setWhat(type);
	//msg->setTarget(id());
}
void BLEControlHandler::setCommand(sp<AMessage>& msg, ControlCommand_t cmd){
	msg->setInt32("CMD", cmd);
	//msg->setTarget(id());
}
void BLEControlHandler::setCallback(BLEControlHandlerCallBack cb){
	mCb = cb;
}

status_t BLEControlHandler::handlerMessage(sp<AMessage>& msg){
	 switch (msg->what()) 
	 	{
	 		case ControlType_Audio:
				{
					int cmd;
					
					msg->findInt32("CMD", &cmd);

					if(mCb != NULL){
						mCb(ControlType_Audio, (ControlCommand_t)cmd);
					}
					LOGV(LOG_TAG, "audio cmd %d", cmd);
						
				}
			break;
			case ControlType_LED:
			break;
			default:
				break;
	 	}
}


}


