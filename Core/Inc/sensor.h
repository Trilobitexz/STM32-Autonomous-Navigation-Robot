#ifndef SENSOR_H
#define SENSOR_H

#include "main.h"

// 如果之後你需要用 TIM3 來計算超音波回波時間，可以在此宣告
// (注意：你目前將 TIM3_CH1 用作右後輪 PWM，若要計時需使用 TIM3 的其他通道或另一個 Timer)
// extern TIM_HandleTypeDef htim3;

// 感測器讀取函式 (未來若需啟用超音波與循跡)
uint8_t Read_Track_Sensor(void);
float Read_Ultrasonic_Distance(void);

// 微秒延遲輔助函式
void Delay_us(uint16_t us);

#endif /* SENSOR_H */
