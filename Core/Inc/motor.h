#ifndef MOTOR_H
#define MOTOR_H

#include "main.h" // 為了使用 HAL 庫與 GPIO 定義

// 宣告在 main.c 或 TIM 設定中初始化的 Timer
extern TIM_HandleTypeDef htim2;

// 基礎車體運動函式
void Set_Motor(uint8_t motor_id, int8_t dir, uint16_t speed);
void Car_Forward(uint16_t speed);
void Car_Stop(void);
void Car_Turn_Left(uint16_t speed);
void Car_Turn_Right(uint16_t speed);
void Car_Strafe_Left(uint16_t speed);
void Car_Strafe_Right(uint16_t speed);

#endif /* MOTOR_H */
