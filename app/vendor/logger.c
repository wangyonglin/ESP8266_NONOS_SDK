#include "../../app/include/driver/oled.h"

#include "mem.h"
#include "string.h"
#include "stdarg.h"
#include "osapi.h"
#include "logger.h"
void clear(){
	OLED_Clear();
}
void logger_init()
{
	OLED_Init();
}
//error,info,debug,warning,none
void logger(int tag,char*msg)
{
	switch (tag)
	{
			case header:
				OLED_Header_Clear();
				OLED_ShowString(0,0,msg);
				break;
			case error:
				OLED_Body_Clear();
				OLED_ShowString(0,2,"error:");
				OLED_ShowString(48,2,msg);
				break;
			case info:
				OLED_Body_Clear();
				OLED_ShowString(0,2,"info:");
				OLED_ShowString(40,2,msg);
				break;
			case debug:
				OLED_Body_Clear();
				OLED_ShowString(0,2,"debug:");
				OLED_ShowString(48,2,msg);
				break;
			case warning:
				OLED_Body_Clear();
				OLED_ShowString(0,2,"warning:");
				OLED_ShowString(49,2,msg);
				break;
			case unknown:
				OLED_Body_Clear();
				OLED_ShowString(0,2,"unknown:");
				OLED_ShowString(40,2,msg);
				break;
			case none:
				OLED_Body_Clear();
				OLED_ShowString(0,2,msg);
				break;
	}



}

void TTL(int tag,char*hex){

	switch (tag)
		{
		case RXD:
			OLED_Body_Clear();
			OLED_ShowString(0,2,"RXD:");
			OLED_ShowString(40,2,hex);
			break;
		case TXD:
			OLED_Body_Clear();
			OLED_ShowString(0,2,"TXD:");
			OLED_ShowString(40,2,hex);
			break;
		}
}
