#include "sensor.h"

/* ================= 腳位定義區 ================= */
// 循跡模組 (TCRT5000) 腳位定義 (對應你 main.h 的 PB13, PB14, PB15, PB1, PB2)
#define Track_L2_GPIO_Port GPIOB
#define Track_L2_Pin       GPIO_PIN_13
#define Track_L1_GPIO_Port GPIOB
#define Track_L1_Pin       GPIO_PIN_14
#define Track_M_GPIO_Port  GPIOB
#define Track_M_Pin        GPIO_PIN_15
#define Track_R1_GPIO_Port GPIOB
#define Track_R1_Pin       GPIO_PIN_1
#define Track_R2_GPIO_Port GPIOB
#define Track_R2_Pin       GPIO_PIN_2

// 超音波模組 (HC-SR04) 腳位定義 (對應你 main.h 的 PC10, PC11)
#define TRIG_GPIO_Port     GPIOC
#define TRIG_Pin           GPIO_PIN_10
#define ECHO_GPIO_Port     GPIOC
#define ECHO_Pin           GPIO_PIN_11
/* ============================================== */

// 建立微秒延遲函式
// ⚠️ 警告：由於 htim3 目前已指派給右後輪作 PWM 輸出，若在此直接重置 Counter 會干擾馬達運轉。
// 若未來要啟用超音波，請指派另一個獨立的 Timer (如 htim6) 來做 Delay_us
void Delay_us(uint16_t us) {
    // 假設未來使用 htim6 計時
    // __HAL_TIM_SET_COUNTER(&htim6, 0);
    // while (__HAL_TIM_GET_COUNTER(&htim6) < us);
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
    // ⚠️ 警告：目前 Timer 資源緊湊，此函式需配合獨立 Timer (如 htim6) 才能正常運作
    uint32_t local_time = 0;
    float distance = 0;

    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    Delay_us(10);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

    while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET);

    // __HAL_TIM_SET_COUNTER(&htim6, 0);
    // while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET) {
    //     local_time = __HAL_TIM_GET_COUNTER(&htim6);
    // }

    // 計算距離：距離(cm) = 時間(us) * 聲速(340m/s) / 2
    distance = local_time * 0.017;
    return distance;
