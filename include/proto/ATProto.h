#ifndef __AT_PROTO_H__
#define __AT_PROTO_H__
#include <foundation/AHandler.h>
#include <utils/Mutex.h>
#include <utils/Vector.h>
#include <proto/Proto.h>
namespace Inspiry{

class ALooper;
class AHandler;

class ATProto :public AProto{
		
	public:
		ATProto(sp<AProto> proto);
		
		virtual ~ATProto(){};
		
		virtual status_t dataReady(sp<ABuffer> buf);

		virtual status_t writeData(sp<ABuffer> buf);
		
		APROTO_AUTO_DECLARE();
		
private:
		wp<AProto> mLowerProto;
		enum status{
			status_1_0D,
			status_1_0A,
			status_REV,
			status_2_0D,
			status_2_0A,
			
		};
		status mStatus;
		unsigned char mData[1024];
		const int mMaxDataLength;
		int mRevBytes;
};

}

#endif
