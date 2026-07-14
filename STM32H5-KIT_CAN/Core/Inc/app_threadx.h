/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.h
  * @author  MCD Application Team
  * @brief   ThreadX applicative header file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_THREADX_H
#define __APP_THREADX_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tx_api.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Private defines -----------------------------------------------------------*/
#define TX_APP_STACK_SIZE                       512
#define TX_APP_THREAD_PRIO                      10

/* USER CODE BEGIN PD */
#define TX_CANTX_STACK_SIZE						1024
#define TX_CANTX_THREAD_PRIO					10

#define TX_CANRX_STACK_SIZE						1024
#define TX_CANRX_THREAD_PRIO					10

/////
///
#define CAN_BR_IDX_100K			1
#define CAN_BR_IDX_125K			2
#define CAN_BR_IDX_250K			3
#define CAN_BR_IDX_500K			4
#define CAN_BR_IDX_1M			5

#define CAN_IDE_STD				1
#define CAN_IDE_EXT				2

#define CAN_MAX_DATA_SIZE   	64

#define CAN_BR_NUM			  	6
#define CAN_DLC				    8


/* USER CODE END PD */

/* Main thread defines -------------------------------------------------------*/
#ifndef TX_APP_THREAD_PREEMPTION_THRESHOLD
#define TX_APP_THREAD_PREEMPTION_THRESHOLD      TX_APP_THREAD_PRIO
#endif

#ifndef TX_APP_THREAD_TIME_SLICE
#define TX_APP_THREAD_TIME_SLICE                TX_NO_TIME_SLICE
#endif

#ifndef TX_APP_THREAD_AUTO_START
#define TX_APP_THREAD_AUTO_START                TX_AUTO_START
#endif
/* USER CODE BEGIN MTD */

/* USER CODE END MTD */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
UINT App_ThreadX_Init(VOID *memory_ptr);
void MX_ThreadX_Init(void);
void tx_app_thread_entry(ULONG thread_input);

/* USER CODE BEGIN EFP */
void tx_cantx_thread_entry(ULONG thread_input);
void tx_canrx_thread_entry(ULONG thread_input);

/* USER CODE END EFP */

/* USER CODE BEGIN 1 */
typedef __PACKED_STRUCT
{
	uint32_t		br;
	uint32_t		prescaler;
	uint32_t		sjw;
	uint32_t		bs1;
	uint32_t		bs2;
} _CAN_BR_STRUCT;

extern _CAN_BR_STRUCT CAN_BR_Param[];

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_THREADX_H */
