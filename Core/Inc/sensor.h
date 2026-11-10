#ifndef SENSOR_H
#define SENSOR_H

#include "main.h"

// 宣告在 main.c 設定中初始化的 Timer (用於超音波微秒計時)
extern TIM_HandleTypeDef htim3;

// 感測器讀取函式
uint8_t Read_Track_Sensor(void);
float Read_Ultrasonic_Distance(void);

// 微秒延遲輔助函式
void Delay_us(uint16_t us);

#endif /* SENSOR_H */
