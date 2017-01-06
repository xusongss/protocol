#ifndef __BLE_CONTROL_HANDLER_H__
#define __BLE_CONTROL_HANDLER_H__

#include<proto/Proto.h>
#include<proto/InspiryBLEControl.h>


namespace Inspiry{

class BLEControlHandler: public AProtoHandler{
	public:
		BLEControlHandler();
		virtual ~BLEControlHandler();
		/*
		enum ControlType{
			ControlType_Audio,
			ControlType_LED,
		};
		enum ControlCommand{
			
			AudioCommand_Prompt = 0x01,
			AudioCommand_Start,
			AudioCommand_Success, 
			AudioCommand_Error,
		};
		typedef void* (*BLEControlHandlerCallBack)(int type, int cmd);
		*/
		void setControlType(sp<AMessage>& msg, ControlType_t type);
		void setCommand(sp<AMessage>& msg, ControlCommand_t cmd);
		virtual status_t handlerMessage(sp<AMessage>& msg);
		void setCallback(BLEControlHandlerCallBack cb);
	private:
		BLEControlHandlerCallBack mCb;
};
}
#endif

