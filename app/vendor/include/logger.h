#ifndef VENDOR_LOGGER_H
#define VENDOR_LOGGER_H
		typedef enum{header,error,info,debug,warning,unknown,none}event_tag_t;
		typedef enum{RXD,TXD}ttl_tag_t;
		void clear();
		void logger_init();
		void logger(int tag,char*msg);
		void TTL(int tag,char*hex);
#endif
