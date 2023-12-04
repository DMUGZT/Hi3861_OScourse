#include<stdbool.h>
#include<stdio.h>
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


<<<<<<< HEAD
=======

>>>>>>> 631cdb0 (local)
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
    printf("1car forward");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE1);
    gpio_control(GPIO10, IOT_GPIO_VALUE0);
}

void car_left(void)
{
    printf("1car turn left");
    gpio_control(GPIO0, IOT_GPIO_VALUE0);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE1);
    gpio_control(GPIO10, IOT_GPIO_VALUE0);
}

void car_right(void)
{
    printf("1car turn right");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE0);
    gpio_control(GPIO9, IOT_GPIO_VALUE0);
    gpio_control(GPIO10, IOT_GPIO_VALUE0);
}

void car_stop(void)
{
    printf ("1car stop");
    gpio_control(GPIO0, IOT_GPIO_VALUE1);
    gpio_control(GPIO1, IOT_GPIO_VALUE1);
    gpio_control(GPIO9, IOT_GPIO_VALUE1);
    gpio_control(GPIO10, IOT_GPIO_VALUE1);
}

void car_reverse(void)
{
    printf ("1car reverse");
    gpio_control(GPIO0, IOT_GPIO_VALUE0);
    gpio_control(GPIO1, IOT_GPIO_VALUE1);
    gpio_control(GPIO9, IOT_GPIO_VALUE1);
    gpio_control(GPIO10, IOT_GPIO_VALUE0);
}
#define ANGLE 8
void waiting_distance(float temp){
    int angle = get_YAW();
    int dif_l=0,dif_r=0;
    while(temp - m_distance<= 17){//前进30cm
        printf("\t\t\t\t\tOrigin dis:%f cur dis:%f dif:%f\n",temp,m_distance,temp-m_distance);
        dif_l=0;
        dif_r=0;
        if(angle-YAW>0)dif_r=abs(angle-YAW);
        dif_l=abs(angle-YAW-360);
        dif_l=dif_l>=360?dif_l-360:dif_l;
        if(dif_l>45)dif_l=0;
        if(dif_r>45)dif_r=0;
        //-15----15之间为正常，-45----15&&15----45之间回正
        printf("\t\t\t\t\tdif_left:%d dif_right:%d\n",dif_l,dif_r);
        if(dif_l >= ANGLE) car_right(),osDelay(10);
        else if(dif_r >= ANGLE) car_left(),osDelay(10);
        else car_forward();
    }
}

void waiting_degree_180(int temp){
    int dif=0;
    do
    {
        dif=abs(temp-YAW-360);
        if(dif>=360)
        {
            dif-=360;
        }
        printf("\t\t\t\tOrigin:%d After:%d dif:%d\n",temp,YAW,dif);
    }
    while(dif <= 172);
}

void waiting_degree(int temp){
    int dif=0;
    do
    {
        dif=abs(temp-YAW-360);
        if(dif>=360)
        {
            dif-=360;
        }
        printf("\t\t\t\tOrigin:%d After:%d dif:%d\n",temp,YAW,dif);
    }
    while(dif <= 86);
}
void waiting_degree_right(int temp){
    int dif=0;
    do
    {
        dif=abs(temp-YAW);
        printf("\t\t\t\tOrigin:%d After:%d dif:%d\n",temp,YAW,dif);
    }
    while(dif <= 85);
}
extern void Thread_Ultrasonic_direct();
extern void Thread_Ultrasonic();
extern osThreadId_t newThread(char *name, osThreadFunc_t func, char *arg);
extern bool thread_ready;
void one_direct_distance()
{
    if(tid_Ultrasonic!=0)
    {
        printf("thread id : %d",tid_Ultrasonic);
        osStatus_t status1=osThreadTerminate(tid_Ultrasonic);
        printf("status:%d\n",status1);
    }

    tid_Ultrasonic=newThread("Ultrasonic Threaed",
        Thread_Ultrasonic_direct,"Ultrasonic thread");
    
}
void waiting()
{
    while(1)
    {
        printf("ready?:%s\n",thread_ready==true?"true":"false");
        if(thread_ready==true)break;
    }
}
void three_direct_distance()
{
    if(tid_Ultrasonic!=0)
    {
        printf("thread id : %d",tid_Ultrasonic);
        osStatus_t status1=osThreadTerminate(tid_Ultrasonic);
        printf("status:%d\n",status1);
    }

    tid_Ultrasonic=newThread("Ultrasonic Threaed",
        Thread_Ultrasonic,"Ultrasonic thread");
}


int times=0;
bool flag=false;
void control(){

    while(true){
        thread_ready=false;
        bool left = true;
        bool forward = true;
        bool right = true;
        ultrasonic();
        osDelay(5);
    
        printf("left:%f mid:%f right:%f",left_distance,mid_distance,right_distance);

        //把float类型距离转化成布尔类型的值
        if(left_distance < 30) left = false;
        if(mid_distance < 30) forward = false;
        if(right_distance < 30) right = false;
        
        one_direct_distance();
        waiting();
        //打开只读前方距离
        if(left == true){
            printf("[times:%d]enter left!\n",times);
            //转弯操作分为两个步骤：1原地转 2向前一个单位
            //先进行转向
            int temp = get_YAW();
            car_left();
            waiting_degree(temp);
            //再向前一个单位
            float temp1=m_distance;
            car_forward();
            printf("cur distance:%f",temp1);
            waiting_distance(temp1);
            car_stop(); 
        }
        else if(forward == true){
            printf("[times:%d]enter forward!\n",times);
            float temp = m_distance;
            printf("cur distance:%f",temp);
            car_forward();
            waiting_distance(temp);
            car_stop();
        }
        else if(right == true){
            //转弯操作分为两个步骤：1原地转 2向前一个单位
            printf("[times:%d]enter right!\n",times);
            //先进行转向
            car_right();
            int temp = get_YAW();
            waiting_degree_right(temp);
            //再向前一个单位
            float temp1=m_distance;
            car_forward();
            printf("cur distance:%f",temp1);
            waiting_distance(temp1);
            car_stop();
        }
        else{
            printf("[times:%d]enter reserve!\n",times);
            car_reverse();
            int temp1 = get_YAW();
            printf("YAW:%d",get_YAW());
            waiting_degree_180(temp1);
            float temp = m_distance;
            printf("cur distance:%f",temp);
            waiting_distance(temp);
            car_stop();
        }
        times++;
        osStatus_t status1=osThreadTerminate(tid_Ultrasonic);
    }
<<<<<<< HEAD
}
=======
}
>>>>>>> 631cdb0 (local)
