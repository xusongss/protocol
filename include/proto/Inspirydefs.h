#ifndef __INSPIRY_DEFS_H__
#define __INSPIRY_DEFS_H__

#ifdef __cplusplus 
extern "C" { 
#endif

/*
typedef enum {
	OK                = 0,    // Everything's swell.
    NO_ERROR          = 0,    // No errors.
    
}status_t;
*/
#define SERIAL_DEV_NAME_LEN (32)

typedef struct{
	char name[SERIAL_DEV_NAME_LEN];
	int baudrate;
	int bits;
	int stop;
	int parity;

}SerialConfig_t;
#pragma pack(push)
#pragma pack(1)
typedef struct MacPackage{
	unsigned char mac[6];
	signed char rssi;
	unsigned char channel;
	unsigned char stop_byte;
}MacPackage_t;
#pragma pack(pop)



#ifdef __cplusplus 
} 
#endif

#endif

