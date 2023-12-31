#include <stdio.h>
#include <stdlib.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
// #include "gyro.h"
#include "iot_errno.h"
#include "hcsr04.h"
#include "iot_gpio_ex.h"
#include <stdbool.h>

CAR_DRIVE car_drive = { 0 };
// typedef struct {
//     uint32_t LeftForward;
//     uint32_t RightForward;
//     uint32_t TurnLeft;
//     uint32_t TurnRight;
//     float yaw;
//     float distance;
//     unsigned int leftangle;
//     unsigned int middangle;
//     unsigned int rightangle;
// } CAR_DRIVE;
ENUM_MODE g_mode = MODE_ON_OFF;
// typedef enum {
//     MODE_ON_OFF = 0,
//     MODE_SET_LEFT_FORWARD,
//     MODE_SET_RIGHT_FORWARD,
//     MODE_SET_TURN_LEFT,
//     MODE_SET_TURN_RIGHT,
//     MODE_SET_YAW,
//     MODE_SET_DISTANCE,
//     MODE_SET_LEFTSG92R,
//     MODE_SET_MIDDERSG92R,
//     MODE_SET_RIGHTSG92R,
//     MODE_END,
// } ENUM_MODE;

int g_CarStarted = 0;
float yaw_data = 0.0f;

#define GPIO_8 8
#define GPIO_7 7
#define GPIO_FUNC 0

#define MASK_BUTTON1        (0x10)
#define MASK_BUTTON2        (0x08)
#define MASK_BUTTON3        (0x04)
#define YAW                 (90.0)
#define DISTANCE            (15.0)
#define CAR_TURN_LEFT                     (0)
#define CAR_TURN_RIGHT                    (1)

float GetDistance  (void)
{
    static unsigned long start_time = 0, time = 0;
    float distance = 0.0;
    IotGpioValue value = IOT_GPIO_VALUE0;
    unsigned int flag = 0;
    float pi = 0.034;
    int l = 2;
    unsigned int delayTime = 20;
    IoTWatchDogDisable();

    hi_io_set_func(GPIO_8, GPIO_FUNC);
    IoTGpioSetDir(GPIO_8, IOT_GPIO_DIR_IN);

    IoTGpioSetDir(GPIO_7, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(GPIO_7, IOT_GPIO_VALUE1);
    hi_udelay(delayTime);
    IoTGpioSetOutputVal(GPIO_7, IOT_GPIO_VALUE0);

    while (1) {
        IoTGpioGetInputVal(GPIO_8, &value);
        if (value == IOT_GPIO_VALUE1 && flag == 0) {
            start_time = hi_get_us();
            flag = 1;
        }
        if (value == IOT_GPIO_VALUE0 && flag == 1) {
            time = hi_get_us() - start_time;
            start_time = 0;
            break;
        }
    }
    distance = time * pi / l;
    return distance;
}

float left_distance = 0.0;
float right_distance = 0.0;
float mid_distance=0.0;
float m_distance = 0.0;
unsigned int engine_go_where(void)
{
    unsigned int temp;

    /* 舵机往左转动测量左边障碍物的距离 */
    S92RInit();

    EngineTurnLeft(car_drive.leftangle);
    hi_sleep(200);
    // TaskMsleep(200); // 200ms
    left_distance = GetDistance();
    hi_sleep(200);
    // TaskMsleep(200); // 200ms
    /* 归中 */
    RegressMiddle(car_drive.middangle);
    hi_sleep(200);
    // TaskMsleep(200); // 200ms
    mid_distance=GetDistance();

    /* 舵机往右转动测量右边障碍物的距离 */
    EngineTurnRight(car_drive.rightangle);
    hi_sleep(200);
    // TaskMsleep(200); // 200ms
    right_distance = GetDistance();
    hi_sleep(200);
    // TaskMsleep(200); // 200ms
    /* 归中 */
    RegressMiddle(car_drive.middangle);

    printf("left:%f mid:%f right:%f\n",left_distance,mid_distance,right_distance);
    if (left_distance > right_distance) {
        temp =  CAR_TURN_LEFT;
    } else {
        temp =  CAR_TURN_RIGHT;
    }
    return temp;
}
float get_left()
{
    return left_distance;
}
float get_mid()
{
    return mid_distance;
}
float get_right()
{
    return right_distance;
}
float get_m()
{
    return m_distance;
}

/*
 * 根据障碍物的距离来判断小车的行走方向
 * 1、距离大于等于15cm继续前进
 * 2、距离小于15cm，先停止再后退0.1s,继续进行测距,再进行判断
 */
// void car_where_to_go(float distance)
// {
//     if (distance < car_drive.distance) {
//         car_backward(car_drive.LeftForward, car_drive.RightForward);
//         hi_sleep(500);
//         // TaskMsleep(500); // 后退500ms
//         car_stop();
//         unsigned int ret = engine_go_where();
//         if (ret == CAR_TURN_LEFT) {
//             while ((GetYaw() - yaw_data) < car_drive.yaw) {
//                 Lsm_Get_RawAcc();
//                 car_left(car_drive.TurnRight);
//             }
//         } else if (ret == CAR_TURN_RIGHT) {
//             while ((yaw_data - GetYaw()) < car_drive.yaw) {
//                 Lsm_Get_RawAcc();
//                 car_right(car_drive.TurnLeft);
//             }
//         }
//     } else {
//         car_forward(car_drive.LeftForward, car_drive.RightForward);
//     }
//     yaw_data = GetYaw();
// }

/* 超声波避障 */
void ultrasonic(void)//转头，输出周围距离
{
    uint32_t t=engine_go_where();
    return;
}
bool thread_ready=false;
void ultrasonic_direct(void)//不转头，直接输出距离
{
    /* 获取前方物体的距离 */

    m_distance = GetDistance();
    printf("\t\tdistance:%f\n",m_distance);
    thread_ready=true;
    return;
}

