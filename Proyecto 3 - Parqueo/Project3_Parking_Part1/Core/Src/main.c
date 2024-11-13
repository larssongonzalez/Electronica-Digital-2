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
#include <string.h>
#include <stdio.h>
#include "neo_pixel.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUMBER_OF_LEDS 8

#define ESP32_ADDRESS 0x20

#define NUM_SENSORS 4 // Número de sensores

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint16_t rawValues[4];
int adc_channel_count = sizeof(rawValues)/sizeof(rawValues[0]);
uint8_t adc_conv_complete_flag = 0;
char buffer[100];
uint8_t current_index = 0; // Indice para seleccionar el valor de rawValues
uint8_t data_to_send;      // Variable para almacenar el valor a enviar
int contador = 0;	// Contador del display
// Array para almacenar los contadores de cada sensor
int contadorDisponibles = 8; // Inicializar en 8 espacios disponibles
int estadoSensores[NUM_SENSORS] = {0}; // Estado de cada sensor (0: desocupado, 1: ocupado)
//int contadores[NUM_SENSORS] = {0};
uint8_t uart5_rx_buffer[50]; // Buffer para almacenar los 8 caracteres
uint8_t rx_index = 0;  // Índice de seguimiento


neopixel_led leds[NUMBER_OF_LEDS + 1];
neopixel_led circular_pattern_led[2 * NUMBER_OF_LEDS + 1];
rgb_color led_pattern[NUMBER_OF_LEDS];

neopixel_led temp_neo_pixel;
rgb_color red_color = {255, 0, 0};
rgb_color green_color = {0, 255, 0};
rgb_color blue_color = {0, 0, 255};
rgb_color black_color = {0, 0, 0};
uint16_t led_pos_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */
void send_i2c_command(char* command);
void send_uart_command(UART_HandleTypeDef *huart, char* command);
void actualizarDisplay(int valor);
void actualizarContador();
void mostrarContadorEnDisplay();
//void procesarDatos(uint8_t* datos);
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
	/*neopixel_led temp_neo_pixel;
	rgb_color red_color = {255, 0, 0};
	rgb_color green_color = {0, 255, 0};
	rgb_color blue_color = {0, 0, 255};
	rgb_color black_color = {0, 0, 0};
	uint16_t led_pos_counter = 0;*/

	/*// Patterns
	for(int i = 0; i < NUMBER_OF_LEDS; i++){
		led_pattern[i].b = (i%2) * 255;
	}

	// Some pattern
	led_pattern[0].b = 127;
	led_pattern[0].r = (50);
	led_pattern[0].g = 0;
	led_pattern[1].b = 255;
	led_pattern[1].r = (100);
	led_pattern[1].g = 0;
	led_pattern[2].b = 0;
	led_pattern[2].r = (150);
	led_pattern[2].g = 127;
	led_pattern[3].b = 0;
	led_pattern[3].r = (150);
	led_pattern[3].g = 255;

	// Setting the pattern
	set_pattern_led(circular_pattern_led, led_pattern, NUMBER_OF_LEDS);
	set_pattern_led(circular_pattern_led + NUMBER_OF_LEDS, led_pattern, NUMBER_OF_LEDS);*/

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)rawValues, adc_channel_count);
  HAL_UART_Receive_IT(&huart5, (uint8_t*)uart5_rx_buffer, 8); // Recibe los 8 caracteres

  HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_RESET);




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(adc_conv_complete_flag == 1){
		  snprintf(buffer, 100, "S1: %d, S2: %d, S3: %d, S4: %d\r\n", rawValues[0], rawValues[1], rawValues[2], rawValues[3]);
		  //HAL_UART_Transmit(&huart2, (uint8_t*)buffer, sizeof(buffer), HAL_MAX_DELAY);


		  // Actualizar el contador de espacios disponibles
		  actualizarContador();

		  // Mostrar el contador en el display
		  mostrarContadorEnDisplay();



		  if(rawValues[0] < 150){
			  send_i2c_command("S1_LOW");
			  send_uart_command(&huart5, "A"); // Enviar comando A por UART5
			  reset_specific_led(leds, 0);
			  set_specific_led(leds, 0, &red_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			  HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_RESET);  // Apagar LED verde

		  } else{
			  send_i2c_command("S1_HIGH");
			  send_uart_command(&huart5, "B"); // Enviar comando B por UART5
			  reset_specific_led(leds, 0);
			  set_specific_led(leds, 0, &green_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			  HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_RESET);  // Apagar LED rojo

		  }

		  if(rawValues[1] < 150){
			  reset_specific_led(leds, 1);
			  set_specific_led(leds, 1, &red_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			  send_i2c_command("S2_LOW");
			  send_uart_command(&huart5, "C"); // Enviar comando C por UART5
			  //HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_SET);
			  //HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_RESET);  // Apagar LED verde

		  } else{
			  send_i2c_command("S2_HIGH");
			  send_uart_command(&huart5, "D"); // Enviar comando D por UART5
			  //HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_SET);
			  //HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_RESET);  // Apagar LED rojo
			  reset_specific_led(leds, 1);
			  set_specific_led(leds, 1, &green_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
		  }

		  if(rawValues[2] < 150){
			  send_i2c_command("S3_LOW");
			  send_uart_command(&huart5, "E"); // Enviar comando E por UART5
			  //HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_SET);
			  //HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_RESET);  // Apagar LED verde
			  reset_specific_led(leds, 2);
			  set_specific_led(leds, 2, &red_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs

		  } else {
			  send_i2c_command("S3_HIGH");
			  send_uart_command(&huart5, "F"); // Enviar comando F por UART5
			  //HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_SET);
			  //HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_RESET);  // Apagar LED rojo
			  reset_specific_led(leds, 2);
			  set_specific_led(leds, 2, &green_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
		  }

		  if(rawValues[3] < 150){
			  send_i2c_command("S4_LOW");
			  send_uart_command(&huart5, "G"); // Enviar comando D por UART5
			  //HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_SET);
			  //HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_RESET);  // Apagar LED verde
			  reset_specific_led(leds, 3);
			  set_specific_led(leds, 3, &red_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs

		  } else{
			  send_i2c_command("S4_HIGH");
			  send_uart_command(&huart5, "H"); // Enviar comando D por UART5
			  //HAL_GPIO_WritePin(led_verde_GPIO_Port, led_verde_Pin, GPIO_PIN_SET);
			  //HAL_GPIO_WritePin(led_rojo_GPIO_Port, led_rojo_Pin, GPIO_PIN_RESET);  // Apagar LED rojo
			  reset_specific_led(leds, 3);
			  set_specific_led(leds, 3, &green_color); // Establecer el primer LED en rojo
			  HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
		  }


		  adc_conv_complete_flag = 0;
		  // delay for 500 Milliseconds
		  HAL_Delay(500);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 160;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 3;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 24;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, segc_Pin|segb_Pin|sega_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, led_rojo_Pin|led_verde_Pin|segg_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, segf_Pin|sege_Pin|segd_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : segc_Pin segb_Pin sega_Pin */
  GPIO_InitStruct.Pin = segc_Pin|segb_Pin|sega_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : led_rojo_Pin led_verde_Pin segg_Pin */
  GPIO_InitStruct.Pin = led_rojo_Pin|led_verde_Pin|segg_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : segf_Pin sege_Pin segd_Pin */
  GPIO_InitStruct.Pin = segf_Pin|sege_Pin|segd_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/****************************************Interrupcion para Sensores**********************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {

    	adc_conv_complete_flag = 1;

    }
}

/*************************************Interrupcion para Recepcion de Caracteres**********************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART5) {
    	if(uart5_rx_buffer[0] == 'i'){	// Recibe comando 'U'
				char* data = "i\n";
				send_i2c_command("S5_LOW");
				contadorDisponibles--; // Decrementar si se ocupa
				reset_specific_led(leds, 4);
				set_specific_led(leds, 4, &red_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs

			}else if(uart5_rx_buffer[0] == 'j'){	// Recibe comando 'U'
				char* data = "j\n";
				send_i2c_command("S5_HIGH");
				contadorDisponibles++; // Incrementar si se desocupa
				reset_specific_led(leds, 4);
				set_specific_led(leds, 4, &green_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}
			if(uart5_rx_buffer[0] == 'k'){	// Recibe comando 'U'
				char* data = "k\n";
				send_i2c_command("S6_LOW");
				contadorDisponibles--; // Decrementar si se ocupa
				reset_specific_led(leds, 5);
				set_specific_led(leds, 5, &red_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}else if(uart5_rx_buffer[0] == 'l'){	// Recibe comando 'U'
				char* data = "l\n";
				send_i2c_command("S6_HIGH");
				contadorDisponibles++; // Incrementar si se desocupa
				reset_specific_led(leds, 5);
				set_specific_led(leds, 5, &green_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}
			if(uart5_rx_buffer[0] == 'm'){	// Recibe comando 'U'
				char* data = "m\n";
				send_i2c_command("S7_LOW");
				contadorDisponibles--; // Decrementar si se ocupa
				reset_specific_led(leds, 6);
				set_specific_led(leds, 6, &red_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}else if(uart5_rx_buffer[0] == 'n'){	// Recibe comando 'U'
				char* data = "n\n";
				send_i2c_command("S7_HIGH");
				contadorDisponibles++; // Incrementar si se desocupa
				reset_specific_led(leds, 6);
				set_specific_led(leds, 6, &green_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}
			if(uart5_rx_buffer[0] == 'o'){	// Recibe comando 'U'
				char* data = "o\n";
				send_i2c_command("S8_LOW");
				contadorDisponibles--; // Decrementar si se ocupa
				reset_specific_led(leds, 7);
				set_specific_led(leds, 7, &red_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}else if(uart5_rx_buffer[0] == 'p'){	// Recibe comando 'U'
				char* data = "p\n";
				send_i2c_command("S8_HIGH");
				contadorDisponibles++; // Incrementar si se desocupa
				reset_specific_led(leds, 7);
				set_specific_led(leds, 7, &green_color); // Establecer el primer LED en rojo
				HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
			}
    	}

        // Habilitamos la recepción nuevamente
        HAL_UART_Receive_IT(&huart5, (uint8_t*)uart5_rx_buffer, 1); // Recibimos 8 caracteres
}


/****************************************Interrupcion para NeoPixel**********************************************/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)	{
	HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
	htim3.Instance->CCR1 = 0;
}

// Función para enviar un comando I2C
void send_i2c_command(char* command) {
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(ESP32_ADDRESS << 1), (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
}

void send_uart_command(UART_HandleTypeDef *huart, char* command) {
    HAL_UART_Transmit(huart, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
}

// Función para actualizar el contador de un sensor específico
void actualizarContador() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        // Determinar el estado actual del sensor
        int nuevoEstado = (rawValues[i] < 150) ? 1 : 0; // 1: ocupado, 0: desocupado

        // Si el estado del sensor ha cambiado
        if (nuevoEstado != estadoSensores[i]) {
            // Actualizar el contador de espacios disponibles
            if (nuevoEstado == 1) {
                contadorDisponibles--; // Decrementar si se ocupa
            } else {
                contadorDisponibles++; // Incrementar si se desocupa
            }
        }

        // Actualizar el estado del sensor
        estadoSensores[i] = nuevoEstado;
    }
}

// Función para mostrar el valor del contador en el display
void mostrarContadorEnDisplay() {
    actualizarDisplay(contadorDisponibles); // Mostrar el número de espacios disponibles
}

/* Función para actualizar el display de 7 segmentos */
void actualizarDisplay(int valor) {
    switch (valor) {
        case 0: // Código para mostrar 0 en el display
            HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_RESET);
            break;
        case 1: // Código para mostrar 1 en el display
            HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_RESET);
            break;
        case 2:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;
        case 3:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;
        case 4:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;
        case 5:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;
        case 6:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;
        case 7:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_RESET);
			break;
        case 8:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;
        case 9:
        	HAL_GPIO_WritePin(sega_GPIO_Port, sega_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segb_GPIO_Port, segb_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segc_GPIO_Port, segc_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segd_GPIO_Port, segd_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(sege_GPIO_Port, sege_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(segf_GPIO_Port, segf_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(segg_GPIO_Port, segg_Pin, GPIO_PIN_SET);
			break;

        default:
            break;
    }
}

/*void procesarDatos(uint8_t* datos)
{
    // Procesar los 8 caracteres recibidos (i, j, k, l, m, n, o, p)
    for (int i = 0; i < 4; i++) {
        uint8_t sensor_estado = datos[i];   // Cada uno de los caracteres i, j, k, l, m, n, o, p

        // Analizamos el estado de cada sensor
        switch (sensor_estado) {
            case 'i': // El sensor 1 está debajo del umbral
            	send_i2c_command("S5_LOW");
            	contadorDisponibles--; // Decrementar si se ocupa
                break;
            case 'j': // El sensor 1 no está debajo del umbral
            	send_i2c_command("S5_HIGH");
            	contadorDisponibles++; // Incrementar si se desocupa
                break;
            case 'k': // El sensor 2 está debajo del umbral
            	send_i2c_command("S6_LOW");
            	contadorDisponibles--; // Decrementar si se ocupa
                break;
            case 'l': // El sensor 2 no está debajo del umbral
            	send_i2c_command("S6_HIGH");
            	contadorDisponibles++; // Incrementar si se desocupa
                break;
            case 'm': // El sensor 3 está debajo del umbral
            	send_i2c_command("S7_LOW");
            	contadorDisponibles--; // Decrementar si se ocupa
                break;
            case 'n': // El sensor 3 no está debajo del umbral
            	send_i2c_command("S7_HIGH");
            	contadorDisponibles++; // Incrementar si se desocupa
                break;
            case 'o': // El sensor 4 está debajo del umbral
            	send_i2c_command("S8_LOW");
            	contadorDisponibles--; // Decrementar si se ocupa
                break;
            case 'p': // El sensor 4 no está debajo del umbral
            	send_i2c_command("S8_HIGH");
            	contadorDisponibles++; // Incrementar si se desocupa
                break;
            default:
                // Si el valor es inesperado
                break;
        }

        mostrarContadorEnDisplay();
    }
}*/


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
