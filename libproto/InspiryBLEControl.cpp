#include <proto/InspiryBLEControl.h>
#include <proto/Com.h>
#include <proto/BaseProto.h>
#include <proto/BLEControlProto.h>
#include <foundation/InspiryLog.h>
#undef LOG_TAG
#define LOG_TAG "nspiryBLEControlForC"

class InnerClass{
public:
	int init(SerialConfig_t *config, BLEControlHandlerCallBack cb);
	int unInit();
private:
	Inspiry::sp<Inspiry::Com> mCom;
};

static InnerClass * proxy = NULL;

int InnerClass::init(SerialConfig_t *config, BLEControlHandlerCallBack cb)
{
	if(config == NULL || cb == NULL){
		
		LOGE(LOG_TAG, " config or cb is null!!!");
		return -1;
	}
	
	Inspiry::ComConfig conf;

	strncpy(conf.name, config->name, sizeof(conf.name) - 1);
	conf.baudrate = config->baudrate;
	conf.bits = config->bits;
	conf.stop = config->stop;
	conf.parity = config->parity;
	LOGD(LOG_TAG, "dev(%s) baudrate(%d) bits(%d) stop(%d) parity(%d)",
			conf.name,
			conf.baudrate,
			conf.bits,
			conf.stop,
			conf.parity);
	
	mCom = new Inspiry::Com(&conf);

	if(mCom == NULL){
		
		LOGE(LOG_TAG, " Com  init error!!!");
		return -1;
	}

	Inspiry::sp<Inspiry::BaseProto> mBaseProto;
	
	mBaseProto = new Inspiry::BaseProto(mCom);

	if(mBaseProto == NULL){
		
		LOGE(LOG_TAG, " BaseProto  init error!!!");
		
		return -1;
	}
	Inspiry::sp<Inspiry::BLEControlHandler> mHandler;
	
	Inspiry::sp<Inspiry::BLEControlProto> mBLEProto;
	
	mHandler= new Inspiry::BLEControlHandler;

	if(mHandler == NULL){
		
		LOGE(LOG_TAG, " BLEControlHandler  init error!!!");
		
		return -1;
	}
	mHandler->setCallback(cb);
	
	mBLEProto = new Inspiry::BLEControlProto(mBaseProto, mHandler);


	if(mBLEProto == NULL){
		
		LOGE(LOG_TAG, " mBLEProto  init error!!!");
		
		return -1;
	}

	mCom->start();
	
	return 0;
	
}
int InnerClass::unInit()
{
	mCom->stop();
}

int InspiryBLEControlInit(SerialConfig_t *config, BLEControlHandlerCallBack cb)
{
	
	if(proxy != NULL)
	{
		InspiryBLEControlUnInit();
	}
	proxy = new InnerClass();

	proxy->init(config, cb);
	
}
int InspiryBLEControlUnInit()
{
	proxy->unInit();
	delete proxy;
	proxy = NULL;
	return 0;
}

