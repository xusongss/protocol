#ifndef __BLE_CONTROL_PROTO_H__
#define __BLE_CONTROL_PROTO_H__
#include<proto/Proto.h>
#include<proto/BLEControlHandler.h>


namespace Inspiry{
class BLEControlProto:public AProto{
	public:
			BLEControlProto(sp<AProto> proto, sp<BLEControlHandler> handler);
			virtual ~BLEControlProto();
			virtual status_t dataReady(sp<ABuffer> buf);
	private:
		/*
			对应 baseproto TYPE=0x02
			
			ver :=0x01
			audio := 0x01
			prompt := 0x01
			start := 0x02
			success := 0x03
			error := 0x04
			Package结构：
			-------------------------------------
			| TYPE VER (1) |  COMMAND (1 BYTE)	|
			-------------------------------------
			TYPE：四个bit，数据类型，取值audio
			VER：四个bit，版本号，当前版本号0x01
			COMMAND ：一个字节， 取值 prompt	| start | success |error 
		
			Package := TYPE  VER COMMAND 
			TYPE := audio
			VER := ver
			COMMAND := prompt  | start | success |error 
		*/
		const unsigned char  	mBaseprotoType;
		const unsigned char 	mVer;
		const unsigned char 	mAudio; 
		const unsigned char 	mPrompt;
		const unsigned char 	mStart;
		const unsigned char 	mSuccess;
		const unsigned char 	mError;
		const int length;
		sp<BLEControlHandler> mHandler;
		
};
}
#endif
