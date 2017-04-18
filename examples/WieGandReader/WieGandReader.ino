#include <stdint.h>
#include <WieGand.hpp>

//define a wiegand with d0 = 2, d1 = 3, maximum 8bytes (64bits) data received)
typedef WieGand<2, 3, 8> WieGandIn;

//define a wiegand with d0 = 18, d1 = 19, maximum 8bytes (64bits) data received)
typedef WieGand<18, 19, 8> WieGandOut;

//also need call INIT_WIEGAND
INIT_WIEGAND(2, 3, 8);
INIT_WIEGAND(18, 19, 8);

void setup()
{
	Serial.begin(9600);
	while(!Serial)
	{
		;
	}
	
	WieGandIn::begin();
    WieGandOut::begin();
}

void loop()
{
	if(WieGandIn::available() && WieGandIn::getBitCount() == 64)
	{
		const uint8_t* ptr = WieGandIn::getBuffer();
		
		uint16_t AgencyCode = *(uint16_t*)ptr;
        ptr += 2;
        uint16_t SystemCode = *(uint16_t*)ptr;
        ptr += 2;
        uint32_t CredentialCode = *(uint32_t*)ptr;
        CredentialCode = CredentialCode >> 8;
        ptr += 3;
		
		Serial.println("WieGandIn:");
        Serial.print("AgencyCode:");
        Serial.println(AgencyCode, DEC);
		
        Serial.print("SystemCode:");
        Serial.println(SystemCode, DEC);
		
        Serial.print("CredentialCode:");
        Serial.println(CredentialCode, DEC);

        WieGandIn::reset();
	}
	
	if(WieGandOut::available() && WieGandOut::getBitCount() == 64)
	{
		const uint8_t* ptr = WieGandOut::getBuffer();
		
		uint16_t AgencyCode = *(uint16_t*)ptr;
        ptr += 2;
        uint16_t SystemCode = *(uint16_t*)ptr;
        ptr += 2;
        uint32_t CredentialCode = *(uint32_t*)ptr;
        CredentialCode = CredentialCode >> 8;
        ptr += 3;
		
		Serial.println("WieGandOut:");
        Serial.print("AgencyCode:");
        Serial.println(AgencyCode, DEC);
		
        Serial.print("SystemCode:");
        Serial.println(SystemCode, DEC);
		
        Serial.print("CredentialCode:");
        Serial.println(CredentialCode, DEC);

        WieGandOut::reset();
	}
}
