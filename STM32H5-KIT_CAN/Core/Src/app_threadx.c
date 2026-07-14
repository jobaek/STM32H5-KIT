/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
  *
  * CAN1(TX) : CAN2(RX)
  **/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CAN_TX_ID				0x123

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define COUNTOF(BUFFER)			(sizeof((BUFFER)) / sizeof(*(BUFFER)))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD tx_app_thread;
/* USER CODE BEGIN PV */
TX_THREAD tx_cantx_thread;
TX_THREAD tx_canrx_thread;

TX_SEMAPHORE tx_cantx_semaphore;
TX_SEMAPHORE tx_canrx_semaphore;

FDCAN_RxHeaderTypeDef rxHeader;
FDCAN_TxHeaderTypeDef txHeader;

// FDCAN : 80MHz
// BR, PreScaler, SJW=1, Seg1, Seg2
_CAN_BR_STRUCT CAN_BR_Param[CAN_BR_NUM] =
{
	{CAN_BR_IDX_250K, 20, 1, 13, 2},		// auto
	{CAN_BR_IDX_100K, 50, 1, 13, 2},		// 100k
	{CAN_BR_IDX_125K, 40, 1, 13, 2},		// 125k
	{CAN_BR_IDX_250K, 20, 1, 13, 2},		// 250k
	{CAN_BR_IDX_500K, 10, 1, 13, 2},		// 500k
	{CAN_BR_IDX_1M, 5, 1, 13, 2}			// 1000k
};

uint32_t rxID;
uint8_t RxData[8U];

uint8_t TxData[] = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x00};

uint32_t  CAN_BR_Table[CAN_BR_NUM] = {0, 100, 125, 250, 500, 1000};

FDCAN_TxHeaderTypeDef TxHeader;
FDCAN_RxHeaderTypeDef RxHeader;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static uint32_t BufferCmp8b(const uint8_t *pBuffer1, const uint8_t *pBuffer2, uint16_t BufferLength);

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  CHAR *pointer;

  /* Allocate the stack for tx app thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create tx app thread.  */
  if (tx_thread_create(&tx_app_thread, "tx app thread", tx_app_thread_entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */
  /* Allocate the stack for CAN Tx thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
		  TX_CANTX_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create CAN Tx thread.  */
  if (tx_thread_create(&tx_cantx_thread, "tx can tx thread", tx_cantx_thread_entry, 0, pointer,
		  TX_CANTX_STACK_SIZE, TX_CANTX_THREAD_PRIO, TX_CANTX_THREAD_PRIO,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for CAN Rx thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
		  TX_CANRX_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create CAN Rx thread.  */
  if (tx_thread_create(&tx_canrx_thread, "tx can rx thread", tx_canrx_thread_entry, 0, pointer,
		  TX_CANRX_STACK_SIZE, TX_CANRX_THREAD_PRIO, TX_CANRX_THREAD_PRIO,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  if (tx_semaphore_create(&tx_cantx_semaphore, "tx CAN_Tx semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }

  if (tx_semaphore_create(&tx_canrx_semaphore, "tx CAN_Rx semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }


  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the tx_app_thread_entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void tx_app_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN tx_app_thread_entry */

  /* USER CODE END tx_app_thread_entry */
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN Before_Kernel_Start */

  /* USER CODE END Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN Kernel_Start_Error */

  /* USER CODE END Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/**
  * @brief  Function implementing the tx_cantx_thread_entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void tx_cantx_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN tx_cantx_thread_entry */
	TxHeader.Identifier = 0x123;                         // CAN ID (예: Standard ID 0x123)
	TxHeader.IdType = FDCAN_STANDARD_ID;                 // 표준 ID 사용 (Extended ID의 경우 FDCAN_EXTENDED_ID)
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;             // 데이터 프레임 타입
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;             // 💡 Classic CAN 최대 크기인 8바이트 지정
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;              // 💡 핵심: Classic 모드이므로 BRS OFF
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;               // 💡 핵심: Classic CAN 포맷 지정
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;    // 이벤트 FIFO 저장 안 함
	TxHeader.MessageMarker = 0;

	while(1)
	{
		if (tx_semaphore_get(&tx_cantx_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
		{

			/// CAN Send Packet
			if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData) != HAL_OK)
			{
			    /* 전송 실패 시 에러 처리 */
			    Error_Handler();
			}
		}

		tx_thread_sleep(1);
	}
  /* USER CODE END tx_cantx_thread_entry */
}


/**
  * @brief  Function implementing the tx_canrx_thread_entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void tx_canrx_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN tx_canrx_thread_entry */
	while(1)
	{
		if (tx_semaphore_get(&tx_canrx_semaphore, TX_WAIT_FOREVER) == TX_SUCCESS)
		{
			HAL_GPIO_TogglePin(LED_STS_GPIO_Port, LED_STS_Pin);
			printf("= CAN Rx : %X - %X:%X:%X:%X:%X:%X:%X:%X \r\n", rxID, RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5], RxData[6], RxData[7]);
			/// CAN Send Packet
			TxData[7]++;
		}

		tx_thread_sleep(1);
	}
  /* USER CODE END tx_canrx_thread_entry */
}

/**
  * @brief  EXTI line falling detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);

  if (GPIO_Pin == BTN_Pin)
  {
	  tx_semaphore_put(&tx_cantx_semaphore);
  }

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Falling_Callback could be implemented in the user file
   */
}

/**
  * @brief  Rx FIFO 0 callback.
  * @param  hfdcan pointer to an FDCAN_HandleTypeDef structure that contains
  *         the configuration information for the specified FDCAN.
  * @param  RxFifo0ITs indicates which Rx FIFO 0 interrupts are signaled.
  *         This parameter can be any combination of @arg FDCAN_Rx_Fifo0_Interrupts.
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0U)
  {
	  if (hfdcan->Instance == FDCAN2)
	  {
		  if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
		  {
			  // CAN1 수신 데이터 처리 로직
			  rxID = RxHeader.Identifier;

			  tx_semaphore_put(&tx_canrx_semaphore);
		  }
	  }




//    /* Retrieve Rx messages from RX FIFO0 */
//    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxHeader, rxData) != HAL_OK)
//    {
//      Error_Handler();
//    }
//
//    /* Compare received RX message to expected data. Ignore if not matching. */
//    if ((rxHeader.Identifier == RX_ID) &&
//        (rxHeader.IdType     == FDCAN_STANDARD_ID) &&
//        (rxHeader.DataLength == FDCAN_DLC_BYTES_16) &&
//        (BufferCmp8b(txData, rxData, COUNTOF(rxData)) == 0U))
//    {
//      /* Turn LED1 on */
//      BSP_LED_On(LED1);
//    }
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1 buffer to be compared.
  * @param  pBuffer2 buffer to be compared.
  * @param  BufferLength: buffer's length.
  * @retval 0: pBuffer1 is identical to pBuffer2
  * @retval 1: pBuffer1 differs from pBuffer2
  */
static uint32_t BufferCmp8b(const uint8_t *pBuffer1, const uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1U;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0U;
}

/* USER CODE END 1 */
