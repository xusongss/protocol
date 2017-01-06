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
			| HEAD(3 B) | VER��1 B��| LEN(2) | TYPE(1 ) |	DATA | CRC16 |
			--------------------------------------------------------------

			Package ��������Ӧ����256�ֽ�
			HEAD�������ֽڣ��̶�ֵ 0x55 0xFF 0x55
			VER��һ���ֽڣ��汾�ţ���ǰ�汾��0x01
			LEN �������ֽڣ����ݰ����ȣ����� TYPE��DATA ��������CRC16��
			TYPE��һ���ֽڣ��������ͣ��ɵڶ��ڻ��������ĵ��涨
			DATA������������ʽ��TYPE����
			CRC16: ��DATA�ֶ�����У��
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
