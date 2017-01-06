
#include <proto/ATProto.h>
#include <foundation/ALooper.h>
#include <foundation/AMessage.h>
#include <foundation/ABuffer.h>
#include <foundation/InspiryLog.h>
#include <utils/Errors.h>
#include <foundation/AString.h>
#include <foundation/hexdump.h>
#include <proto/AProtoFactory.h>

#undef LOG_TAG
#define LOG_TAG "ATProto"

namespace Inspiry{

static sp<ATProto> create_instance(sp<AProto> proto){
	if(proto == NULL){
		
		LOGE(LOG_TAG, "create_instance err!!!");
	}
	return  new ATProto(proto);
}

/*
	'A' 'T' 'I' 'D'
*/
APROTO_AUTO_ADD_TO_FACTORY(ATProto, AT,  'A'<<24 | 'T' << 16 | 'I' << 8 | 'D' , create_instance);



ATProto::ATProto(sp<AProto> proto):
	mStatus(status_1_0D), mRevBytes(0), mMaxDataLength(sizeof(mData))
{
/*
	mHandler = new ATProtoHandler;
	
	looper->registerHandler(mHandler);*/
	
	proto->setProto(this);
	
	mLowerProto = proto;
}
status_t ATProto::dataReady(sp<ABuffer> data){
	
	const unsigned char * buf = data->data();
	
	int len = data->size();

	static int cl = 0;
	static int indicator = 0;
	while(len--)
	{
		LOGD(LOG_TAG, "0x%x", *buf);
		switch(mStatus)
		{
			case status_1_0D:
				if( (*buf) == (unsigned char)0x0D ){
					mStatus = status_1_0A;
				}
				break;
			case status_1_0A:
				if( (*buf) == (unsigned char)0x0A ){
					mStatus = status_2_0D;
					cl = 0;
					indicator = mRevBytes;
				}else{
					mStatus = status_1_0D;
				}
				break;
			case status_2_0D:
				if( (*buf) == (unsigned char)0x0D){
					
					mStatus = status_2_0A;
					
				} 
				else{
						if(mRevBytes >= mMaxDataLength){
							
							LOGE(LOG_TAG, "Fuck!!! check the unreliable data!!!");
							
							AString log;
							
							hexdump(mData, mMaxDataLength, 0, &log);

							LOGE(LOG_TAG, "%s", log.c_str());

							mRevBytes = 0;
							
							cl = 0;
							
							indicator = 0;
						}

						
						mData[mRevBytes++] = *buf;
						
						cl++;
						
						
				}
				break;
			case status_2_0A:
				if((*buf) == (unsigned char)0x0A){
					
					mData[mRevBytes++] = 0;
					
					cl++;
					
					LOGD(LOG_TAG, "Recv Effective(%dB) ALL(%dBs) MAX(%dBs)", cl, mRevBytes, mMaxDataLength);
					
					LOGD(LOG_TAG, "%s", mData+indicator);
					//sp<ABuffer> abuf(, cl);
					AString log;

					hexdump(&mData[indicator], cl, 0, &log);
					
					LOGD(LOG_TAG, "%s", log.c_str());

					if(strcmp((const char*)mData+indicator, "ERROR") == 0){
						LOGD(LOG_TAG, "return ERROR");
						
					}else if(strcmp((const char*)mData+indicator, "OK") == 0){

						LOGD(LOG_TAG, "return OK");
					}
					
					mStatus = status_1_0D;
					
				}else if((*buf) == (unsigned char)0x0D){	

					mData[mRevBytes++] = 0x0D;
					cl++;
				}
				else{
					mData[mRevBytes++] = 0x0D;
					cl++;

					mData[mRevBytes++] = *buf;
					cl++;
					mStatus = status_2_0D;
				}
				break;
		}
		buf++;
	}
	
}

status_t ATProto::writeData(sp<ABuffer> buf){
	
	sp<AProto> lowProto = mLowerProto.promote();

	if(lowProto != NULL){
		
		return lowProto->writeData(buf);
	
	}
	else{
		return UNKNOWN_ERROR;
	}
}


}
