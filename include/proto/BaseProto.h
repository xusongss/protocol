#ifndef __BASEPROTO_H__
#define __BASEPROTO_H__
#include<proto/Proto.h>

namespace Inspiry{

class BaseProto :public AProto{
	
	
	public:
			BaseProto(sp<AProto> proto);
			
			virtual ~BaseProto();
			
			virtual status_t dataReady(sp<ABuffer> buf);
			
			virtual status_t setProto(sp<AProto> proto);
			
			virtual unsigned int getFourCC(){
				return 'B' <<24 | 'A' << 16 |'S'<< 8 | 'E' ; 
			}
	private:
		/*
			--------------------------------------------------------------
			| HEAD(3 B) | VER（1 B）| LEN(2) | TYPE(1 ) |	DATA | CRC16 |
			--------------------------------------------------------------

			Package 最大包长不应超过256字节
			HEAD：三个字节，固定值 0x55 0xFF 0x55
			VER：一个字节，版本号，当前版本号0x01
			LEN ：两个字节，数据包长度（包括 TYPE，DATA 但不包含CRC16）
			TYPE：一个字节，数据类型，由第二节或者其他文档规定
			DATA：不定长，格式有TYPE决定
			CRC16: 对DATA字段内容校验
		*/
		int mExpectLen;		
		int mRevBytes;
		unsigned char  mPackage[1024];
		wp<AProto> mLowerProto;
		sp<AProto> mUpProto;

		enum status{
			status_1_55,
			status_FF,
			status_2_55,
			status_ver,
			status_H_LEN,
			status_L_LEN,
			status_PACKAGE,
			
		};
		status mStatus;
			
};

}
#endif
