#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <foundation/ABuffer.h>
#include <foundation/InspiryLog.h>
#include <proto/Com.h>
#include <proto/MacSniffingProto.h>
#include<proto/Proto.h>
#include<proto/BaseProto.h>
#include<proto/BLEControlProto.h>


#undef LOG_TAG
#define LOG_TAG "MacSniffingLisener"

using namespace Inspiry;
/*
class MacSniffingLisener : public ALisener{
	virtual status_t dataReady(sp<ABuffer> buf )
	{
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
	virtual ~MacSniffingLisener(){
		LOGD(LOG_TAG, "MacSniffingLisener is deleted");
		}
};*/
int main()
{
	int i = 1000000;
	
	ComConfig config;
	
	sp<Com> mCom;
	
	sp<MacSniffingProto> sniffing;
	
	sp<BaseProto> mBaseProto;
	sp<BLEControlProto> mBLEProto;
	
	//sp<MacSniffingLisener> lisener;
	//strcpy(config.name, "/dev/ttyUSB0");
	strcpy(config.name, "/dev/ttyS0");
	config.baudrate = 9600;
	config.bits = 8;
	config.stop = 1;
	config.parity = 0;

	mCom = new Com(&config,  'U'<<24 | 'S' << 16 | 'B' << 8 | '0');
	#if 0
	sniffing = new MacSniffingProto(mCom, new MacSniffingProto::MacSniffingProtoHandler);
	#endif
	mBaseProto = new BaseProto(mCom);
	mBLEProto = new BLEControlProto(mBaseProto, new BLEControlHandler);
	
	mCom->start();
	while(i--){
		sleep(1);
	}

	
	return 0;
}
