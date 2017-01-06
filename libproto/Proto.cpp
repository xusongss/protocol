
#include <proto/Proto.h>

namespace Inspiry{
unsigned int AProto::getFourCC(){

return UNKNOWN_PROTO;

}
const char * AProto::getClassName(){

return NULL;	
}
const char * AProto::getProtocolName(){

return NULL;
}
status_t AProto::setConfig(sp<AMessage> config)
{
	return NOT_SUPPORTED;
}

void AProto::dump(AString &str){
	 str.append("==>\r\n");
	 str.append("	proto:	");
	 str.append(getProtocolName());
	 str.append("\r\n");
	 str.append("	class: ");
	 str.append(getClassName());
	 str.append("\r\n");
	 str.append("	FourCC: ");
	 str.append(getFourCC());
}


}// namespace Inspiry

