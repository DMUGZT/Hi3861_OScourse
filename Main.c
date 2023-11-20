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

#define UART_BUFF_SIZE 100
#define U_SLEEP_TIME   100000
#define THREAD_NUM (1000)
#define STACK_SIZE (1024)
#define DELAY_TICKS_20   (20)
#define DELAY_TICKS_100 (100)

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

osThreadId_t newThread(char *name, osThreadFunc_t func, char *arg)
{
    osThreadAttr_t attr = {
        name, 0, NULL, 0, NULL, STACK_SIZE*2, osPriorityNormal, 0, 0
    };
    osThreadId_t tid = osThreadNew(func, (void *)arg, &attr);
    if (tid == NULL) {
        printf("[Thread Test] osThreadNew(%s) failed.\r\n", name);
    } else {
        printf("[Thread Test] osThreadNew(%s) success, thread id: %d.\r\n", name, tid);
    }
    return tid;
}
int cnt=0;
osThreadId_t tid_GY25,tid_Ultrasonic;
uint16_t YAW,PITCH,ROLL;
void Thread_GY25()
{
    
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
                YAW=(readbuf[1]<<8|readbuf[2]);
                PITCH=(readbuf[3]<<8|readbuf[4]);
                ROLL=(readbuf[5]<<8|readbuf[6]);
                printf("count:%d Y:%d P:%d R:%d\n",cnt++,YAW/100,PITCH/100,ROLL/100);
                // hi_sleep(10);
            }


        }
        k=0;
        osThreadYield();
        // osThreadSuspend(tid_GY25);
        // osThreadResume(tid_Ultrasonic);

    }
}
// extern void ultrasonic();
void Thread_Ultrasonic()
{
    while(1)
    {
        ultrasonic();
        osThreadYield();
        // osThreadSuspend(tid_Ultrasonic);
        // osThreadResume(tid_GY25);
    }
    

    
}

static void Task(void)
{
    uint32_t count = 0;
    uint32_t len = 0;
    unsigned char uartReadBuff[UART_BUFF_SIZE] = {0};
    // 对UART2的一些初始化
    Uart2GpioInit();
    // 对UART2参数的一些配置
    Uart2Config();
    int status1,status2;

    tid_GY25 = newThread("GY_25Thread",Thread_GY25,"GY-25 thread");
    tid_Ultrasonic = newThread("Ultrasonic Threaed",Thread_Ultrasonic,"Ultrasonic thread");
    printf("Thread Count:%d",osThreadGetCount());
    // printf("Thread Enumerate Count:%d",osThreadEnumerate());
    // osDelay(500);
    // status1=osThreadTerminate(tid_GY25);
    printf("[GY-25 thread]osThreadTerminate, status1: %d.\r\n", status1);
    // status2=osThreadTerminate(tid_Ultrasonic);
    printf("[Ultrasonic thread]osThreadTerminate, status2: %d.\r\n", status2);
    printf("线程结束");
    // while(1)
    // {
    //     static uint8_t k=0,readbuf[8]={0};
    //     k=IoTUartRead(HI_UART_IDX_2,readbuf,UART_BUFF_SIZE);
    //     if(readbuf[0]!=0xaa)
    //     {
    //         k=0;
    //         memset(readbuf,0,sizeof(uint8_t)*8);
    //     }
    //     if(k==8)
    //     {
    //         if(readbuf[7]==0x55)
    //         {
    //             YAW=(readbuf[1]<<8|readbuf[2]);
    //             PITCH=(readbuf[3]<<8|readbuf[4]);
    //             ROLL=(readbuf[5]<<8|readbuf[6]);
    //             printf("count:%d Y:%d P:%d R:%d\n",cnt++,YAW/100,PITCH/100,ROLL/100);
    //             hi_sleep(10);
    //         }


    //     }
    //     k=0;
    // }


    // while (1) {
        // 清空字符串
    // for(int j=0;j<2;j++){
    //     memset(uartReadBuff,0,sizeof(unsigned char)*UART_BUFF_SIZE);
    //     len = IoTUartRead(HI_UART_IDX_2,uartReadBuff,UART_BUFF_SIZE);
    //     if (len > 0) {
    //         printf("%d\t",len);
    //         for(int i=0;i<len;i++){
    //             // printf("%#X",uartReadBuff);
    //             putchar(uartReadBuff[i]);
    //             putchar(' ');
    //         }
    //     }
    //     printf("END!");
    // // }
    // }
}




void Entry(void)
{
    osThreadAttr_t attr;
    IoTWatchDogDisable();

    attr.name = "Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 5 * 1024; // 任务栈大小*1024 stack size 5*1024
    attr.priority = osPriorityNormal+2;

    if (osThreadNew((osThreadFunc_t)Task, NULL, &attr) == NULL) {
        printf("[Task] Failed to create Task!\n");
    }
}

APP_FEATURE_INIT(Entry);
