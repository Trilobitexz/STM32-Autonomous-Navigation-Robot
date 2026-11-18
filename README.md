# STM32-Autonomous-Navigation-Robot

基於 **STM32F446RE** 微控制器與 **麥克納姆輪 (Mecanum Wheels)** 設計的全向移動自走車專題[cite: 2, 4]。具備 5 路紅外線精準循跡、超音波即時避障，以及四輪獨立霍爾編碼器（Encoder）閉迴圈轉速與里程回授功能[cite: 2, 3, 6]。

---

## 🛠️ 硬體架構 (Hardware Architecture)

* **核心微控制器**：STM32 Nucleo-F446RE
* **底盤與傳動**：4 輪麥克納姆輪全向底盤 + 4 顆 TT 減速馬達（附 A/B 雙相霍爾編碼器）[cite: 2, 3, 4, 6]
* **馬達驅動模組**：L298N 雙 H 橋馬達驅動板 × 2（實現四輪獨立正反轉與 PWM 調速）[cite: 2, 4]
* **環境感測器**：
  * **TCRT5000** 5 路紅外線循跡感測器模組（黑線路徑追蹤）[cite: 2, 4]
  * **HC-SR04** 超音波測距模組（前方障礙物偵測與閃避）[cite: 2, 4]
* **狀態顯示與除錯介面**：液晶顯示器 (LCD) / USART2 (ST-Link Virtual COM Port, 115200 bps)
* **供電系統**：雙節 18650 鋰電池串聯（7.4V ~ 8.4V）供電

---

## 🔌 硬體腳位與定時器配置 (Pinout & Timer Mapping)

| 功能模組 | STM32 周邊 / 腳位 | 說明 |
| :--- | :--- | :--- |
| **馬達 PWM 調速 (ENA / ENB)** | `TIM1` (CH1 ~ CH4) | 輸出 4 路獨立 PWM 訊號至兩片 L298N |
| **馬達方向控制 (IN1 ~ IN8)** | `PA4`~`PA7`, `PB1`, `PB2`, `PB13`, `PB14` | 控制 4 顆馬達的正轉、反轉與煞車 |
| **左前輪編碼器 (FL Encoder)** | `TIM2` (CH1 / CH2) | 硬體 Encoder Mode (TI1 & TI2 四倍頻) |
| **右前輪編碼器 (FR Encoder)** | `TIM5` (CH1 / CH2) | 硬體 Encoder Mode (TI1 & TI2 四倍頻)[cite: 3] |
| **左後輪編碼器 (RL Encoder)** | `TIM4` (CH1 / CH2) | 硬體 Encoder Mode (TI1 & TI2 四倍頻)[cite: 3] |
| **右後輪編碼器 (RR Encoder)** | `TIM3` (CH1 / CH2) | 硬體 Encoder Mode (TI1 & TI2 四倍頻)[cite: 3] |
| **5 路循跡感測器 (TCRT5000)** | `PB13`, `PB14`, `PB15`, `PB1`, `PB2` *(依實際配置調整)* | 數位輸入讀取 L2 / L1 / M / R1 / R2 狀態 |
| **超音波感測器 (HC-SR04)** | `PC10` (Trig) / `PC11` (Echo) | 觸發 10µs 脈衝並量測回波高電位時間[cite: 3] |
| **序列埠除錯 (UART)** | `USART2` (`PA2` TX / `PA3` RX) | 115200 bps 輸出感測器與編碼器即時數值 |

---

## 📂 專案軟體架構 (Project Structure)

本專案採用模組化分層設計，將底層硬體驅動與主決策邏輯分離：

```text
STM32-Autonomous-Navigation-Robot/
├── Core/
│   ├── Inc/
│   │   ├── main.h          # 全域腳位與系統巨集定義[cite: 3]
│   │   ├── motor.h         # 馬達與麥克納姆輪運動函式宣告[cite: 2]
│   │   └── sensor.h        # 循跡與超音波感測器函式宣告[cite: 2]
│   └── Src/
│       ├── main.c          # 系統初始化、編碼器讀取與主控制決策迴圈
│       ├── motor.c         # L298N 底層驅動與全向運動學實作（前進、轉向、左右平移）[cite: 2]
│       └── sensor.c        # TCRT5000 狀態讀取與 HC-SR04 距離演算法[cite: 2]
└── STM32-Autonomous-Navigation-Robot.ioc  # STM32CubeMX 硬體周邊配置檔
