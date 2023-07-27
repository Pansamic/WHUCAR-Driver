#ifndef _IRS_H_
#define _IRS_H_
#ifdef __cplusplus
extern "C"{
#endif
#include <stdint.h>
#include <stdbool.h>
#define IRS_slave_addr 0x4E

typedef struct
{
    uint8_t IRS_data[8];
}IRS_t;

extern IRS_t IRS_dev;
void IRS_Update(void);


#ifdef __cplusplus
}
#endif
#endif
