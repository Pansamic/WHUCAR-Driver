#include "tv_uart.h"
#include <hw_memmap.h>
#include <stdint.h>

uint32_t IOTimeOut = 5000;//ͨ�ų�ʱ
uint8_t wBuf[128];
uint8_t wLen = 0;

//UART �����ݻ�����
uint8_t uartBuf[128];
int head = 0;
int tail  = 0;

void Uart_Flush(void)
{
	head = tail = 0;
}

int16_t Uart_Read(void)
{
	if(head!=tail){
		uint8_t Data = uartBuf[head];
		head =  (head+1)%128;
		return Data;
	}else{
		return -1;
	}
}
//UART �������ݽӿ�
int readSCS(unsigned char *nDat, int nLen)
{
	int Size = 0;
	int ComData;
	uint32_t t_user = 0;
	while(1){
		ComData = Uart_Read();
		if(ComData!=-1){
			if(nDat){
				nDat[Size] = ComData;
			}
			Size++;
			t_user = 0;
		}
		if(Size>=nLen){
			break;
		}
		t_user++;
		if(t_user>IOTimeOut){
			break;
		}
	}
	return Size;
}
//UART �������ݽӿ�
int writeSCS(unsigned char *nDat, int nLen)
{
	while(nLen--){
		if(wLen<sizeof(wBuf)){
			wBuf[wLen] = *nDat;
			wLen++;
			nDat++;
		}
	}
	return wLen;
}

//�����л���ʱ
void SCSDelay(void)
{
	uint8_t i = 30;
	while(i--){}//0.056*i(us)
}

//���ջ�����ˢ��
void rFlushSCS()
{
	SCSDelay();
	Uart_Flush();
}

//���ͻ�����ˢ��
void wFlushSCS()
{
	if(wLen){
		UART_Transmit(FEETCH_UART,wBuf, wLen);
		wLen = 0;
	}
}

