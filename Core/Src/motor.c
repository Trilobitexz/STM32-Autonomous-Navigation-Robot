#include "motor.h"

// ================= 方向腳位定義 =================
// 根據最新配置，方向腳位已移至 PC0~PC3, PC8~PC9, PB8~PB9，避開編碼器腳位

// 左前輪 FL (馬達 1) 方向腳位
#define IN1_PORT GPIOC
#define IN1_PIN  GPIO_PIN_0
#define IN2_PORT GPIOC
#define IN2_PIN  GPIO_PIN_1

// 右前輪 FR (馬達 3) 方向腳位
#define IN3_PORT GPIOC
#define IN3_PIN  GPIO_PIN_2
#define IN4_PORT GPIOC
#define IN4_PIN  GPIO_PIN_3

// 左後輪 RL (馬達 2) 方向腳位
#define IN5_PORT GPIOC
#define IN5_PIN  GPIO_PIN_9
#define IN6_PORT GPIOC
#define IN6_PIN  GPIO_PIN_8

// 右後輪 RR (馬達 4) 方向腳位
#define IN7_PORT GPIOB
#define IN7_PIN  GPIO_PIN_9
#define IN8_PORT GPIOB
#define IN8_PIN  GPIO_PIN_8
// ==============================================

/**
  * @brief  控制單一馬達的轉向與速度
  * @param  motor_id: 1(左前), 2(左後), 3(右前), 4(右後)
  * @param  dir: 1(正轉), -1(反轉), 0(停止)
  * @param  speed: PWM 佔空比 (0 ~ ARR)
  */
void Set_Motor(uint8_t motor_id, int8_t dir, uint16_t speed) {
    GPIO_TypeDef *port1, *port2;
    uint16_t pin1, pin2;
    uint32_t timer_channel;

    // 依據馬達 ID 指派方向腳位與對應的 TIM1 頻道
    switch(motor_id) {
        case 1: // 左前 FL
            port1 = IN1_PORT; pin1 = IN1_PIN;
            port2 = IN2_PORT; pin2 = IN2_PIN;
            timer_channel = TIM_CHANNEL_4; // 對應 PA11
            break;
        case 2: // 左後 RL
            port1 = IN5_PORT; pin1 = IN5_PIN;
            port2 = IN6_PORT; pin2 = IN6_PIN;
            timer_channel = TIM_CHANNEL_2; // 對應 PA9
            break;
        case 3: // 右前 FR
            port1 = IN3_PORT; pin1 = IN3_PIN;
            port2 = IN4_PORT; pin2 = IN4_PIN;
            timer_channel = TIM_CHANNEL_3; // 對應 PA10
            break;
        case 4: // 右後 RR
            port1 = IN7_PORT; pin1 = IN7_PIN;
            port2 = IN8_PORT; pin2 = IN8_PIN;
            timer_channel = TIM_CHANNEL_1; // 對應 PA8
            break;
        default: return;
    }

    // 設定馬達方向
    if (dir == 1) {
        HAL_GPIO_WritePin(port1, pin1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(port2, pin2, GPIO_PIN_RESET);
    } else if (dir == -1) {
        HAL_GPIO_WritePin(port1, pin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port2, pin2, GPIO_PIN_SET);
    } else { // 停止
        HAL_GPIO_WritePin(port1, pin1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port2, pin2, GPIO_PIN_RESET);
        speed = 0;
    }

    // 統一使用 htim1 輸出 PWM 控制速度
    __HAL_TIM_SET_COMPARE(&htim1, timer_channel, speed);
}

/**
  * @brief 麥克納姆輪基礎動作封裝 (此處邏輯假設馬達極性皆相同)
  *        若轉向顛倒，請直接將 L298N 上的馬達紅黑線反接，或在此處將 dir 乘上 -1
  */
void Car_Forward(uint16_t speed) {
    Set_Motor(1, 1, speed); Set_Motor(2, 1, speed);
    Set_Motor(3, 1, speed); Set_Motor(4, 1, speed);
}

void Car_Stop(void) {
    Set_Motor(1, 0, 0); Set_Motor(2, 0, 0);
    Set_Motor(3, 0, 0); Set_Motor(4, 0, 0);
}

void Car_Turn_Left(uint16_t speed) {
    Set_Motor(1, -1, speed); Set_Motor(2, -1, speed);
    Set_Motor(3, 1, speed);  Set_Motor(4, 1, speed);
}

void Car_Turn_Right(uint16_t speed) {
    Set_Motor(1, 1, speed);  Set_Motor(2, 1, speed);
    Set_Motor(3, -1, speed); Set_Motor(4, -1, speed);
}

// 麥克納姆輪特有橫移：左前退、左後進、右前進、右後退
void Car_Strafe_Left(uint16_t speed) {
    Set_Motor(1, -1, speed); Set_Motor(2, 1, speed);
    Set_Motor(3, 1, speed);  Set_Motor(4, -1, speed);
}

// 麥克納姆輪特有橫移：左前進、左後退、右前退、右後進
void Car_Strafe_Right(uint16_t speed) {
    Set_Motor(1, 1, speed); Set_Motor(2, -1, speed);
    Set_Motor(3, -1, speed);  Set_Motor(4, 1, speed);
}
