#ifndef __INSPIRY_BLE_CONTROL_H__
#define __INSPIRY_BLE_CONTROL_H__

#include <proto/Inspirydefs.h>

#ifdef __cplusplus 
extern "C" { 
#endif


typedef enum {
	ControlType_Audio,
	ControlType_LED,
}ControlType_t;
typedef enum {
	
	AudioCommand_Prompt = 0x01,
	AudioCommand_Start,
	AudioCommand_Success, 
	AudioCommand_Error,
}ControlCommand_t;

typedef void* (*BLEControlHandlerCallBack)(ControlType_t type, ControlCommand_t cmd);

int InspiryBLEControlInit(SerialConfig_t *config, BLEControlHandlerCallBack cb);
int InspiryBLEControlUnInit();


#ifdef __cplusplus 
} 
#endif


#endif
