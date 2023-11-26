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


#define STACK_SIZE (1024)

osThreadId_t tid_GY25,tid_Ultrasonic,tid_Control;
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
void Thread_Ultrasonic()
{
    while(1)
    {
        ultrasonic();//返回的是左中右的距离,调用一次获取一次
        osThreadYield();
    }
}
extern CAR_DRIVE car_drive ;
void Thread_Ultrasonic_direct()
{
    RegressMiddle(car_drive.middangle);
    hi_sleep(200);
    while(1)
    {
        ultrasonic_direct();//返回的是前方的距离,调用一次获取一次
        osThreadYield();
    }
}
// void Thread_Control()
// {
//     control();
// }

static void Task(void)
{
    uint32_t count = 0;
    uint32_t len = 0;


    int status1,status2,status3;

    // tid_GY25 = newThread("GY_25Thread",Thread_GY25,"GY-25 thread");

    // tid_Ultrasonic = newThread("Ultrasonic Threaed",Thread_Ultrasonic,"Ultrasonic thread");
    control();
    // tid_Control= newThread("Car Main Control function thread",Thread_Control,"Control Thread");
    // printf("Thread Count:%d",osThreadGetCount());
    // printf("Thread Enumerate Count:%d",osThreadEnumerate());
    // osDelay(500);
    osDelay(10000000);
    // status1=osThreadTerminate(tid_GY25);
    // printf("[GY-25 thread]osThreadTerminate, status1: %d.\r\n", status1);
    // status2=osThreadTerminate(tid_Ultrasonic);
    // printf("[Ultrasonic thread]osThreadTerminate, status2: %d.\r\n", status2);
    // status3=osThreadTerminate(tid_Control);
    // printf("[Control Thread]osThreadTerminate, status2: %d.\r\n", status3);
    printf("线程结束");
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
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)Task, NULL, &attr) == NULL) {
        printf("[Task] Failed to create Task!\n");
    }
}

APP_FEATURE_INIT(Entry);
