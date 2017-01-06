#ifndef A_PROTO_H__
#define A_PROTO_H__
#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Vector.h>
#include <foundation/AString.h>

namespace Inspiry{
struct ABuffer;
struct AMessage;
//#define DEBUG 1
#define UNKNOWN_PROTO  ('U' <<24 | 'N' << 16 | 'D' << 8 | 'F')									\
	
#define APROTO_AUTO_DECLARE() \
		virtual unsigned int getFourCC();\
		virtual const char * getClassName();\
		virtual const char * getProtocolName(); 


class AProto : public RefBase{

public:
	virtual status_t dataReady(sp<ABuffer> buf ){
		
		return NOT_SUPPORTED;
		
	};
	virtual status_t writeData(sp<ABuffer> buf ){
		
		return NOT_SUPPORTED;
		
	}
	virtual status_t setProto(sp<AProto> proto){
		
		return NOT_SUPPORTED;
		
	}
	virtual status_t setConfig(sp<AMessage> config);
	
	APROTO_AUTO_DECLARE();
	
	void dump(AString &str);
	
	virtual ~AProto(){};
	
public:
	static sp<AProto> create(unsigned int fourcc);
	
private:
	static Vector<sp<AProto> >mBasket;
	
	
};
class AProtoHandler:public RefBase{
	
public:
	virtual status_t handlerMessage(sp<AMessage>& msg) = 0;
	virtual ~AProtoHandler(){};
	
};

}// namespace Inspiry
#endif
