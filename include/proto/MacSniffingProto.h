#ifndef __MACSNIFFINGPROTO_H__
#define __MACSNIFFINGPROTO_H__
#include <utils/Errors.h>
#include <proto/Proto.h>


namespace Inspiry{

class MacSniffingProto : public AProto
{
public:
		
	class MacSniffingProtoHandler : public AProtoHandler
	{
		public:
			
			/*
				@buf the ABuffer is the struct of MacPackage
			*/
		status_t setMessage(sp<AMessage>& msg, sp<ABuffer>& buf);
		status_t getMessage(sp<AMessage>& msg, sp<ABuffer>* buf);
				virtual status_t handlerMessage(sp<AMessage>& msg);
		virtual ~MacSniffingProtoHandler();
	};
	
	public:
		MacSniffingProto(sp<AProto> proto, sp<MacSniffingProtoHandler> handler);
		~MacSniffingProto();
		virtual status_t dataReady(sp<ABuffer> buf);

		#pragma pack(push)
		#pragma pack(1)
		typedef struct MacPackage{
			unsigned char mac[6];
			signed char rssi;
			unsigned char channel;
			unsigned char stop_byte;
		}MacPackage;
		#pragma pack(pop)
	private:
		sp<MacSniffingProtoHandler> mHandler;
		enum status
		{
			status_68,
			status_ff1,
			status_ff2,
			status_ff3,
			status_ff4,
			status_package
		};
		status mStatus;
		char mPackage[sizeof(MacPackage)];
		int mPackageBytes;
};
	
}

#endif

