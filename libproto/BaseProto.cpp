#include <utils/Errors.h>
#include <foundation/ABuffer.h>
#include <foundation/ACrc16CCIT.h>
#include <foundation/AString.h>
#include <foundation/hexdump.h>
#include<proto/BaseProto.h>
#include <foundation/InspiryLog.h>
#define LOG_TAG "BaseProto"
namespace Inspiry{

BaseProto::BaseProto(sp<AProto> proto):
	mRevBytes(0), mExpectLen(-1), mStatus(status_1_55)

{
	proto->setProto(this);
	
	mLowerProto = proto;
	
	memset(mPackage, 0, sizeof(mPackage));
}
BaseProto::~BaseProto(){
}
status_t BaseProto::setProto(sp<AProto> proto)
{
	mUpProto = proto;
}

status_t BaseProto::dataReady(sp<ABuffer> data){
	AString log;
	const unsigned char * buf = data->data();
	
	int len = data->size();


	LOGV(LOG_TAG, "dataReady len %d", len);
	
	hexdump((void*)buf, len, 0, &log);
	
	LOGV(LOG_TAG,"%s", log.c_str());
	
	while(len--)
	{
		bool reset = false;
		
		switch(mStatus)
		{
			case status_1_55:
				
				if((unsigned char)0x55 == *buf){
					
					mStatus = status_FF;
				}
				else{
					LOGE(LOG_TAG, "mStatus(%d) desired(0x55) rev(0x%02hhx)!!!", mStatus, *buf);
					reset = true;
				}
				break;
			case status_FF:

				if((unsigned char)0xFF == *buf){
					
					mStatus = status_2_55;
				}
				else{
					LOGE(LOG_TAG, "mStatus(%d) desired(0xFF) rev(0x%02hhx)!!!", mStatus, *buf);

					reset = true;
				}
				
				break;
			case status_2_55:

				if((unsigned char)0x55 == *buf){
					
					mStatus = status_ver;
				}
				else{
					LOGE(LOG_TAG, "mStatus(%d) desired(0x55) rev(0x%02hhx)!!!", mStatus, *buf);

					reset = true;
				}
				
				break;
			case status_ver:

				if((unsigned char)0x01 == *buf){
					
					mStatus = status_H_LEN;
				}
				else{
					LOGE(LOG_TAG, "mStatus(%d) desired(0x01) rev(0x%02hhx)  version must(0x01)!!!", mStatus, *buf);

					reset = true;
				}
				break;

			case status_H_LEN:

				mExpectLen  = (*buf) << 8;

				mStatus = status_L_LEN;
				
				break;
			case status_L_LEN:

				mExpectLen  |=  *buf;

				mStatus = status_PACKAGE;
				
				break;
			case status_PACKAGE:
				{
					if(mExpectLen <= 0){

						LOGE(LOG_TAG, "ExpectLen(%d) is err!!!", mExpectLen);
						reset = true;
					}else{
						mPackage[mRevBytes++] = *buf;
						
						if(mRevBytes == mExpectLen + 2){

							LOGV(LOG_TAG, "dataReady get full package %d", mExpectLen);
				
							unsigned short crc = ACrc16Ccitt(mPackage+1, mRevBytes -3);
				
							if(crc != (mPackage[mRevBytes-2] << 8 | mPackage[mRevBytes-1]) )
							{
								LOGE(LOG_TAG, "crc is Incorrect 0x%x 0x%x", crc, (mPackage[mRevBytes-2] << 8 | mPackage[mRevBytes-1]));
							}
							
							sp<ABuffer> package = new ABuffer(mPackage, mRevBytes-2);
							
							
							if(mUpProto != NULL)
							{
								LOGV(LOG_TAG, "dataReady push the package to up proto");
								mUpProto->dataReady(package);
							}
							reset = true;
						}
					}		
				}
				break;
			default:
				reset = true;
				break;
				
		}
		buf++;
		if(reset)
		{
			mStatus = status_1_55;
			mExpectLen = -1;
			mRevBytes = 0;
			
		}
	}
}

}

