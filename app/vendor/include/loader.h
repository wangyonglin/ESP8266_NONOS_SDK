#ifndef VENDOR_LOADER_H
#define VENDOR_LOADER_H

typedef void(*T)(void *);

typedef struct {
	T   fun;
    void*   arg;
} loader_cb_t;


//��������
typedef void (*Recv_Uart_Callback)(uint8* pData_buf, uint16 data_len);
//ע��ص�����
void funUart_ReadBack_Register(Recv_Uart_Callback callBack);

#endif
