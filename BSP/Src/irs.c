#ifdef __cplusplus
extern "C"{
#endif
#include <irs.h>
#include <tv_i2c.h>
#include <math.h>
#include <main.h>

// #define IRS_WriteByte(Reg,Value)\
//     I2C2_WriteByte(IRS_slave_addr,Reg,Value)
// #define IRS_Write(Reg,pData,Length)\
//     I2C2_Write(IRS_slave_addr,Reg,pData,Length)
// #define IRS_ReadByte(Reg)\
//     I2C2_ReadByte(IRS_slave_addr,Reg)
// #define IRS_Read(Reg,pBuf,Length)\
//     I2C2_Read(IRS_slave_addr,Reg,pBuf,Length)

// void IRS_Init(void)
// {
    
// }

void IRS_Update(void){
    // IRS_WriteByte(0x9E,0xB0);
    // IRS_Read(0x9F,IRS_dev.IRS_data,8);
    
	uint8_t i;
	for (i = 0; i < 8; ++i) {
		GPIOPinWrite(IR_SENSOR_GPIO_Port, IR_SENSOR_CLK_Pin, ~IR_SENSOR_CLK_Pin);
        SysCtlDelay(120*5);
		IRS_dev.IRS_data[i]=(GPIOPinRead(IR_SENSOR_GPIO_Port, IR_SENSOR_DAT_Pin));
		GPIOPinWrite(IR_SENSOR_GPIO_Port, IR_SENSOR_CLK_Pin, IR_SENSOR_CLK_Pin);
        SysCtlDelay(120*5);
	}
}


    
#ifdef __cplusplus
}
#endif
