#include<stdbool.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "iot_gpio.h"
#include "hi_io.h"
#include "hi_time.h"
#include "iot_pwm.h"
#include "hi_pwm.h"

#define GPIO0 0
#define GPIO1 1
#define GPIO9 9
#define GPIO10 10
#define GPIOFUNC 0
#define TASK_STAK_SIZE    (1024*10)

extern float left_distance;
extern float mid_distance;
extern float right_distance;
extern float m_distance;

extern uint16_t YAW;

extern osThreadId_t tid_Ultrasonic;


void gpio_control (unsigned int gpio, IotGpioValue value)
{
    hi_io_set_func(gpio, GPIOFUNC);
    IoTGpioSetDir(gpio, IOT_GPIO_DIR_OUT);
    IoTGpioSetOutputVal(gpio, value);
}

void car_backward(void)
{
    gpio_control(GPIO0, IOT_GPIO_VALUE0);
    gpio_control(GPIO1, IOT_GPIO_VALUE1);
    gpio_control(GPIO9, IOT_GPIO_VALUE0);
    gpio_control(GPIO10, IOT_GPIO_VALUE1);
}

void car_forward(void)
{
    printf("car forward");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE0);
    gpio_control(GPIO10, IOT_GPIO_VALUE1);
}

void car_left(void)
{
    printf("car turn left");
    gpio_control(GPIO0, IOT_GPIO_VALUE0);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE0);
    gpio_control(GPIO10, IOT_GPIO_VALUE1);
}

void car_right(void)
{
    printf("car turn right");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE0);
    gpio_control(GPIO10, IOT_GPIO_VALUE0);
}

void car_stop(void)
{
    printf ("car stop");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE1);
    gpio_control(GPIO9, IOT_GPIO_VALUE1);
    gpio_control(GPIO10, IOT_GPIO_VALUE1);
}

void car_reverse(void)
{
    printf ("car reverse");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE1);
    gpio_control(GPIO10, IOT_GPIO_VALUE0);
}

void waiting_distance(float temp){
    int angle = YAW;
    while(m_distance - temp >= 50){
        if(angle - YAW >= 15) car_right();
        else if(angle - YAW <= -15) car_left();
        else car_forward();
    }
}

void waiting_degree_180(int temp){
    while(abs(YAW - temp) <= 180);
}

void waiting_degree(int temp){
    while(abs(YAW - temp) <= 90);
}
extern void Thread_Ultrasonic_direct();
extern void Thread_Ultrasonic();
extern osThreadId_t newThread(char *name, osThreadFunc_t func, char *arg);

void one_direct_distance()
{
    osThreadTerminate(tid_Ultrasonic);
    tid_Ultrasonic=newThread("Ultrasonic Threaed",
        Thread_Ultrasonic_direct,"Ultrasonic thread");
}
void three_direct_distance()
{
    osThreadTerminate(tid_Ultrasonic);
    tid_Ultrasonic=newThread("Ultrasonic Threaed",
        Thread_Ultrasonic,"Ultrasonic thread");
}




void control(){

    while(true){
        bool left = true;
        bool forward = true;
        bool right = true;

        //把float类型距离转化成布尔类型的值
        if(left_distance < 15) left = false;
        if(mid_distance < 15) forward = false;
        if(right_distance < 15) right = false;

        one_direct_distance();
        //打开只读前方距离
        if(left == true){
            //转弯操作分为两个步骤：1原地转 2向前一个单位
            //先进行转向
            car_left();
            int temp = YAW;
            waiting_degree(temp);
            //再向前一个单位
            car_forward();
            temp = m_distance;
            waiting_distance(temp);
            car_stop(); 
        }
        else if(forward == true){
            car_forward();
            float temp = m_distance;
            waiting_distance(temp);
            car_stop();
        }
        else if(right == true){
            //转弯操作分为两个步骤：1原地转 2向前一个单位
            //先进行转向
            car_right();
            int temp = YAW;
            waiting_degree(temp);
            //再向前一个单位
            car_forward();
            temp = m_distance;
            waiting_distance(temp);
            car_stop();
        }
        else{
            car_reverse();
            int temp1 = YAW;
            waiting_degree_180(temp1);
            float temp = m_distance;
            waiting_distance(temp);
            car_stop();
        }
        three_direct_distance();//终止只读前方并启动读取三方距离
    }
}

// #include<stdbool.h>

// extern float left_distance;
// extern float mid_distance;
// extern float right_distance;
// extern float m_distance;

// extern int YAM;

// void waiting_distance(int temp){
//     while(m_distance - temp >= 50);
// }

// void waiting_degree(int temp){
//     while(abs(YAM - temp) <= 90);
// }

// void control(){

//     while(true){
//         bool left = true;
//         bool forward = true;
//         bool right = true;
//         bool flag = true;

//         //把float类型距离转化成布尔类型的值
//         if(left_distance < 15) left = false;
//         if(mid_distance < 15) forward = false;
//         if(right_distance < 15) right = false;

//         if(left == true){
//             flag = true;
//             car_left();
//             int temp = YAM;
//             waiting_degree(temp);
//             car_stop();
//         }
//         //也可以尝试添加一个标志位判断是否进入死胡同 或者 支持调头的操作
//         else if(forward == true && flag == true){
//             car_forward();
//             int temp = m_distance;
//             waiting_distance(temp);
//             car_stop();
//         }
//         else if(right == true){
//             flag = true;
//             car_right();
//             int temp = YAM;
//             waiting_degree(temp);
//             car_stop();
//         }
//         else{
//             flag = false;
//             car_backward();
//             int temp = m_distance;
//             waiting_distance(temp);
//             car_stop();
//         }
//     }
// }