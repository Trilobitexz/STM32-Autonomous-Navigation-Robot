#include "sensor.h"

/* ================= 腳位定義區 ================= */
// 請根據你實際接到 STM32 的腳位修改以下的 GPIO_PIN 與 GPIO_PORT
// 以下為假設配置，請務必改成你目前的實際接法

// 循跡模組 (TCRT5000) 腳位定義
#define Track_L2_GPIO_Port GPIOC // 假設接在 PC0
#define Track_L2_Pin       GPIO_PIN_0
#define Track_L1_GPIO_Port GPIOC // 假設接在 PC1
#define Track_L1_Pin       GPIO_PIN_1
#define Track_M_GPIO_Port  GPIOC // 假設接在 PC2
#define Track_M_Pin        GPIO_PIN_2
#define Track_R1_GPIO_Port GPIOC // 假設接在 PC3
#define Track_R1_Pin       GPIO_PIN_3
#define Track_R2_GPIO_Port GPIOC // 假設接在 PC4
#define Track_R2_Pin       GPIO_PIN_4

// 超音波模組 (HC-SR04) 腳位定義
#define TRIG_GPIO_Port     GPIOB // 假設接在 PB8
#define TRIG_Pin           GPIO_PIN_8
#define ECHO_GPIO_Port     GPIOB // 假設接在 PB9
#define ECHO_Pin           GPIO_PIN_9
/* ============================================== */

// 建立微秒延遲函式 (依賴 htim3)
void Delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim3, 0);
    while (__HAL_TIM_GET_COUNTER(&htim3) < us);
}

// 讀取 5 路 TCRT5000 循跡狀態
uint8_t Read_Track_Sensor(void) {
    uint8_t sensor_val = 0;

    // 依序讀取最左(L2)到最右(R2)的感測器，並將結果推入 bitmask
    sensor_val |= (HAL_GPIO_ReadPin(Track_L2_GPIO_Port, Track_L2_Pin) << 4);
    sensor_val |= (HAL_GPIO_ReadPin(Track_L1_GPIO_Port, Track_L1_Pin) << 3);
    sensor_val |= (HAL_GPIO_ReadPin(Track_M_GPIO_Port, Track_M_Pin) << 2);
    sensor_val |= (HAL_GPIO_ReadPin(Track_R1_GPIO_Port, Track_R1_Pin) << 1);
    sensor_val |= (HAL_GPIO_ReadPin(Track_R2_GPIO_Port, Track_R2_Pin) << 0);

    return sensor_val; // 例如回傳 0b00100 (0x04) 代表只有中間壓線
}

// 讀取 HC-SR04 超音波距離
float Read_Ultrasonic_Distance(void) {
    uint32_t local_time = 0;
    float distance = 0;

    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    Delay_us(10);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

    while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET);

    __HAL_TIM_SET_COUNTER(&htim3, 0);
    while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET) {
        local_time = __HAL_TIM_GET_COUNTER(&htim3);
    }

    // 4. 計算距離：距離(cm) = 時間(us) * 聲速(340m/s) / 2
    distance = local_time * 0.017;
    return distance;
}
