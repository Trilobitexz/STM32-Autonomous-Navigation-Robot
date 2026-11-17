#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"

// 宣告在 main.c 中初始化的 TIM1 (統一負責 4 路 PWM 輸出給 L298N)
extern TIM_HandleTypeDef htim1;

// 基礎車體運動函式
void Set_Motor(uint8_t motor_id, int8_t dir, uint16_t speed);
void Car_Forward(uint16_t speed);
void Car_Stop(void);
void Car_Turn_Left(uint16_t speed);
void Car_Turn_Right(uint16_t speed);
void Car_Strafe_Left(uint16_t speed);
void Car_Strafe_Right(uint16_t speed);

#endif /* MOTOR_H */
