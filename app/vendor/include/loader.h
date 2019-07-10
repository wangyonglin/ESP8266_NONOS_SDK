#ifndef VENDOR_LOADER_H
#define VENDOR_LOADER_H

typedef void(*T)(void *);

typedef struct {
	T   fun;
    void*   arg;
} loader_cb_t;


//声明方法
typedef void (*Recv_Uart_Callback)(uint8* pData_buf, uint16 data_len);
//注册回调函数
void funUart_ReadBack_Register(Recv_Uart_Callback callBack);

#endif
