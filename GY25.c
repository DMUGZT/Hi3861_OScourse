#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include<hi_task.h>
#include "iot_gpio_ex.h"
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "iot_uart.h"
#include "hi_uart.h"
#include "iot_watchdog.h"
#include "iot_errno.h"
#include "hcsr04.h"

#define UART_BUFF_SIZE 100
#define U_SLEEP_TIME   100000
#define THREAD_NUM (1000)
#define STACK_SIZE (1024)
#define DELAY_TICKS_20   (20)
#define DELAY_TICKS_100 (100)

int cnt=0;

void Uart2GpioInit(void)
{
    IoTGpioInit(IOT_IO_NAME_GPIO_11);
    // 设置GPIO11的管脚复用关系为UART2_TX Set the pin reuse relationship of GPIO0 to UART1_ TX
    IoSetFunc(IOT_IO_NAME_GPIO_11, IOT_IO_FUNC_GPIO_11_UART2_TXD);
    IoTGpioInit(IOT_IO_NAME_GPIO_12);
    // 设置GPIO12的管脚复用关系为UART2_RX Set the pin reuse relationship of GPIO1 to UART1_ RX
    IoSetFunc(IOT_IO_NAME_GPIO_12, IOT_IO_FUNC_GPIO_12_UART2_RXD);
}
void Uart2Config(void)
{
    uint32_t ret;
    /* 初始化UART配置，波特率 115200，数据bit为8,停止位1，奇偶校验为NONE */
    /* Initialize UART configuration, baud rate is 9600, data bit is 8, stop bit is 1, parity is NONE */
    IotUartAttribute uart_attr = {
        .baudRate = 115200,
        .dataBits = 8,
        .stopBits = 1,
        .parity = 0,
    };
    ret = IoTUartInit(HI_UART_IDX_2, &uart_attr);
    if (ret != IOT_SUCCESS) {
        printf("Init Uart2 Falied Error No : %d\n", ret);
        return;
    }
}

uint16_t YAW,PITCH,ROLL;
void Thread_GY25()
{
    unsigned char uartReadBuff[UART_BUFF_SIZE] = {0};
    // 对UART2的一些初始化
    Uart2GpioInit();
    // 对UART2参数的一些配置
    Uart2Config();
    while(1)
    {
        static uint8_t k=0,readbuf[8]={0};
        k=IoTUartRead(HI_UART_IDX_2,readbuf,UART_BUFF_SIZE);
        if(readbuf[0]!=0xaa)
        {
            k=0;
            memset(readbuf,0,sizeof(uint8_t)*8);
        }
        if(k==8)
        {
            if(readbuf[7]==0x55)
            {
                YAW=(readbuf[1]<<8|readbuf[2])/100;
                PITCH=(readbuf[3]<<8|readbuf[4])/100;
                ROLL=(readbuf[5]<<8|readbuf[6])/100;
                // int t=YAW;
                if(YAW>180)YAW=YAW-295;
                // if(PITCH>180)PITCH=PITCH-295;
                // if(ROLL>180)ROLL=ROLL-295;
                // if(YAW>180)YAW-=360;
                printf("(AF)count:%d Y:%d P:%d R:%d\n",cnt++,YAW,PITCH,ROLL);
                // printf("\t\t\t(AF)count:%d Y:%d P:%d R:%d\n",cnt++,t,PITCH,ROLL);
                // hi_sleep(10);
            }
        }
        k=0;
    }
    osThreadYield();
}
uint16_t get_YAW()
{
    return YAW;
}
void Entry_GY(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 5 * 1024; // 任务栈大小*1024 stack size 5*1024
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)Thread_GY25, NULL, &attr) == NULL) {
        printf("[Task] Failed to create Thread_GY25!\n");
    }
}

APP_FEATURE_INIT(Entry_GY);