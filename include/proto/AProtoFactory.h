#ifndef __APROTO_FACTORY_H__
#define __APROTO_FACTORY_H__
#include <proto/Proto.h>
namespace Inspiry{

#define APROTO_AUTO_ADD_TO_FACTORY(classname, protoname, fourcc, create_fun)\
static unsigned int inner_##classname##_##protoname_fourcc = (fourcc);\
static const char * inner_##classname##_##protoname_name = #protoname;\
class inner_##classname##_##protoname{\
	public:\
	inner_##classname##_##protoname(){\
		AProtoFactory::registerProtocol((#protoname),(fourcc), (create_fun));\
	};\
};\
static inner_##classname##_##protoname inner_##classname##_##protoname_instance;\
unsigned int  classname::getFourCC(){\
	return (inner_##classname##_##protoname_fourcc);\
}\
const char * classname::getClassName(){\
	return #classname;\
}\
const char * classname::getProtocolName(){\
	return #protoname;\
}

class AProtoFactory{

public:
	typedef sp<ATProto> (*create_protocol_t)(sp<AProto> proto);
	
	static status_t registerProtocol(const char *name, unsigned int fourcc, create_protocol_t fun){};
private:
	static sp<ATProto> create(sp<AProto> proto, unsigned int fourcc){};
};

}

#endif
