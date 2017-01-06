#ifndef _COM_H__
#define _COM_H__
#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <proto/Proto.h>
#include <utils/KeyedVector.h>


namespace Inspiry{

//class AProto;

#define COM_NAME_LEN (128)

struct ComConfig
{
		char name[COM_NAME_LEN];
		int baudrate;
		int bits;
		int stop;
		int parity;
};

class Com  :public AProto
{

public:
    //Com();
	Com(const ComConfig *config, int FourCC = 0);
	~Com();
	/*
	int open();
	int openNoblock();
    int close();
    int isOpen();
	int getHandle();

	
	void getDefaultConfig(Config *config);
    
    */
    /**/
	
	int setConfig(const ComConfig *config);
	virtual status_t start();
	virtual status_t stop();
	virtual status_t setProto(sp<AProto> proto);
	virtual status_t dataReady(sp<ABuffer> buf );
	virtual status_t writeData(sp<ABuffer> buf );
	virtual status_t setActiveProto(unsigned int fourcc);
	virtual status_t getActiveProto(unsigned int *fourcc);
	
private:
	private:
	int open();
	int openNoblock();
	int close();
	int setConfig(int baudrate, int bits, int stop,int parity);
	int covBaudrate(int baudrate);
	status_t read(char * buf, int *length);
	sp<AProto> getActiveProto();
private:
	
	sp<AProto> mProto;
	
	KeyedVector<uint32_t, sp<AProto> > mProtos;
	
	unsigned int mActiveProto;
	
	int mHandle;
	
	ComConfig mConfig;
	
	struct ComInnerThread;
	
    sp<ComInnerThread> mThread;
	
	int mFourCC;
	
};

}
#endif

