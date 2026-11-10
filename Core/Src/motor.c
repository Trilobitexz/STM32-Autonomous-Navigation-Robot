/* USER CODE BEGIN Includes */
#include "motor.h"
#include "sensor.h"
#include <stdio.h>
/* USER CODE END Includes */

/* USER CODE BEGIN 0 */
// 由於你不再需要 UART，這個 _write 函式可以移除或保留，不會影響功能
/* USER CODE END 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 檢查是否由霍爾感測器 (PC5) 觸發中斷
    if(GPIO_Pin == HALL_SENSOR_Pin)
    {
        // 點亮 PA5 上的 LED 燈 (偵測到磁場)
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

        // 為了讓你用肉眼看到閃爍，加入短暫延遲 (約 1 秒)
        // 注意：在正式開發中，中斷內不應該有長時間的 delay 或空迴圈
        for(volatile int i = 0; i < 1000000; i++);

        // 熄滅 LED 燈
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  // 啟動 4 個輪子的 PWM 通道
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    // 程式會在這個無窮迴圈中等待
    // 當霍爾感測器觸發時，會自動跳到上方的 HAL_GPIO_EXTI_Callback 執行
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
