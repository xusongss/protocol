#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <foundation/ABase.h>
#include <foundation/ABuffer.h>
#include <foundation/InspiryLog.h>
#include <utils/Errors.h>
#include <utils/threads.h>
#include "proto/Com.h"
#undef LOG_TAG
#define LOG_TAG "COM"
#define DEBUG 0
namespace Inspiry {
	static void hexdump(const unsigned char *buf, int num)
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


struct Com::ComInnerThread : public Thread {
    ComInnerThread(Com *com)
        : mCom(com)
    {
    }


    virtual bool threadLoop() {
       // return mLooper->loop();
       	int err = NO_ERROR;
		sp<AProto> target = NULL;
		char buffer[255];
		int length = sizeof(buffer);
		sp<ABuffer> buf = new ABuffer(buffer, length);
		err = mCom->read(buffer,&length);
		
		switch(err)
		{
			case NO_ERROR:
				hexdump((const unsigned char*)buffer, length);
				//target = mCom->mProto;
				target = mCom->getActiveProto();
				if(target != NULL)
			 	{
			 		if(DEBUG)
		 			{
		 				LOGV("ComInnerThread", "push data to up proto %d", length);
		 			}
			 		buf->setRange(0, length);
					buf->setInt32Data(mCom->mFourCC);
			 		target->dataReady(buf);
					return true;
			 	}
				break;
			case TIMED_OUT:
				return true;
				break;
			default:
				return false;
		}
		return true;
		
    }


protected:
    virtual ~ComInnerThread() {}

private:
    Com *mCom;
    //android_thread_id_t mThreadId;
    DISALLOW_EVIL_CONSTRUCTORS(ComInnerThread);
};


Com::Com(const ComConfig *config, int FourCC):
	mFourCC(FourCC), mActiveProto(UNKNOWN_PROTO)
{
	memcpy(&mConfig, config, sizeof(ComConfig));
	
	mThread = new ComInnerThread(this);
}

 status_t Com::setProto(sp<AProto> proto)
{
	mProto = proto;
	
	unsigned int fourcc = proto->getFourCC();
	
	if( fourcc != UNKNOWN_PROTO){

		ssize_t index;
		
		index = mProtos.indexOfKey(fourcc);

		if(index < 0){

			LOGD(LOG_TAG, "setProto: proto %d set in", fourcc);

			mProtos.add(fourcc, proto);
			
		}else{
			LOGD(LOG_TAG, "setProto: proto %d is in it", fourcc);
		}
		

		if(mActiveProto == UNKNOWN_PROTO){
			
			mActiveProto = fourcc;
			
		}
	}
	else{

		LOGE(LOG_TAG, "set unknown proto");
		
		return UNKNOWN_ERROR;
	}
	return NO_ERROR;
	
}
 status_t Com::setActiveProto(unsigned int fourcc){

	if(fourcc != UNKNOWN_PROTO)
	{
		ssize_t index;
		
		index = mProtos.indexOfKey(fourcc);

		if(index < 0){

			LOGE(LOG_TAG, "setActiveProto:fourcc : %d is err!!", fourcc);

			return UNKNOWN_ERROR;
		}
		
		mActiveProto = fourcc;

		return NO_ERROR;
	}

	return UNKNOWN_ERROR;
	
 }
 status_t Com::getActiveProto(unsigned int *fourcc){

	*fourcc = mActiveProto;
	
	return NO_ERROR;
 }
 sp<AProto> Com::getActiveProto(){

	if(mActiveProto != UNKNOWN_PROTO)
	{
		ssize_t index;
		
		index = mProtos.indexOfKey(mActiveProto);
		
		if(index < 0){

			LOGE(LOG_TAG, "getActiveProto : %d is err!!", mActiveProto);
			
			return NULL;
		}
		
		return mProtos.valueAt(index);
	}
	
	LOGE(LOG_TAG, "No Active Proto");
	
	return NULL;
 	
 }
 status_t Com::dataReady(sp<ABuffer> buf ){
}
 status_t Com::writeData(sp<ABuffer> buf ){
 	
 	status_t ret = 0;

	unsigned const char * pBuffer =  buf->data();

	size_t len = buf->size();
	
	const int waitTime = 500*1000;

	 do{
            struct timeval timeout={3,waitTime};
			fd_set mFdset;
            FD_ZERO(&mFdset);
            FD_SET(mHandle, &mFdset);
            ret = select(mHandle+1,NULL,&mFdset,NULL,&timeout);
            if(ret == -1){
				
                LOGE(LOG_TAG, "WriteCom error select error");
				
                return UNKNOWN_ERROR;
				
            }else if(ret == 0){
            
                LOGE(LOG_TAG, "WriteCom error time off(%d)", waitTime);
				
           		return UNKNOWN_ERROR;
				
            } else{
            
                if(!FD_ISSET(mHandle,&mFdset))
                {
                    LOGE(LOG_TAG, "WriteCom error mHandle is out of FDSET");
					
                    return UNKNOWN_ERROR;
                }
                ret = write(mHandle, pBuffer, len);
				
                if(ret < 0)
				{
                	LOGE(LOG_TAG,"WriteCom mHandle(%d) ret(%d) errno(%d) ", mHandle, ret, errno);

					return UNKNOWN_ERROR;
				}
                if(ret > 0)
                {
                     pBuffer += ret;
                     len -= ret;
                }
            }
    }while(len);

	return NO_ERROR;
}

status_t Com::start(){

	int err =NO_ERROR;
	
	err = openNoblock();
	
	if(NO_ERROR != err )
	{	
		return err;
	}
	
	mThread->run();
}
status_t Com::stop(){
	mThread->requestExitAndWait();
}
Com::~Com(){
	if(mThread->isRunning())
	{
		mThread->requestExitAndWait();
	}
	close();
}


int Com::open()
{
	mHandle = ::open(mConfig.name, O_RDWR |O_NOCTTY | O_NDELAY);
    if (mHandle < 0)
    {
        LOGE(LOG_TAG, "com %s open failed", mConfig.name);
        return -1;
    }
	setConfig(mConfig.baudrate, mConfig.bits, mConfig.stop, mConfig.parity);
}
int Com::openNoblock()
{
	mHandle = ::open(mConfig.name, O_RDWR |O_NONBLOCK |O_NOCTTY | O_NDELAY);
    if (mHandle < 0)
    {
        LOGE(LOG_TAG, "com %s open failed", mConfig.name);
        return -1;
    }
	setConfig(mConfig.baudrate, mConfig.bits, mConfig.stop, mConfig.parity);
}

int Com::close()
{
	int ret = 0;
	LOGD(LOG_TAG, "com %s closed!!!", mConfig.name);
	ret = ::close(mHandle);
	mHandle = -1;
	return ret;
}
status_t Com::read(char * buf, int *length)
{
	const int buff_len = *length;
	int ret = NO_ERROR;
	
	const int waitTime = 500*1000;
	fd_set mFdset;
	struct timeval timeout={1,waitTime};
	FD_ZERO(&mFdset);
	FD_SET(mHandle, &mFdset);
	
	ret = select(mHandle+1,&mFdset,NULL,NULL,&timeout);
	
	if(ret == -1)
	{
        LOGE(LOG_TAG, "ReadCom select error");
		
        return UNKNOWN_ERROR;
		
    }else if(ret == 0){
    
        //LOGV(LOG_TAG, "ReadCom  time off(%dus)", waitTime);
		
        return TIMED_OUT;
    }
		
	if(!FD_ISSET(mHandle,&mFdset))
    {
        LOGE(LOG_TAG, "ReadCom mHandle is out of FDSET");
        return UNKNOWN_ERROR;
    }
    ret = ::read(mHandle, buf, buff_len);
    if(ret < 0)
    {
		if(errno ==  EAGAIN)
		{
			LOGE(LOG_TAG,"ReadCom mHandle(%d) ret(%d) errno(%d EAGAIN) ", mHandle, ret, errno);
			
			usleep(150*1000);
			
			return TIMED_OUT;
		}
		else
		{
			LOGE(LOG_TAG,"ReadCom mHandle(%d) ret(%d) errno(%d) ", mHandle, ret, errno);
			
			return UNKNOWN_ERROR;
		}
    }
	if(ret > 0)
    {
    	*length = ret;
    }
	else
	{
		LOGE(LOG_TAG,"ReadCom mHandle(%d) length = 0 ", mHandle);
		*length = 0;
	}
	return NO_ERROR;
	
}

int Com::setConfig(const ComConfig *config)
{
	if(config != NULL)
	{
		memcpy(&mConfig, config, sizeof(ComConfig));
		if(mHandle > 0)
		{
			setConfig(mConfig.baudrate, mConfig.bits, mConfig.stop, mConfig.parity);
		}
		
	}
}

int Com::setConfig(int baudrate, int bits, int stop,int parity ) {
    struct termios new_cfg;
    struct termios old_cfg;
    int speed;
    if (tcgetattr(mHandle, &old_cfg) != 0)
    {
        LOGE(LOG_TAG, "tcgetattr error!!!");
        return -1;
    }
    //设置字符大小
	
    new_cfg = old_cfg;
    cfmakeraw(&new_cfg);
    new_cfg.c_cflag &= ~CSIZE;

    speed = covBaudrate(baudrate);

    cfsetispeed(&new_cfg, speed);
    cfsetospeed(&new_cfg, speed);

    //设置奇偶校验位
	
    switch (parity)
    {
        case 1://奇校验
        {
            new_cfg.c_cflag |= PARENB;
            new_cfg.c_cflag |= PARODD;
            new_cfg.c_iflag |= (INPCK | ISTRIP);
            break;
        }
        case 2://偶校验
        {
            new_cfg.c_iflag |= (INPCK | ISTRIP);
            new_cfg.c_cflag |= PARENB;
            new_cfg.c_cflag &= ~PARODD;
            break;
        }
        case 0://无奇偶校验位
        {
            new_cfg.c_cflag &= ~PARENB;
            break;
        }
        default://无奇偶校验位
        {
            new_cfg.c_cflag &= ~PARENB;
            break;
        }
    }
    //设置停止位
    switch (stop)
    {
        case 1:
        {
            new_cfg.c_cflag &= ~CSTOPB;
            break;
        }
        case 2:
        {
            new_cfg.c_cflag |= CSTOPB;
            break;
        }
        default:
        {
            new_cfg.c_cflag &= ~CSTOPB;
            break;
        }
    }
    //设置数据位
    switch (bits)
    {
        case 5:
        {
            new_cfg.c_cflag |= CS5;
            break;
        }
        case 6:
        {
            new_cfg.c_cflag |= CS6;
            break;
        }
        case 7:
        {
            new_cfg.c_cflag |= CS7;
            break;
        }
        case 8:
        {
            new_cfg.c_cflag |= CS8;
            break;
        }
        default:
        {
            new_cfg.c_cflag |= CS8;
            break;
        }
    }
    //设置等待时间和最小接收字符
    new_cfg.c_cc[VTIME] = 1;
    new_cfg.c_cc[VMIN] = 1;

    //处理未接收字符
    tcflush(mHandle, TCIFLUSH);
    //激活新配置
    if ((tcsetattr(mHandle, TCSANOW, &new_cfg)) != 0)
    {
        LOGE(LOG_TAG, "tcsetattr error!!!");
        return -1;
    }
    LOGD(LOG_TAG, "set setConfig mBaudrate(%d) mBits(%d) mStop(%d) mParity(%d)",
    		baudrate,
    		bits,
    		stop,
    		parity);
    return 0;
}

int Com::covBaudrate(int baudrate) {
    int speed;
	
    //cov波特率
	
    switch (baudrate)
    {
        case 110:
        {
            speed = B110;
            break;
        }
        case 300:
        {
            speed = B300;
            break;
        }
        case 600:
        {
            speed = B600;
            break;
        }
        case 1200:
        {
            speed = B1200;
            break;
        }
        case 2400:
        {
            speed = B2400;
            break;
        }
        case 4800:
        {
            speed = B4800;
            break;
        }
        case 9600:
        {
            speed = B9600;
            break;
        }
		
#if 0
		
        case 14400:
        {
            speed = B14400;
            break;
        }
		
#endif
		
        case 19200:
        {
            speed = B19200;
            break;
        }
        case 38400:
        {
            speed = B38400;
            break;
        }
		
#if 0

        case 56000:
        {
            speed = B56000;
            break;
        }
		
#endif
		
        case 57600:
        {
            speed = B57600;
            break;
        }
        case 115200:
        {
            speed = B115200;
            break;
        }
#if 0

        case 128000:
        {
            speed = B128000;
            break;
        }
        case 256000:
        {
            speed = B256000;
            break;
        }
#endif

        default:
        {
            speed = B57600;
            break;
        }
    }
    return speed;
}

}

