#include <utils/Errors.h>
#include <foundation/ABuffer.h>
#include <foundation/AMessage.h>
#include <foundation/InspiryLog.h>
#include <proto/MacSniffingProto.h>


#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "MacSniffingProto"

namespace Inspiry{
void hexdump(const unsigned char *buf, int num)
{
#if 0
	int i;

	for(i = 0; i < num; i++) 
	{
		printf("0x%02hhx ", buf[i]);
		if ((i+1)%8 == 0) 
		{
			printf("\n");
		}
	}
	printf("\n");
#endif
}
static int onPackageAvailable(MacSniffingProto::MacPackage * package)
{
	LOGD(LOG_TAG, "mac 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx", 
			package->mac[0],
			package->mac[1],
			package->mac[2],
			package->mac[3],
			package->mac[4],
			package->mac[5]
			);
}
status_t MacSniffingProto::MacSniffingProtoHandler::setMessage(sp<AMessage>& msg, sp<ABuffer>& buf){
	msg->setBuffer("ABuffer", buf);
}
status_t MacSniffingProto::MacSniffingProtoHandler::getMessage(sp<AMessage>& msg, sp<ABuffer>* buf){
	msg->findBuffer("ABuffer", buf);
}

status_t MacSniffingProto::MacSniffingProtoHandler::handlerMessage(sp<AMessage>& msg){
	sp<ABuffer> buf;
	getMessage(msg, &buf);
	const MacSniffingProto::MacPackage * package = (MacSniffingProto::MacPackage *)buf->data();
		
		LOGD(LOG_TAG, "mac 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx 0x%02hhx", 
			package->mac[0],
			package->mac[1],
			package->mac[2],
			package->mac[3],
			package->mac[4],
			package->mac[5]
			);
}
MacSniffingProto::MacSniffingProtoHandler::~MacSniffingProtoHandler(){
}

MacSniffingProto::MacSniffingProto(sp<AProto> proto ,sp<MacSniffingProtoHandler> handler):
	mPackageBytes(0),mStatus(status_68), mHandler(handler)
{
	proto->setProto(this);
}
MacSniffingProto::~MacSniffingProto(){
}


status_t MacSniffingProto::dataReady(sp<ABuffer> data){
	const unsigned char * buf = data->data();
	int len = data->size();
	hexdump(buf, len);
	while(len--)
	{	
		switch(mStatus)
		{
			case status_68:
				if( (unsigned char)0x68 == *buf )
				{
					
					mStatus = status_ff1;
				}
				break;
			case status_ff1:
				if((unsigned char)0xff == *buf)
				{
					mStatus = status_ff2;
				}
				else
				{
					mStatus = status_68;
				}
				break;
			case status_ff2:
				if((unsigned char)0xff == *buf)
				{
					mStatus = status_ff3;
				}
				else
				{
					mStatus = status_68;
				}
				break;
			case status_ff3:
				if((unsigned char)0xff == *buf)
				{
					mStatus = status_ff4;
				}
				else
				{
					mStatus = status_68;
				}
				break;
			case status_ff4:
				if((unsigned char)0xff == *buf)
				{
					mStatus = status_package;
				}
				else
				{
					mStatus = status_68;
				}
				break;
			case status_package:
				mPackage[mPackageBytes] = *buf;
				mPackageBytes++;
				if(mPackageBytes == sizeof(MacPackage))
				{
					hexdump((unsigned char *)mPackage, mPackageBytes);
					//onPackageAvailable((MacPackage*)mPackage);
					
					if(mHandler != NULL)
					{	
						sp<AMessage> msg = new AMessage;
						sp<ABuffer> package = new ABuffer(&mPackage, mPackageBytes);
						package->setRange(0, sizeof(MacPackage));
						mHandler->setMessage(msg, package);
						mHandler->handlerMessage(msg);
					}
					
					mStatus = status_68;
					mPackageBytes = 0;
				}
				break;
		}
		buf++;
	}
	return NO_ERROR;
}

}


