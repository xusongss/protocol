#include <utils/Errors.h>
#include <foundation/ABuffer.h>
#include <foundation/AMessage.h>
#include <foundation/InspiryLog.h>
#include<proto/BLEControlProto.h>

#undef LOG_TAG
#define LOG_TAG "BLEControlProto"

namespace Inspiry{

BLEControlProto::BLEControlProto(sp<AProto> proto, sp<BLEControlHandler> handler):
	mHandler(handler),
	mBaseprotoType((unsigned char) 0x02),
	mVer((unsigned char)0x01),
	mAudio((unsigned char)0x01),
	mPrompt((unsigned char)0x01),
	mStart((unsigned char)0x02),
	mSuccess((unsigned char)0x03),
	mError((unsigned char)0x04),
	length(2)
{
	proto->setProto(this);
}
BLEControlProto::~BLEControlProto(){
}
status_t BLEControlProto::dataReady(sp<ABuffer> data){

	
	const unsigned char * buf = data->data();
	
	int len = data->size();

	LOGV(LOG_TAG, "dataReady len %d", len);
	
	if(len != (length + 1)){
		
		LOGE(LOG_TAG, "length(0x%x) incorrect!!!", len);
		
		return NO_ERROR; 
		
	}
	
	if((buf[0] != mBaseprotoType) || ((buf[1] & 0x0F) != mVer)){

		LOGE(LOG_TAG, "BaseprotoType(0x%x) or VER(0x%x) is incorrect!!!", buf[0], (buf[1] & 0x0F));
		
		return NO_ERROR; 
	}
	const unsigned char type = (buf[1] & 0xF0) >> 4;
	
	if(type != mAudio)
	{
		LOGE(LOG_TAG, "type(0x%x) incorrect!!!", type);
		
		return NO_ERROR; 
	}
	const unsigned char cmd = buf[2];

	sp<AMessage> msg = new AMessage;
		
	mHandler->setControlType(msg, ControlType_Audio);
	
	mHandler->setCommand(msg, (ControlCommand_t)cmd);
	
	mHandler->handlerMessage(msg);

	
}
}

