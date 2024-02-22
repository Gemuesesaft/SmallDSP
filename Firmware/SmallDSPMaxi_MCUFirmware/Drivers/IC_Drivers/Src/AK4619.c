/**
 ******************************************************************************
 * @file    AK4619.c
 * @version 1.0
 * @author  Till Heuer
 * @brief   Driver for AKM AK4619 4 channel audio codec.
 * @date 	Feb 20, 2024
 ******************************************************************************
 */

/** @addtogroup IC_Drivers
 * @{
 */

/** @addtogroup Audio_Codec
 * @{
 */

#include <AK4619.h>

/*******************************Register Settings*******************************/

uint8_t AK4619_ActivateSPIComunication(ak4619_Device_t *device) {
	/* Holds data for SPI transmission */
	uint8_t spiData[4] = {0xDE, 0xAD, 0xDA, 0x7A};
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Checks for input errors. */
	if(device->mcuInterface != ak4619_SPI) {
		return HAL_ERROR;
	}
	/* Repeats spi call, in case of busy spi unit. */
	for(uint8_t attempt = 0; attempt <= AK4619_COM_MAX_ATTEMPTS; attempt++) {
		/* Reads current setting of register. */
		HAL_GPIO_WritePin(device->NCSPort, device->NCSPin, GPIO_PIN_RESET);
		status = HAL_SPI_Transmit(device->hspi, spiData, 4, AK4619_COM_TIMEOUT);
		HAL_GPIO_WritePin(device->NCSPort, device->NCSPin, GPIO_PIN_SET);
		if(status == HAL_OK) {
			/* Breaks out of loop when successful. */
			break;
		} else if(status == HAL_BUSY) {
			/* Delays next spi call if first attempt failed. */
			HAL_Delay(AK4619_SPI_ATTEMPT_DELAY);
		} else {
			/* Returns error when spi unit fails. */
			return HAL_ERROR;
		}
	}
	return status;
}

uint8_t AK4619_WriteSPI(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data) {
	/* Holds data for SPI transmission */
	uint8_t spiData[4] = {0xC3, 0x00, registerAddress, *data};
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Checks for input errors. */
	if(device->mcuInterface != ak4619_SPI || registerAddress > AK4619_REG_DAC_MUTE_AND_FILTER_SETTING) {
		return HAL_ERROR;
	}
	/* Repeats spi call, in case of busy spi unit. */
	for(uint8_t attempt = 0; attempt <= AK4619_COM_MAX_ATTEMPTS; attempt++) {
		/* Reads current setting of register. */
		HAL_GPIO_WritePin(device->NCSPort, device->NCSPin, GPIO_PIN_RESET);
		status = HAL_SPI_Transmit(device->hspi, spiData, 4, AK4619_COM_TIMEOUT);
		HAL_GPIO_WritePin(device->NCSPort, device->NCSPin, GPIO_PIN_SET);
		if(status == HAL_OK) {
			/* Breaks out of loop when successful. */
			break;
		} else if(status == HAL_BUSY) {
			/* Delays next spi call if first attempt failed. */
			HAL_Delay(AK4619_SPI_ATTEMPT_DELAY);
		} else {
			/* Returns error when spi unit fails. */
			return HAL_ERROR;
		}
	}
	return status;
}

uint8_t AK4619_ReadSPI(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data) {
	/* Holds data for SPI transmission */
	uint8_t spiData[3] = {0x43, 0x00, registerAddress};
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Checks for input errors. */
	if(device->mcuInterface != ak4619_SPI || registerAddress > AK4619_REG_DAC_MUTE_AND_FILTER_SETTING) {
		return HAL_ERROR;
	}
	/* Repeats spi call, in case of busy spi unit. */
	for(uint8_t attempt = 0; attempt <= AK4619_COM_MAX_ATTEMPTS; attempt++) {
		/* Reads current setting of register. */
		HAL_GPIO_WritePin(device->NCSPort, device->NCSPin, GPIO_PIN_RESET);
		status = HAL_SPI_TransmitReceive(device->hspi, spiData, data, 4, AK4619_COM_TIMEOUT);
		HAL_GPIO_WritePin(device->NCSPort, device->NCSPin, GPIO_PIN_SET);
		if(status == HAL_OK) {
			/* Breaks out of loop when successful. */
			break;
		} else if(status == HAL_BUSY) {
			/* Delays next spi call if first attempt failed. */
			HAL_Delay(AK4619_SPI_ATTEMPT_DELAY);
		} else {
			/* Returns error when spi unit fails. */
			return HAL_ERROR;
		}
	}
	return status;
}

uint8_t AK4619_WriteI2C(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data) {
	/* Holds i2c status for error catching. */
	uint8_t status = 0;
	/* Checks for input errors. */
	if(device->mcuInterface != ak4619_I2C || registerAddress > AK4619_REG_DAC_MUTE_AND_FILTER_SETTING) {
		return HAL_ERROR;
	}
	/* Repeats i2c call, in case of busy i2c unit. */
	for(uint8_t attempt = 0; attempt <= AK4619_COM_MAX_ATTEMPTS; attempt++) {
		status = HAL_I2C_Mem_Write(
				device->hi2c, (AK4619_I2C_DEVICE_ADDRESS + device->a0) << 1,
				registerAddress, I2C_MEMADD_SIZE_8BIT, data, 1,
				AK4619_COM_TIMEOUT
		);
		if(status == HAL_OK) { // Breaks out of loop when successful.
			break;
		} else if(status == HAL_BUSY) {
			/* Delays next i2c call if first attempt failed. */
			HAL_Delay(AK4619_I2C_ATTEMPT_DELAY);
		} else {
			/* Returns error when i2c unit fails. */
			return HAL_ERROR;
		}
	}
	return status;
}

uint8_t AK4619_ReadI2C(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data) {
	/* Holds i2c status for error catching. */
	uint8_t status = 0;
	/* Checks for input errors. */
	if(device->mcuInterface != ak4619_I2C || registerAddress > AK4619_REG_DAC_MUTE_AND_FILTER_SETTING) {
		return HAL_ERROR;
	}
	/* Repeats i2c call, in case of busy i2c unit. */
	for(uint8_t attempt = 0; attempt <= AK4619_COM_MAX_ATTEMPTS; attempt++) {
		status = HAL_I2C_Mem_Read(device->hi2c, (AK4619_I2C_DEVICE_ADDRESS + device->a0) << 1, registerAddress, I2C_MEMADD_SIZE_8BIT, data, 1, AK4619_COM_TIMEOUT);
		if(status == HAL_OK) { // Breaks out of loop when successful.
			break;
		} else if(status == HAL_BUSY) {
			/* Delays next i2c call if first attempt failed. */
			HAL_Delay(AK4619_I2C_ATTEMPT_DELAY);
		} else {
			/* Returns error when i2c unit fails. */
			return HAL_ERROR;
		}
	}
	return status;
}

uint8_t AK4619_Init(ak4619_Device_t *device) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ActivateSPIComunication(device); //TODO: Error catching for pin, com handler and port
	} else if(device->mcuInterface != ak4619_I2C) {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_PowerManagementReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_POWER_MANAGEMENT, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_POWER_MANAGEMENT, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_PowerManagementReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_POWER_MANAGEMENT, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_POWER_MANAGEMENT, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_AudioInterfaceFormatReg(ak4619_Device_t *device, uint8_t registerValue[2]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_B, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_B, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_AudioInterfaceFormatReg(ak4619_Device_t *device, uint8_t registerValue[2]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_B, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_AUDIO_INTERFACE_FORMAT_B, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_SystemClockReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_SYSTEM_CLOCK_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_SYSTEM_CLOCK_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_SystemClockReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_SYSTEM_CLOCK_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_SYSTEM_CLOCK_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_MicAmpReg(ak4619_Device_t *device, uint8_t registerValue[2]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_MIC_AMP_GAIN_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_MIC_AMP_GAIN_B, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_MIC_AMP_GAIN_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_MIC_AMP_GAIN_B, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_MicAmpReg(ak4619_Device_t *device, uint8_t registerValue[2]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_MIC_AMP_GAIN_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_MIC_AMP_GAIN_B, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_MIC_AMP_GAIN_A, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_MIC_AMP_GAIN_B, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_ADCDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_ADC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_ADC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_ADC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_ADC2_R_DIGITAL_VOLUME, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_ADC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_ADC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_ADC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_ADC2_R_DIGITAL_VOLUME, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_ADCDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]){
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_ADC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_ADC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_ADC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_ADC2_R_DIGITAL_VOLUME, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_ADC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_ADC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_ADC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_ADC2_R_DIGITAL_VOLUME, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_ADCDigitalFilterReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_ADC_DIGITAL_FILTER_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_ADC_DIGITAL_FILTER_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_ADCDigitalFilterReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_ADC_DIGITAL_FILTER_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_ADC_DIGITAL_FILTER_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_ADCAnalogInputReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_ADC_ANALOG_INPUT_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_ADC_ANALOG_INPUT_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_ADCAnalogInputReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_ADC_ANALOG_INPUT_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_ADC_ANALOG_INPUT_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_ADCMuteAndHPFControlReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_ADC_MUTE_AND_HPF_CONTROL, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_ADC_MUTE_AND_HPF_CONTROL, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_ADCMuteAndHPFControlReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_ADC_MUTE_AND_HPF_CONTROL, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_ADC_MUTE_AND_HPF_CONTROL, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_DACDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_DAC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_DAC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_DAC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteSPI(device, AK4619_REG_DAC2_R_DIGITAL_VOLUME, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_DAC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_DAC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_DAC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_WriteI2C(device, AK4619_REG_DAC2_R_DIGITAL_VOLUME, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_DACDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_DAC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_DAC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_DAC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadSPI(device, AK4619_REG_DAC2_R_DIGITAL_VOLUME, ++registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_DAC1_L_DIGITAL_VOLUME, registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_DAC1_R_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_DAC2_L_DIGITAL_VOLUME, ++registerValue);
		if(status) return EXIT_FAILURE;
		status = AK4619_ReadI2C(device, AK4619_REG_DAC2_R_DIGITAL_VOLUME, ++registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_DACInputSettingsReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_DAC_INPUT_SELECT_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_DAC_INPUT_SELECT_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_DACInputSettingsReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_DAC_INPUT_SELECT_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_DAC_INPUT_SELECT_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_DACDeemphasisReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_DAC_DEEMPHASIS_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_DAC_DEEMPHASIS_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_DACDeemphasisReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_DAC_DEEMPHASIS_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_DAC_DEEMPHASIS_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_SetRegister_DACMuteAndFilterSettingsReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_WriteSPI(device, AK4619_REG_DAC_MUTE_AND_FILTER_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_WriteI2C(device, AK4619_REG_DAC_MUTE_AND_FILTER_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

uint8_t AK4619_GetRegister_DACMuteAndFilterSettingsReg(ak4619_Device_t *device, uint8_t *registerValue) {
	uint8_t status = 0;
	if(device->mcuInterface == ak4619_SPI) {
		status = AK4619_ReadSPI(device, AK4619_REG_DAC_MUTE_AND_FILTER_SETTING, registerValue);
	} else if(device->mcuInterface == ak4619_I2C) {
		status = AK4619_ReadI2C(device, AK4619_REG_DAC_MUTE_AND_FILTER_SETTING, registerValue);
	} else {
		return EXIT_FAILURE;
	}
	return status;
}

/******************************Individual Settings******************************/

uint8_t AK4619_SetPowerSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_PowerMode_t powerMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_DAC2 || powerMode > ak4619_NormalOperation) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	if(converter <= ak4619_ADC2) {
		registerValue = (registerValue & (~(0b1 << (4 + converter)))) | (powerMode << (4 + converter));
	} else {
		registerValue = (registerValue & (~(0b1 << (converter - 1)))) | (powerMode << (converter - 1));
	}
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetPowerSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_PowerMode_t *powerMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	if(converter <= ak4619_ADC2) {
		*powerMode = (registerValue & (0b1 << (4 + converter))) >> (4 + converter);
	} else {
		*powerMode = (registerValue & (0b1 << (converter - 1))) >> (converter - 1);
	}
	return status;
}

uint8_t AK4619_SetResetMode(ak4619_Device_t *device, ak4619_ResetMode_t resetMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(resetMode > ak4619_Enabled) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b1)) | resetMode;
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetResetMode(ak4619_Device_t *device, ak4619_ResetMode_t *resetMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*resetMode = registerValue & 0b1;
	return status;
}

uint8_t AK4619_SetTDMMode(ak4619_Device_t *device, ak4619_TDMMode_t tdmMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(tdmMode > ak4619_TDMMode) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[0] = (registerValue[0] & ~(0b1 << 7)) | (tdmMode << 7);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetTDMMode(ak4619_Device_t *device, ak4619_TDMMode_t *tdmMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*tdmMode = (registerValue[0] & (0b1 << 7)) >> 7;
	return status;
}

uint8_t AK4619_SetAudioInterfaceFormat(ak4619_Device_t *device, ak4619_AudioInterfaceFormat_t ifFormat) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(ifFormat > ak4619_TDM256_MSBJustifiedMode) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[0] = (registerValue[0] & ~(0b111 << 4)) | (ifFormat << 4);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetAudioInterfaceFormat(ak4619_Device_t *device, ak4619_AudioInterfaceFormat_t *ifFormat) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*ifFormat = (registerValue[0] & (0b111 << 4)) >> 4;
	return status;
}

uint8_t AK4619_SetSlotLength(ak4619_Device_t *device, ak4619_WordLength_t slotLength) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(slotLength > ak4619_32Bit) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[0] = (registerValue[0] & ~(0b11 << 2)) | (slotLength << 2);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetSlotLength(ak4619_Device_t *device, ak4619_WordLength_t *slotLength) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*slotLength = (registerValue[0] & (0b11 << 2)) >> 2;
	return status;
}

uint8_t AK4619_SetBCLKEdge(ak4619_Device_t *device, ak4619_BCLKEdge_t bclkEdge) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(bclkEdge > ak4619_BCLK_RisingEdge) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[0] = (registerValue[0] & ~(0b1 << 1)) | (bclkEdge << 1);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetBCLKEdge(ak4619_Device_t *device, ak4619_BCLKEdge_t *bclkEdge) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*bclkEdge = (registerValue[0] & (0b1 << 1)) >> 1;
	return status;
}

uint8_t AK4619_SetFastModeSetting(ak4619_Device_t *device, ak4619_SDOUTFastMode_t fastMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(fastMode > ak4619_SDOUT_FastMode) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[0] = (registerValue[0] & ~0b1) | fastMode;
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetFastModeSetting(ak4619_Device_t *device, ak4619_SDOUTFastMode_t *fastMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*fastMode = registerValue[0] & 0b1;
	return status;
}

uint8_t AK4619_SetSlotStart(ak4619_Device_t *device, ak4619_SlotStartPosition_t slotStart) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(slotStart > ak4619_SDOUT_SlotLengthBasisSlotStart) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[1] = (registerValue[1] & ~(0b1 << 4)) | (slotStart << 4);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetSlotStart(ak4619_Device_t *device, ak4619_SlotStartPosition_t *slotStart) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*slotStart = (registerValue[1] & (0b1 << 4)) >> 4;
	return status;
}

uint8_t AK4619_SetADCWordLength(ak4619_Device_t *device, ak4619_WordLength_t wordLength) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(wordLength > ak4619_32Bit) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[1] = (registerValue[1] & ~(0b11 << 2)) | (wordLength << 2);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetADCWordLength(ak4619_Device_t *device, ak4619_WordLength_t *wordLength) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*wordLength = (registerValue[1] & (0b11 << 2)) >> 2;
	return status;
}

uint8_t AK4619_SetDACWordLength(ak4619_Device_t *device, ak4619_WordLength_t wordLength) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(wordLength > ak4619_16Bit) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[1] = (registerValue[1] & ~0b11) | wordLength;
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetDACWordLength(ak4619_Device_t *device, ak4619_WordLength_t *wordLength) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*wordLength = registerValue[1] & 0b11;
	return status;
}

uint8_t AK4619_SetSystemClockSetting(ak4619_Device_t *device, ak4619_SystemClockSetting_t sysClock) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(sysClock > ak4619_128Fs_192kHz) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_AudioInterfaceFormatReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[1] = (registerValue[1] & ~0b111) | sysClock;
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_PowerManagementReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetSystemClockSetting(ak4619_Device_t *device, ak4619_SystemClockSetting_t *sysClock) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Reads current setting of register. */
	status = AK4619_GetRegister_PowerManagementReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*sysClock = registerValue[1] & 0b111;
	return status;
}

uint8_t AK4619_SetADCVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t tranTime) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(tranTime > ak4619_16overFs_TransitionTime) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b1 << 7)) | (tranTime << 7);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetADCVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t *tranTime) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Reads current setting of register. */
	status = AK4619_GetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*tranTime = (registerValue & (0b1 << 7)) >> 7;
	return status;
}

uint8_t AK4619_SetDACVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t tranTime) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(tranTime > ak4619_16overFs_TransitionTime) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b1 << 7)) | (tranTime << 7);
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetDACVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t *tranTime) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Reads current setting of register. */
	status = AK4619_GetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*tranTime = (registerValue & (0b1 << 7)) >> 7;
	return status;
}

uint8_t AK4619_SetSoftMuteSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_SoftMuteSetting_t softMute) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_DAC2 || softMute > ak4619_SoftMuteEnabled) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	if(converter <= ak4619_ADC2) {
		status = AK4619_GetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	} else {
		status = AK4619_GetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	}
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b1 << ((converter % 2) + 4))) | (softMute << ((converter % 2) + 4));
	/* Sends changed register settings to chip. */
	if(converter <= ak4619_ADC2) {
		status = AK4619_SetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	} else {
		status = AK4619_SetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	}
	return status;
}

uint8_t AK4619_GetSoftMuteSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_SoftMuteSetting_t *softMute) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	if(converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	if(converter <= ak4619_ADC2) {
		status = AK4619_GetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	} else {
		status = AK4619_GetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	}
	/* Combines current value of register with value that has to be changed. */
	*softMute = (registerValue & (0b1 << ((converter % 2) + 4)) >> ((converter % 2) + 4));
	return status;
}

uint8_t AK4619_SetADCDCBlockingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DCBlockingFilterMode_t hpf) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_ADC2 || hpf > ak4619_DCBlockingFilterDisabled) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b1 << (converter + 1))) | (hpf << (converter + 1));
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetADCDCBlockingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DCBlockingFilterMode_t *hpf) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_ADC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_ADCMuteAndHPFControlReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*hpf = (registerValue & (0b1 << (converter + 1))) >> (converter + 1);
	return status;
}

uint8_t AK4619_SetDACInputSource(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DACSourceSetting_t source) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter < ak4619_DAC1 || converter > ak4619_DAC2 || source > ak4619_SDOUT2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_DACInputSettingsReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b11 << (converter * 2))) | (source << (converter * 2));
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_DACInputSettingsReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetDACInputSource(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DACSourceSetting_t *source) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter < ak4619_DAC1 || converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_DACInputSettingsReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*source = (registerValue & (0b11 << (converter * 2))) >> (converter * 2);
	return status;
}

uint8_t AK4619_SetDACDeemphasisFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DeemphasisMode_t deemp) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter < ak4619_DAC1 || converter > ak4619_DAC2 || deemp > ak4619_Deemphasis_32kHz) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_DACDeemphasisReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b11 << (converter * 2))) | (deemp << (converter * 2));
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_DACDeemphasisReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetDACDeemphasisFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DeemphasisMode_t *deemp) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter < ak4619_DAC1 || converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_DACDeemphasisReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*deemp = (registerValue & (0b11 << (converter * 2))) >> (converter * 2);
	return status;
}

uint8_t AK4619_SetMicAmpGain(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_MicGainSetting_t micGain) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(adc > ak4619_ADC2R || micGain > ak4619_27dB_MicGain) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_MicAmpReg(device, registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[adc/2] = (registerValue[adc/2] & ~(0b1111 << ((adc % 2) * 4))) | (micGain << ((adc % 2) * 4));
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_MicAmpReg(device, registerValue);
	return status;
}

uint8_t AK4619_GetMicAmpGain(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_MicGainSetting_t *micGain) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[2] = {0};
	/* Checks for input errors. */
	if(adc > ak4619_ADC2R) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_MicAmpReg(device, registerValue);
	/* Combines current value of register with value that has to be changed. */
	*micGain = (registerValue[adc/2] & (0b1111 << ((adc % 2) * 4))) >> ((adc % 2) * 4);
	return status;
}

uint8_t AK4619_SetDigitalVolume(ak4619_Device_t *device, ak4619_Converter_t converter, uint8_t volume[2]) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[4] = {0};
	/* Checks for input errors. */
	if(converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	if(converter <= ak4619_ADC2) {
		status = AK4619_GetRegister_ADCDigitalVolumeReg(device, registerValue);
	} else {
		status = AK4619_GetRegister_DACDigitalVolumeReg(device, registerValue);
	}
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue[(converter % 2) * 2] = volume[0];
	registerValue[((converter % 2) * 2) + 1]  = volume[1];
	/* Sends changed register settings to chip. */
	if(converter <= ak4619_ADC2) {
		status = AK4619_SetRegister_ADCDigitalVolumeReg(device, registerValue);
	} else {
		status = AK4619_SetRegister_DACDigitalVolumeReg(device, registerValue);
	}
	return status;
}

uint8_t AK4619_GetDigitalVolume(ak4619_Device_t *device, ak4619_Converter_t converter, uint8_t volume[2]) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue[4] = {0};
	if(converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	if(converter <= ak4619_ADC2) {
		status = AK4619_GetRegister_ADCDigitalVolumeReg(device, registerValue);
	} else {
		status = AK4619_GetRegister_DACDigitalVolumeReg(device, registerValue);
	}
	/* Combines current value of register with value that has to be changed. */
	volume[0] = registerValue[(converter % 2) * 2];
	volume[1] = registerValue[((converter % 2) * 2) + 1];
	return status;
}

uint8_t AK4619_SetAntiAliasingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DigitalFilter_t aaFilter) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_DAC2 || aaFilter > ak4619_VoiceFilter || (converter >= ak4619_DAC1 && aaFilter > ak4619_ShortDelaySlowRollOffFilter)) {
		return EXIT_FAILURE;
	}
	if(converter <= ak4619_ADC2) {
		/* Reads current setting of register. */
		status = AK4619_GetRegister_ADCDigitalFilterReg(device, &registerValue);
		if(status) return EXIT_FAILURE;
		/* Combines current value of register with value that has to be changed. */
		registerValue = (registerValue & ~(0b111 << (converter * 4))) | (aaFilter << (converter * 4));
		/* Sends changed register settings to chip. */
		status = AK4619_SetRegister_ADCDigitalFilterReg(device, &registerValue);
	} else {
		/* Reads current setting of register. */
		status = AK4619_GetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
		if(status) return EXIT_FAILURE;
		/* Combines current value of register with value that has to be changed. */
		registerValue = (registerValue & ~(0b11 << ((converter - 2) * 2))) | (aaFilter << ((converter - 2) * 2));
		/* Sends changed register settings to chip. */
		status = AK4619_SetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
	}
	return status;
}

uint8_t AK4619_GetAntiAliasingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DigitalFilter_t *aaFilter) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(converter > ak4619_DAC2) {
		return EXIT_FAILURE;
	}
	if(converter <= ak4619_ADC2) {
		/* Reads current setting of register. */
		status = AK4619_GetRegister_ADCDigitalFilterReg(device, &registerValue);
		/* Combines current value of register with value that has to be changed. */
		*aaFilter = (registerValue & (0b111 << (converter * 4))) >> (converter * 4);
	} else {
		/* Reads current setting of register. */
		status = AK4619_GetRegister_DACMuteAndFilterSettingsReg(device, &registerValue);
		/* Combines current value of register with value that has to be changed. */
		*aaFilter = (registerValue & (0b11 << ((converter - 2) * 2))) >> ((converter - 2) * 2);
	}
	return status;
}

uint8_t AK4619_SetADCInputMode(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_AnalogInputMode_t inputMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(adc > ak4619_ADC2R || inputMode > ak4619_PseudoDifferential) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_ADCAnalogInputReg(device, &registerValue);
	if(status) return EXIT_FAILURE;
	/* Combines current value of register with value that has to be changed. */
	registerValue = (registerValue & ~(0b11 << (adc * 2))) | (inputMode << (adc * 2));
	/* Sends changed register settings to chip. */
	status = AK4619_SetRegister_ADCAnalogInputReg(device, &registerValue);
	return status;
}

uint8_t AK4619_GetADCInputMode(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_AnalogInputMode_t *inputMode) {
	/* Holds hal status for error catching. */
	uint8_t status = 0;
	/* Holds register data. */
	uint8_t registerValue = 0;
	/* Checks for input errors. */
	if(adc > ak4619_ADC2R) {
		return EXIT_FAILURE;
	}
	/* Reads current setting of register. */
	status = AK4619_GetRegister_ADCAnalogInputReg(device, &registerValue);
	/* Combines current value of register with value that has to be changed. */
	*inputMode = (registerValue & (0b11 << (adc * 2))) >> (adc * 2);
	return status;
}

uint8_t AK4619_DigitalVolume_dBtoBinary(ak4619_ADCDAC_t adcdac, float volume_dB) {
    int result = 0;
    result = (int)(volume_dB * 2);
    if(adcdac == ak4619_ADC) {
        result = 48 - result;
    } else {
        result = 24 - result;
    }
    if(result < 0) {
        result = 0;
    } else if(result > 255) {
        result = 255;
    }
    return (uint8_t)result;
}

/**
 * @}
 */

/**
 * @}
 */