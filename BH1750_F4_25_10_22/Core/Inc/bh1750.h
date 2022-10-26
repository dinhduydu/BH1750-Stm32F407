/**
 * @file bh1750.h
 * @author your name (you@domain.com)
 * @brief according BH1750 datasheet
 * @version 0.1
 * @date 2022-10-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef BH1750_H_
#define BH1750_H_
#include <stdint.h>

/*****************************************************************************
 * Macros 
*****************************************************************************/
#define BH1750_ADDRESS			    (0x23<<1)

#define	BH1750_POWER_DOWN		    0x00
#define	BH1750_POWER_ON			    0x01
#define	BH1750_RESET			    0x07
#define	BH1750_DEFAULT_MTREG	    69
#define BH1750_MIN_MTREG            31
#define BH1750_MAX_MTREG            254

#define CONTINUOUS_HIGH_RES_MODE    0x10
#define CONTINUOUS_HIGH_RES_MODE_2  0x11
#define CONTINUOUS_LOW_RES_MODE     0x13
#define ONETIME_HIGH_RES_MODE       0x20
#define ONETIME_HIGH_RES_MODE_2     0x21
#define ONETIME_LOW_RES_MODE        0x23


#define BH1750_CONVERSION_FACTOR	1.2

/********************************************************************************
 * Enum
********************************************************************************/
typedef enum 
{
	BH1750_OK		= 0,
	BH1750_ERROR	= 1
} BH1750_STATUS;

/**********************************************************************************
 * API
***********************************************************************************/

/**
 * @brief Initialization.
 * 
 * @param hi2c 
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_Init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Reset all registers to default value.
 * 
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_Reset(void);

/**
 * @brief Set the power state.
 * 
 * @param PowerOn   0 - sleep, low power.
            	    1 - running.
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_PowerState(uint8_t PowerOn);

/**
 * @brief Set the mode of converting. Look into bh1750_mode enum.
 * 
 * @param Mode 
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_SetMode(uint8_t Mode);

/**
 * @brief Set the Measurement Time register. It allows to increase or decrease the sensitivity.
 * 
 * @param Mtreg 
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_SetMtreg(uint8_t Mtreg);

/**
 * @brief Trigger the conversion in manual modes.
 *	For low resolution conversion time is typical 16 ms,
 *	for high resolution 120 ms. You need to wait until read the measurement value.
 *	There is no need to exit low power mode for manual conversion. It makes automatically.
 * 
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_TriggerManualConversion(void);

/**
 * @brief Read the converted value and calculate the result.
 * 
 * @param Result 
 * @return BH1750_STATUS 
 */
BH1750_STATUS BH1750_ReadLight(float *Result);

#endif /* BH1750_H_ */
