/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint8_t stateS = 0;
uint8_t counter1 = 0;
uint8_t counter2 = 0;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_GPIO_ReadPin(btnS_GPIO_Port, btnS_Pin) == 0){

		  HAL_Delay(500);	// Antirebote del boton
		  stateS = 1;

		  HAL_GPIO_WritePin(GPIOA, ledRS_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, ledYS_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, ledGS_Pin, GPIO_PIN_RESET);
		  HAL_Delay(500);


		  HAL_GPIO_WritePin(GPIOA, ledRS_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, ledYS_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, ledGS_Pin, GPIO_PIN_RESET);
		  HAL_Delay(500);


		  HAL_GPIO_WritePin(GPIOA, ledRS_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, ledYS_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, ledGS_Pin, GPIO_PIN_SET);

		  HAL_GPIO_WritePin(GPIOC, ledwinj1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOC, ledwinj2_Pin, GPIO_PIN_RESET);
		  counter1 = 2;
		  counter2 = 2;

	  }

	  if(stateS == 1){
		  /* Verifica el estado del boton del jugador 1*/
		  if(HAL_GPIO_ReadPin(btnj1_GPIO_Port, btnj1_Pin) == 0){
			  counter1 = counter1 + 1;	// Increment the counter the player 1
			  HAL_Delay(500);	// Antirebote
		  }

		  switch(counter1){
		  case 3:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_SET);
			  break;

		  case 4:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_SET);
			  break;

		  case 5:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_SET);
			  break;

		  case 6:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led4j1_Pin, GPIO_PIN_SET);
			  break;

		  case 7:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led4j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led5j1_Pin, GPIO_PIN_SET);
			  break;

		  case 8:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led4j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led5j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led6j1_Pin, GPIO_PIN_SET);
			  break;

		  case 9:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led4j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led5j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led6j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led7j1_Pin, GPIO_PIN_SET);
			  break;

		  case 10:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led4j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led5j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led6j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led7j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led8j1_Pin, GPIO_PIN_SET);
			  break;

		  default:
			  HAL_GPIO_WritePin(GPIOC, led1j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led2j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led3j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led4j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led5j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led6j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led7j1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, led8j1_Pin, GPIO_PIN_RESET);
			  break;
		  }


		  /* Verifica el estado del boton del jugador 2 */
		  if(HAL_GPIO_ReadPin(btnj2_GPIO_Port, btnj2_Pin) == 0){
			 counter2 = counter2 + 1;
			 HAL_Delay(500);	// Antirebote
		  }

		  switch(counter2){
		  case 3:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_SET);
				  break;

			  case 4:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_SET);
				  break;

			  case 5:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_SET);
				  break;

			  case 6:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led4j2_Pin, GPIO_PIN_SET);
				  break;

			  case 7:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led4j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led5j2_Pin, GPIO_PIN_SET);
				  break;

			  case 8:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led4j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led5j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led6j2_Pin, GPIO_PIN_SET);
				  break;

			  case 9:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led4j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led5j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led6j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led7j2_Pin, GPIO_PIN_SET);
				  break;

			  case 10:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led4j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led5j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led6j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led7j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led8j2_Pin, GPIO_PIN_SET);
				  break;

			  default:
				  HAL_GPIO_WritePin(GPIOB, led1j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led2j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led3j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led4j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led5j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led6j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led7j2_Pin, GPIO_PIN_RESET);
				  HAL_GPIO_WritePin(GPIOB, led8j2_Pin, GPIO_PIN_RESET);
				  break;
		  }


		  if(counter1 >= 10 && counter2 <= 10){
			  /* Contador1 gana */
			  HAL_GPIO_WritePin(GPIOC, ledwinj1_Pin, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(GPIOC, ledwinj2_Pin, GPIO_PIN_RESET);
			  counter2 = 0;	/* Bloquea al jugador 2 */
		  } else if(counter2 >= 10 && counter1 <= 10){
			  /* Contador2 gana */
			  HAL_GPIO_WritePin(GPIOC, ledwinj1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(GPIOC, ledwinj2_Pin, GPIO_PIN_SET);
			  counter1 = 0;	/* Bloquea al jugador 1 */
		  }
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, led1j1_Pin|led2j1_Pin|led3j1_Pin|led4j1_Pin
                          |led5j1_Pin|led6j1_Pin|led7j1_Pin|led8j1_Pin
                          |ledwinj1_Pin|ledwinj2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ledRS_Pin|ledYS_Pin|ledGS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, led1j2_Pin|led2j2_Pin|led3j2_Pin|led6j2_Pin
                          |led7j2_Pin|led8j2_Pin|led4j2_Pin|led5j2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : led1j1_Pin led2j1_Pin led3j1_Pin led4j1_Pin
                           led5j1_Pin led6j1_Pin led7j1_Pin led8j1_Pin
                           ledwinj1_Pin ledwinj2_Pin */
  GPIO_InitStruct.Pin = led1j1_Pin|led2j1_Pin|led3j1_Pin|led4j1_Pin
                          |led5j1_Pin|led6j1_Pin|led7j1_Pin|led8j1_Pin
                          |ledwinj1_Pin|ledwinj2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : ledRS_Pin ledYS_Pin ledGS_Pin */
  GPIO_InitStruct.Pin = ledRS_Pin|ledYS_Pin|ledGS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : led1j2_Pin led2j2_Pin led3j2_Pin led6j2_Pin
                           led7j2_Pin led8j2_Pin led4j2_Pin led5j2_Pin */
  GPIO_InitStruct.Pin = led1j2_Pin|led2j2_Pin|led3j2_Pin|led6j2_Pin
                          |led7j2_Pin|led8j2_Pin|led4j2_Pin|led5j2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : btnj2_Pin btnj1_Pin btnS_Pin */
  GPIO_InitStruct.Pin = btnj2_Pin|btnj1_Pin|btnS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */