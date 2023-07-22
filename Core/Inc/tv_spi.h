#ifndef _TV_SPI_H_
#define _TV_SPI_H_
#ifdef __cplusplus
extern "C"{
#endif

void TV_SPI_Init(void);
void TV_SPI_W(uint8_t uc_SendData);
void TV_SPI_R(uint8_t uc_RecvData);



#ifdef __cplusplus
}
#endif
#endif