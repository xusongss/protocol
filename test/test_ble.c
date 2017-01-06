#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <proto/InspiryBLEControl.h>



#define LOG_TAG "BLE"

static void* BLEContorlCb(ControlType_t type, ControlCommand_t cmd)
{
	printf("---ERV  type(%d) cmd(%d)\n", type, cmd);
}


int main(int argc, const char * argv[])
{
	if(argc < 3)
	{
		printf("test_ble device  baudrate\n");
		
		return 0;
	}

	SerialConfig_t config;
	strcpy(config.name, argv[1]);
	config.baudrate = atoi(argv[2]);
	config.bits = 8;
	config.stop = 1;
	config.parity = 0;

	printf("start\n");
restart:
	
	if(InspiryBLEControlInit(&config, BLEContorlCb) != 0){

		printf("InspiryBLEControlInit err!!!");

		return 0;
	}
	
	int i = 1000000;
	
	while(i--){
		
		sleep(1);
	}
	
	InspiryBLEControlUnInit();
	
	sleep(3);
	
	goto restart;

	
	return 0;
}
