/**
 * @file bh1750.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-10-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "main.h"
#include "bh1750.h"

I2C_HandleTypeDef 	*BH1750_I2C;	/* Handler to I2C interface. */
uint8_t 			BH1750_MODE;	/* Current sensor mode. */ 
uint8_t 			Bh1750_Mtreg;	/* Current MT register value */ 

BH1750_STATUS BH1750_Init(I2C_HandleTypeDef *hi2c)
{
	BH1750_STATUS retStatus;
	BH1750_I2C = hi2c;
	if(BH1750_OK != BH1750_Reset())
	{
		retStatus = BH1750_ERROR;
	}
	else
	{
        if(BH1750_OK == BH1750_SetMtreg(BH1750_DEFAULT_MTREG)) // Set default value;
		{
			retStatus = BH1750_OK;
		}

	}
	return retStatus;
}

BH1750_STATUS BH1750_Reset(void)
{
	BH1750_STATUS retStatus;
	uint8_t tmp = BH1750_RESET; /* Reset Data Register value */
	if(HAL_OK == HAL_I2C_Master_Transmit(BH1750_I2C, BH1750_ADDRESS, &tmp, 1, 10))
	{
		retStatus = BH1750_OK;
	}
	else
	{
		retStatus = BH1750_ERROR;
	}
	return retStatus;
}

BH1750_STATUS BH1750_PowerState(uint8_t PowerOn)
{
	BH1750_STATUS retStatus;
	PowerOn = (PowerOn? BH1750_POWER_ON:BH1750_POWER_DOWN);
	if(HAL_OK == HAL_I2C_Master_Transmit(BH1750_I2C, BH1750_ADDRESS, &PowerOn, 1, 10))
	{
		retStatus = BH1750_OK;
	}
	else
	{
		retStatus = BH1750_ERROR;
	}
	return retStatus; 
}

BH1750_STATUS BH1750_SetMode(uint8_t Mode)
{
	BH1750_STATUS retStatus;
	if(!((Mode >> 4) || (Mode >> 5))) 
	{
		retStatus = BH1750_ERROR;
	}
	if((Mode & 0x0F) > 3)
	{
		retStatus = BH1750_ERROR;
	}
	BH1750_MODE = Mode;
	if(HAL_OK == HAL_I2C_Master_Transmit(BH1750_I2C, BH1750_ADDRESS, &Mode, 1, 10))
	{
		retStatus = BH1750_OK;
	}
	else
	{
		retStatus = BH1750_ERROR;
	}
	return retStatus;
}

BH1750_STATUS BH1750_SetMtreg(uint8_t Mtreg)
{
	BH1750_STATUS retStatus;
	HAL_StatusTypeDef retCode;
	uint8_t tmp[2];
	if (Mtreg < 31 || Mtreg > 254)
	{
		retStatus = BH1750_ERROR;
	}
	Bh1750_Mtreg = Mtreg;
	tmp[0] = (0x40 | (Mtreg >> 5));
	tmp[1] = (0x60 | (Mtreg & 0x1F));
	retCode = HAL_I2C_Master_Transmit(BH1750_I2C, BH1750_ADDRESS, &tmp[0], 1, 10);
	if (HAL_OK != retCode) 
	{
		retStatus = BH1750_ERROR;
	}
	retCode = HAL_I2C_Master_Transmit(BH1750_I2C, BH1750_ADDRESS, &tmp[1], 1, 10);
	if (HAL_OK == retCode)
	{
		retStatus = BH1750_OK;
	}
	else
	{
		retStatus = BH1750_ERROR;
	}
	return retStatus;
}

BH1750_STATUS BH1750_TriggerManualConversion(void)
{
	BH1750_STATUS retStatus;
	if(BH1750_OK == BH1750_SetMode(BH1750_MODE))
	{
		retStatus = BH1750_OK;
	}
	else
	{
		retStatus = BH1750_ERROR;
	}
	return retStatus;
}

BH1750_STATUS BH1750_ReadLight(float *Result)
{
	BH1750_STATUS retStatus;
	float result;
	uint8_t tmp[2];
	if(HAL_OK == HAL_I2C_Master_Receive(BH1750_I2C, BH1750_ADDRESS, tmp, 2, 10))
	{
		result = (tmp[0] << 8) | (tmp[1]);

		if(Bh1750_Mtreg != BH1750_DEFAULT_MTREG)
		{
			result *= (float)((uint8_t)BH1750_DEFAULT_MTREG/(float)Bh1750_Mtreg);
		}

		if(BH1750_MODE == ONETIME_HIGH_RES_MODE_2 || BH1750_MODE == CONTINUOUS_HIGH_RES_MODE_2)
		{
			result /= 2.0;
		}

		*Result = result / (float)BH1750_CONVERSION_FACTOR;
		retStatus = BH1750_OK;
	}
	else
	{
		retStatus = BH1750_ERROR;
	}
	return retStatus;
}