/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
  * Universidad Del Valle De Guatemala
  * IE2023: Electrónica digital 2
  * Autor: James Ramírez - 22538
  * Proyecto: Proyecto 3 - Servidor Web y Nucleo
  * Hardware: STM32
  * Creado: 29/10/2024
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "bitmaps.h"
#include "neo_pixel.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define NUMBER_OF_LEDS 4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

uint8_t rx_data[100]; // Solo para recibir byte por byte

char buffer[100];
uint8_t i2c_tx_data[8];
uint8_t i2c_tx_command_flag1 = 0;
uint8_t i2c_tx_command_flag2 = 0;
uint8_t i2c_tx_command_flag3 = 0;
uint8_t i2c_tx_command_flag4 = 0;
uint8_t i2c_command1 = 'w';
uint8_t i2c_command2 = 'x';
uint8_t i2c_command3 = 'y';
uint8_t i2c_command4 = 'z';



int contador = 8;

int c1_1 = 0;
int c1_2 = 0;

int c2_1 = 0;
int c2_2 = 0;

int c3_1 = 0;
int c3_2 = 0;

int c4_1 = 0;
int c4_2 = 0;

int pin1 = 0;
int pin2 = 0;
int pin3 = 0;
int pin4 = 0;

int pin5 = 0;
int pin6 = 0;
int pin7 = 0;
int pin8 = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */

void send_uart_command(UART_HandleTypeDef *huart, char* command);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void movercarro1 (){

	LCD_Bitmap(16, 122, 42, 26, semrojoabajo16_122_42x26);

	LCD_Bitmap(23, 235, 27, 5, carro1_1);
	HAL_Delay(20);
	LCD_Bitmap(20, 225, 35, 15, carro1_2);
	HAL_Delay(20);
	LCD_Bitmap(20, 215, 35, 25, carro1_3);
	HAL_Delay(20);
	LCD_Bitmap(20, 205, 35, 35, carro1_4);
	HAL_Delay(20);
	LCD_Bitmap(20, 195, 35, 45, carro1_5);
	HAL_Delay(20);
	LCD_Bitmap(19, 185, 37, 55, carro1_6);
	HAL_Delay(20);
	LCD_Bitmap(19, 175, 37, 65, carro1_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(19, 165 - i, 37, 75, carro1_8);
		FillRect (19, 240 - i, 37, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro1 (){
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(19, 158 + i, 37, 75, carro1_8);
		FillRect (19, 158 + i, 37, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (19, 165, 37, 10, 0x00);
	LCD_Bitmap(19, 175, 37, 65, carro1_7);
	HAL_Delay(20);
	FillRect (19, 175, 37, 10, 0x00);
	LCD_Bitmap(19, 185, 37, 55, carro1_6);
	HAL_Delay(20);
	FillRect (19, 185, 37, 10, 0x00);
	LCD_Bitmap(20, 195, 35, 45, carro1_5);
	HAL_Delay(20);
	FillRect (19, 195, 37, 10, 0x00);
	LCD_Bitmap(20, 205, 35, 35, carro1_4);
	HAL_Delay(20);
	FillRect (19, 205, 37, 10, 0x00);
	LCD_Bitmap(20, 215, 35, 25, carro1_3);
	HAL_Delay(20);
	FillRect (19, 215, 37, 10, 0x00);
	LCD_Bitmap(20, 225, 35, 15, carro1_2);
	HAL_Delay(20);
	FillRect (19, 225, 37, 10, 0x00);
	LCD_Bitmap(20, 235, 27, 5, carro1_1);
	HAL_Delay(20);
	FillRect (23, 235, 37, 10, 0x00);

	LCD_Bitmap(16, 122, 42, 26, semverdeabajo16_122_42x26);

	incrementarcontador();

}

void movercarro2(){

	LCD_Bitmap(76, 122, 42, 26, semrojoabajo16_122_42x26);

	LCD_Bitmap(82, 235, 27, 5, carro2_1);
	HAL_Delay(20);
	LCD_Bitmap(78, 225, 35, 15, carro2_2);
	HAL_Delay(20);
	LCD_Bitmap(78, 215, 35, 25, carro2_3);
	HAL_Delay(20);
	LCD_Bitmap(78, 205, 35, 35, carro2_4);
	HAL_Delay(20);
	LCD_Bitmap(78, 195, 35, 45, carro2_5);
	HAL_Delay(20);
	LCD_Bitmap(78, 185, 35, 55, carro2_6);
	HAL_Delay(20);
	LCD_Bitmap(77, 175, 37, 65, carro2_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(77, 165 - i, 37, 75, carro2_8);
		FillRect (77, 240 - i, 37, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro2 (){
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(77, 158 + i, 37, 75, carro2_8);
		FillRect (77, 158 + i, 37, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (77, 165, 37, 10, 0x00);
	LCD_Bitmap(77, 175, 37, 65, carro2_7);
	HAL_Delay(20);
	FillRect (77, 175, 37, 10, 0x00);
	LCD_Bitmap(78, 185, 35, 55, carro2_6);
	HAL_Delay(20);
	FillRect (77, 185, 37, 10, 0x00);
	LCD_Bitmap(78, 195, 35, 45, carro2_5);
	HAL_Delay(20);
	FillRect (77, 195, 37, 10, 0x00);
	LCD_Bitmap(78, 205, 35, 35, carro2_4);
	HAL_Delay(20);
	FillRect (77, 205, 37, 10, 0x00);
	LCD_Bitmap(78, 215, 35, 25, carro2_3);
	HAL_Delay(20);
	FillRect (77, 215, 37, 10, 0x00);
	LCD_Bitmap(78, 225, 35, 15, carro2_2);
	HAL_Delay(20);
	FillRect (77, 225, 37, 10, 0x00);
	LCD_Bitmap(82, 235, 27, 5, carro2_1);
	HAL_Delay(20);
	FillRect (77, 235, 37, 10, 0x00);

	LCD_Bitmap(76, 122, 42, 26, semverdeabajo16_122_42x26);

	incrementarcontador();
}

void movercarro3(){

	LCD_Bitmap(135, 122, 42, 26, semrojoabajo16_122_42x26);

	LCD_Bitmap(143, 235, 26, 5, carro3_1);
	HAL_Delay(20);
	LCD_Bitmap(140, 225, 32, 15, carro3_2);
	HAL_Delay(20);
	LCD_Bitmap(140, 215, 32, 25, carro3_3);
	HAL_Delay(20);
	LCD_Bitmap(140, 205, 32, 35, carro3_4);
	HAL_Delay(20);
	LCD_Bitmap(139, 195, 34, 45, carro3_5);
	HAL_Delay(20);
	LCD_Bitmap(139, 185, 34, 55, carro3_6);
	HAL_Delay(20);
	LCD_Bitmap(139, 175, 34, 65, carro3_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(139, 165 - i, 34, 75, carro3_8);
		FillRect (139, 240 - i, 34, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro3 (){
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(139, 158 + i, 34, 75, carro3_8);
		FillRect (139, 158 + i, 34, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (139, 165, 34, 10, 0x00);
	LCD_Bitmap(139, 175, 34, 65, carro3_7);
	HAL_Delay(20);
	FillRect (139, 175, 34, 10, 0x00);
	LCD_Bitmap(139, 185, 34, 55, carro3_6);
	HAL_Delay(20);
	FillRect (139, 185, 34, 10, 0x00);
	LCD_Bitmap(139, 195, 34, 45, carro3_5);
	HAL_Delay(20);
	FillRect (139, 195, 34, 10, 0x00);
	LCD_Bitmap(140, 205, 32, 35, carro3_4);
	HAL_Delay(20);
	FillRect (139, 205, 34, 10, 0x00);
	LCD_Bitmap(140, 215, 32, 25, carro3_3);
	HAL_Delay(20);
	FillRect (139, 215, 34, 10, 0x00);
	LCD_Bitmap(140, 225, 32, 15, carro3_2);
	HAL_Delay(20);
	FillRect (139, 225, 34, 10, 0x00);
	LCD_Bitmap(143, 235, 26, 5, carro3_1);
	HAL_Delay(20);
	FillRect (139, 235, 37, 10, 0x00);

	LCD_Bitmap(135, 122, 42, 26, semverdeabajo16_122_42x26);

	incrementarcontador();

}

void movercarro4(){

	LCD_Bitmap(194, 122, 42, 26, semrojoabajo16_122_42x26);

	LCD_Bitmap(203, 235, 24, 5, carro4_1);
	HAL_Delay(20);
	LCD_Bitmap(197, 225, 36, 15, carro4_2);
	HAL_Delay(20);
	LCD_Bitmap(197, 215, 36, 25, carro4_3);
	HAL_Delay(20);
	LCD_Bitmap(197, 205, 36, 35, carro4_4);
	HAL_Delay(20);
	LCD_Bitmap(197, 195, 36, 45, carro4_5);
	HAL_Delay(20);
	LCD_Bitmap(197, 185, 36, 55, carro4_6);
	HAL_Delay(20);
	LCD_Bitmap(197, 175, 36, 65, carro4_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(197, 165 - i, 36, 75, carro4_8);
		FillRect (197, 240 - i, 36, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro4(){

	for(int i=0; i <= 7; i++){
		LCD_Bitmap(197, 158 + i, 36, 75, carro4_8);
		FillRect (197, 158 + i, 36, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (197, 165, 37, 10, 0x00);
	LCD_Bitmap(197, 175, 36, 65, carro4_7);
	HAL_Delay(20);
	FillRect (197, 175, 37, 10, 0x00);
	LCD_Bitmap(197, 185, 36, 55, carro4_6);
	HAL_Delay(20);
	FillRect (197, 185, 37, 10, 0x00);
	LCD_Bitmap(197, 195, 36, 45, carro4_5);
	HAL_Delay(20);
	FillRect (197, 195, 37, 10, 0x00);
	LCD_Bitmap(197, 205, 36, 35, carro4_4);
	HAL_Delay(20);
	FillRect (197, 205, 37, 10, 0x00);
	LCD_Bitmap(197, 215, 36, 25, carro4_3);
	HAL_Delay(20);
	FillRect (197, 215, 37, 10, 0x00);
	LCD_Bitmap(197, 225, 36, 15, carro4_2);
	HAL_Delay(20);
	FillRect (197, 225, 37, 10, 0x00);
	LCD_Bitmap(203, 235, 24, 5, carro4_1);
	HAL_Delay(20);
	FillRect (197, 235, 37, 10, 0x00);

	LCD_Bitmap(194, 122, 42, 26, semverdeabajo16_122_42x26);

	incrementarcontador();
}

void movercarro5(){

	LCD_Bitmap(194, 94, 42, 26, semrojoarriba16_94_42x26);

	LCD_Bitmap(198, 0, 34, 5, carro5_1);
	HAL_Delay(20);
	LCD_Bitmap(198, 0, 34, 15, carro5_2);
	HAL_Delay(20);
	LCD_Bitmap(198, 0, 34, 25, carro5_3);
	HAL_Delay(20);
	LCD_Bitmap(198, 0, 34, 35, carro5_4);
	HAL_Delay(20);
	LCD_Bitmap(198, 0, 34, 45, carro5_5);
	HAL_Delay(20);
	LCD_Bitmap(198, 0, 34, 55, carro5_6);
	HAL_Delay(20);
	LCD_Bitmap(198, 0, 34, 65, carro5_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(198, 0 + i, 34, 75, carro5_8);
		FillRect (198, 0 + i, 34, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro5(){

	for(int i=0; i <= 7; i++){
		LCD_Bitmap(198, 7 - i, 34, 75, carro5_8);
		FillRect (198, 82 - i, 34, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (198, 65, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 65, carro5_7);
	HAL_Delay(20);
	FillRect (198, 55, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 55, carro5_6);
	HAL_Delay(20);
	FillRect (198, 45, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 45, carro5_5);
	HAL_Delay(20);
	FillRect (198, 35, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 35, carro5_4);
	HAL_Delay(20);
	FillRect (198, 25, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 25, carro5_3);
	HAL_Delay(20);
	FillRect (198, 15, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 15, carro5_2);
	HAL_Delay(20);
	FillRect (198, 5, 34, 10, 0x00);
	LCD_Bitmap(198, 0, 34, 5, carro5_1);
	HAL_Delay(20);
	FillRect (198, 0, 34, 10, 0x00);

	LCD_Bitmap(194, 94, 42, 26, semverdearriba16_94_42x26);

	incrementarcontador();
}

void movercarro6(){

	LCD_Bitmap(135, 94, 42, 26, semrojoarriba16_94_42x26);

	LCD_Bitmap(138, 0, 37, 5, carro6_1);
	HAL_Delay(20);
	LCD_Bitmap(138, 0, 37, 15, carro6_2);
	HAL_Delay(20);
	LCD_Bitmap(138, 0, 37, 25, carro6_3);
	HAL_Delay(20);
	LCD_Bitmap(138, 0, 37, 35, carro6_4);
	HAL_Delay(20);
	LCD_Bitmap(138, 0, 37, 45, carro6_5);
	HAL_Delay(20);
	LCD_Bitmap(138, 0, 37, 55, carro6_6);
	HAL_Delay(20);
	LCD_Bitmap(138, 0, 37, 65, carro6_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(138, 0 + i, 37, 75, carro6_8);
		FillRect (138, 0 + i, 37, 1, 0x00);
	}
	decrementarcontador();
}

void quitarcarro6(){

	for(int i=0; i <= 7; i++){
		LCD_Bitmap(138, 7 - i, 37, 75, carro6_8);
		FillRect (138, 82 - i, 37, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (138, 65, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 65, carro6_7);
	HAL_Delay(20);
	FillRect (138, 55, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 55, carro6_6);
	HAL_Delay(20);
	FillRect (138, 45, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 45, carro6_5);
	HAL_Delay(20);
	FillRect (138, 35, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 35, carro6_4);
	HAL_Delay(20);
	FillRect (138, 25, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 25, carro6_3);
	HAL_Delay(20);
	FillRect (138, 15, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 15, carro6_2);
	HAL_Delay(20);
	FillRect (138, 5, 37, 10, 0x00);
	LCD_Bitmap(138, 0, 37, 5, carro6_1);
	HAL_Delay(20);
	FillRect (138, 0, 37, 10, 0x00);

	LCD_Bitmap(135, 94, 42, 26, semverdearriba16_94_42x26);

	incrementarcontador();
}

void movercarro7(){

	LCD_Bitmap(76, 94, 42, 26, semrojoarriba16_94_42x26);

	LCD_Bitmap(82, 0, 27, 5, carro7_1);
	HAL_Delay(20);
	LCD_Bitmap(78, 0, 35, 15, carro7_2);
	HAL_Delay(20);
	LCD_Bitmap(78, 0, 35, 25, carro7_3);
	HAL_Delay(20);
	LCD_Bitmap(78, 0, 35, 35, carro7_4);
	HAL_Delay(20);
	LCD_Bitmap(78, 0, 35, 45, carro7_5);
	HAL_Delay(20);
	LCD_Bitmap(78, 0, 35, 55, carro7_6);
	HAL_Delay(20);
	LCD_Bitmap(78, 0, 35, 65, carro7_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(78, 0 + i, 35, 75, carro7_8);
		FillRect (78, 0 + i, 35, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro7(){

	for(int i=0; i <= 7; i++){
		LCD_Bitmap(78, 7 - i, 35, 75, carro7_8);
		FillRect (78, 82 - i, 35, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (78, 65, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 65, carro7_7);
	HAL_Delay(20);
	FillRect (78, 55, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 55, carro7_6);
	HAL_Delay(20);
	FillRect (78, 45, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 45, carro7_5);
	HAL_Delay(20);
	FillRect (78, 35, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 35, carro7_4);
	HAL_Delay(20);
	FillRect (78, 25, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 25, carro7_3);
	HAL_Delay(20);
	FillRect (78, 15, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 15, carro7_2);
	HAL_Delay(20);
	FillRect (78, 5, 35, 10, 0x00);
	LCD_Bitmap(78, 0, 35, 5, carro7_1);
	HAL_Delay(20);
	FillRect (78, 0, 35, 10, 0x00);

	LCD_Bitmap(76, 94, 42, 26, semverdearriba16_94_42x26);

	incrementarcontador();
}

void movercarro8(){

	LCD_Bitmap(16, 94, 42, 26, semrojoarriba16_94_42x26);

	LCD_Bitmap(18, 0, 39, 5, carro8_1);
	HAL_Delay(20);
	LCD_Bitmap(18, 0, 39, 15, carro8_2);
	HAL_Delay(20);
	LCD_Bitmap(18, 0, 39, 25, carro8_3);
	HAL_Delay(20);
	LCD_Bitmap(18, 0, 39, 35, carro8_4);
	HAL_Delay(20);
	LCD_Bitmap(18, 0, 39, 45, carro8_5);
	HAL_Delay(20);
	LCD_Bitmap(18, 0, 39, 55, carro8_6);
	HAL_Delay(20);
	LCD_Bitmap(18, 0, 39, 65, carro8_7);
	HAL_Delay(20);
	for(int i=0; i <= 7; i++){
		LCD_Bitmap(18, 0 + i, 39, 75, carro8_8);
		FillRect (18, 0 + i, 39, 1, 0x00);
	}

	decrementarcontador();
}

void quitarcarro8(){

	for(int i=0; i <= 7; i++){
		LCD_Bitmap(18, 7 - i, 39, 75, carro8_8);
		FillRect (18, 82 - i, 39, 1, 0x00);
	}
	HAL_Delay(20);
	FillRect (18, 65, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 65, carro8_7);
	HAL_Delay(20);
	FillRect (18, 55, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 55, carro8_6);
	HAL_Delay(20);
	FillRect (18, 45, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 45, carro8_5);
	HAL_Delay(20);
	FillRect (18, 35, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 35, carro8_4);
	HAL_Delay(20);
	FillRect (18, 25, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 25, carro8_3);
	HAL_Delay(20);
	FillRect (18, 15, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 15, carro8_2);
	HAL_Delay(20);
	FillRect (18, 5, 39, 10, 0x00);
	LCD_Bitmap(18, 0, 39, 5, carro8_1);
	HAL_Delay(20);
	FillRect (18, 0, 39, 10, 0x00);

	LCD_Bitmap(16, 94, 42, 26, semverdearriba16_94_42x26);

	incrementarcontador();
}

void parqueos_disponibles(){
	if(contador == 8){
		LCD_Bitmap(261, 90, 50, 60, n8);
	}else if(contador == 7){
		LCD_Bitmap(261, 90, 50, 60, n7);
	}else if(contador == 6){
		LCD_Bitmap(261, 90, 50, 60, n6);
	}else if(contador == 5){
		LCD_Bitmap(261, 90, 50, 60, n5);
	}else if(contador == 4){
		LCD_Bitmap(261, 90, 50, 60, n4);
	}else if(contador == 3){
		LCD_Bitmap(261, 90, 50, 60, n3);
	}else if(contador == 2){
		LCD_Bitmap(261, 90, 50, 60, n2);
	}else if(contador == 1){
		LCD_Bitmap(261, 90, 50, 60, n1);
	}else if(contador == 0){
		LCD_Bitmap(261, 90, 50, 60, n0);
	}
}

void incrementarcontador() {
    if (contador < 8) {
        contador++;
    }
}

void decrementarcontador() {
    if (contador > 0) {
        contador--;
    }
}

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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart5,rx_data, 1);  // Empezar la recepción por UART1 en modo interrupción


	LCD_Init();

	LCD_Clear(0x00);
	LCD_Bitmap(0, 0, 252, 240, fondoparking);
	LCD_Bitmap(16, 94, 42, 26, semverdearriba16_94_42x26);
	LCD_Bitmap(16, 122, 42, 26, semverdeabajo16_122_42x26);
	LCD_Bitmap(76, 94, 42, 26, semverdearriba16_94_42x26);
	LCD_Bitmap(76, 122, 42, 26, semverdeabajo16_122_42x26);
	LCD_Bitmap(135, 94, 42, 26, semverdearriba16_94_42x26);
	LCD_Bitmap(135, 122, 42, 26, semverdeabajo16_122_42x26);
	LCD_Bitmap(194, 94, 42, 26, semverdearriba16_94_42x26);
	LCD_Bitmap(194, 122, 42, 26, semverdeabajo16_122_42x26);
	LCD_Bitmap(261, 90, 50, 60, n8);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {



		if ((HAL_GPIO_ReadPin(carro1_GPIO_Port, carro1_Pin) == 0 && pin1 == 0)) {
			HAL_Delay(15);
			movercarro1();
			parqueos_disponibles();
			send_uart_command(&huart5, "i");
			pin1++;
		}else if ((HAL_GPIO_ReadPin(carro1_GPIO_Port, carro1_Pin) == 0) && pin1 == 1) {
			HAL_Delay(15);
			quitarcarro1();
			parqueos_disponibles();
			send_uart_command(&huart5, "j");
			pin1 = 0;
		}

		if ((HAL_GPIO_ReadPin(carro2_GPIO_Port, carro2_Pin) == 0 && pin2 == 0)) {
			HAL_Delay(15);
			movercarro2();
			parqueos_disponibles();
			send_uart_command(&huart5, "k");
			pin2++;
		}else if ((HAL_GPIO_ReadPin(carro2_GPIO_Port, carro2_Pin) == 0) && pin2 == 1) {
			HAL_Delay(15);
			quitarcarro2();
			parqueos_disponibles();
			send_uart_command(&huart5, "l");
			pin2 = 0;
		}

		if ((HAL_GPIO_ReadPin(carro3_GPIO_Port, carro3_Pin) == 0 && pin3 == 0)) {
			HAL_Delay(15);
			movercarro3();
			parqueos_disponibles();
			send_uart_command(&huart5, "m");
			pin3++;
		}else if ((HAL_GPIO_ReadPin(carro3_GPIO_Port, carro3_Pin) == 0) && pin3 == 1) {
			HAL_Delay(15);
			quitarcarro3();
			parqueos_disponibles();
			send_uart_command(&huart5, "n");
			pin3 = 0;
		}

		if ((HAL_GPIO_ReadPin(carro4_GPIO_Port, carro4_Pin) == 0 && pin4 == 0)) {
			HAL_Delay(15);
			movercarro4();
			parqueos_disponibles();
			send_uart_command(&huart5, "o");
			pin4++;
		}else if ((HAL_GPIO_ReadPin(carro4_GPIO_Port, carro4_Pin) == 0) && pin4 == 1) {
			HAL_Delay(15);
			quitarcarro4();
			parqueos_disponibles();
			send_uart_command(&huart5, "p");
			pin4 = 0;
		}

		if (c1_1 == 1 && c1_2 == 0 && pin5 == 0) {
			movercarro5();
			parqueos_disponibles();
			pin5 = 1;
			c1_1 = 0;  // Restablecer para permitir el movimiento nuevamente
		} else if (c1_1 == 0 && c1_2 == 1 && pin5 == 1) {
			quitarcarro5();
			parqueos_disponibles();
			pin5 = 0;
			c1_2 = 0;  // Restablecer para permitir la eliminación nuevamente
		}

		// Lógica para el carro 6
		if (c2_1 == 1 && c2_2 == 0 && pin6 == 0) {
			movercarro6();
			parqueos_disponibles();
			pin6 = 1;
			c2_1 = 0;  // Restablecer para permitir el movimiento nuevamente
		} else if (c2_1 == 0 && c2_2 == 1 && pin6 == 1) {
			quitarcarro6();
			parqueos_disponibles();
			pin6 = 0;
			c2_2 = 0;  // Restablecer para permitir la eliminación nuevamente
		}

		// Lógica para el carro 7
		if (c3_1 == 1 && c3_2 == 0 && pin7 == 0) {
			movercarro7();
			parqueos_disponibles();
			pin7 = 1;
			c3_1 = 0;  // Restablecer para permitir el movimiento nuevamente
		} else if (c3_1 == 0 && c3_2 == 1 && pin7 == 1) {
			quitarcarro7();
			parqueos_disponibles();
			pin7 = 0;
			c3_2 = 0;  // Restablecer para permitir la eliminación nuevamente
		}

		// Lógica para el carro 8
		if (c4_1 == 1 && c4_2 == 0 && pin8 == 0) {
			movercarro8();
			parqueos_disponibles();
			pin8 = 1;
			c4_1 = 0;  // Restablecer para permitir el movimiento nuevamente
		} else if (c4_1 == 0 && c4_2 == 1 && pin8 == 0) {
			quitarcarro8();
			parqueos_disponibles();
			pin8 = 1;
			c4_2 = 0;  // Restablecer para permitir la eliminación nuevamente
		}




		HAL_Delay(10);

	}


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_D6_Pin|led_rojo_Pin|led_verde_Pin
                          |LCD_D3_Pin|LCD_D5_Pin|LCD_D4_Pin|SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RST_Pin LCD_D1_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : carro1_Pin carro2_Pin carro4_Pin carro3_Pin */
  GPIO_InitStruct.Pin = carro1_Pin|carro2_Pin|carro4_Pin|carro3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin LCD_D7_Pin
                           LCD_D0_Pin LCD_D2_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LCD_D6_Pin LCD_D3_Pin LCD_D5_Pin
                           LCD_D4_Pin SD_SS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin|SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : led_rojo_Pin led_verde_Pin */
  GPIO_InitStruct.Pin = led_rojo_Pin|led_verde_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	// Verifica la interrupcion de UART5
	if(huart->Instance == UART5){

		if(rx_data[0] == 'A'){	// Recibe comando 'U'
			char* data = "A\n";
			c1_1 = 1;
			c1_2 = 0;
		}else if(rx_data[0] == 'B'){	// Recibe comando 'U'
			char* data = "B\n";
			c1_1 = 0;
			c1_2 = 1;
		}
		if(rx_data[0] == 'C'){	// Recibe comando 'U'
			char* data = "C\n";
			c2_1 = 1;
			c2_2 = 0;
		}else if(rx_data[0] == 'D'){	// Recibe comando 'U'
			char* data = "D\n";
			c2_1 = 0;
			c2_2 = 1;
		}
		if(rx_data[0] == 'E'){	// Recibe comando 'U'
			char* data = "E\n";
			c3_1 = 1;
			c3_2 = 0;
		}else if(rx_data[0] == 'F'){	// Recibe comando 'U'
			char* data = "F\n";
			c3_1 = 0;
			c3_2 = 1;
		}
		if(rx_data[0] == 'G'){	// Recibe comando 'U'
			char* data = "G\n";
			c4_1 = 1;
			c4_2 = 0;
		}else if(rx_data[0] == 'H'){	// Recibe comando 'U'
			char* data = "H\n";
			c4_1 = 0;
			c4_2 = 1;
		}
	}

	// Vuelve habilitar la recepcion de datos por UART2
	HAL_UART_Receive_IT(&huart5, rx_data, 1);
}

/****************************************Envío hacia la otra núcleo**********************************************/
void send_uart_command(UART_HandleTypeDef *huart, char* command) {
    HAL_UART_Transmit(huart, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
}

/****************************************Interrupcion para NeoPixel**********************************************/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)	{
	HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
	htim3.Instance->CCR1 = 0;
}

/****************************************Callback de Transmisión de I2C**********************************************/
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {
    	if (i2c_tx_command_flag1 == 1) {
			HAL_I2C_Slave_Transmit(&hi2c1, &i2c_command1, 1, HAL_MAX_DELAY);
			i2c_tx_command_flag1 = 0; // Reiniciar el flag después de enviar
		} else if (i2c_tx_command_flag2 == 1) {
			HAL_I2C_Slave_Transmit(&hi2c1, &i2c_command2, 1, HAL_MAX_DELAY);
			i2c_tx_command_flag2 = 0;
		} else if (i2c_tx_command_flag3 == 1) {
			HAL_I2C_Slave_Transmit(&hi2c1, &i2c_command3, 1, HAL_MAX_DELAY);
			i2c_tx_command_flag3 = 0;
		} else if (i2c_tx_command_flag4 == 1) {
			HAL_I2C_Slave_Transmit(&hi2c1, &i2c_command4, 1, HAL_MAX_DELAY);
			i2c_tx_command_flag4 = 0;
		}


    }
}

/****************************************Callback de Recepción de I2C**********************************************/
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {

    }
}

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
	while (1) {
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
