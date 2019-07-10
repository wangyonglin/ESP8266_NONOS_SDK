#include "memhex.h"
#include "mem.h"
#include "string.h"

#include "osapi.h"
void memhex(char* dest,const unsigned char* source, int len)
{
	char*temp=(char*)os_malloc(len*2);
	 short i;
	    unsigned char highByte, lowByte;

	    for (i = 0; i < len; i++)
	    {
	        highByte = source[i] >> 4;
	        lowByte = source[i] & 0x0f ;

	        highByte += 0x30;

	        if (highByte > 0x39)
	        	temp[i * 2] = highByte + 0x07;
	        else
	        	temp[i * 2] = highByte;

	        lowByte += 0x30;
	        if (lowByte > 0x39)
	        	temp[i * 2 + 1] = lowByte + 0x07;
	        else
	        	temp[i * 2 + 1] = lowByte;
	    }
	    os_strncpy(dest,temp,len*2);
	    os_free(temp);
	    return ;

}
