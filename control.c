#include<stdbool.h>

extern float left_distance;
extern float mid_distance;
extern float right_distance;
extern float m_distance;

extern int YAM;

void waiting_distance(float temp){
    int angle = YAM;
    while(m_distance - temp >= 50){
        if(angle - YAM >= 15) car_right();
        else if(angle - YAM <= -15) car_left();
        else car_forward();
    }
}

void waiting_degree(int temp){
    while(abs(YAM - temp) <= 90);
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

        if(left == true){
            //转弯操作分为两个步骤：1原地转 2向前一个单位
            //先进行转向
            car_left();
            int temp = YAM;
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
            int temp = YAM;
            waiting_degree(temp);
            //再向前一个单位
            car_forward();
            temp = m_distance;
            waiting_distance(temp);
            car_stop();
        }
        else{
            car_reverse();
            float temp = m_distance;
            waiting_distance(temp);
            car_stop();
        }
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