/**
 ******************************************************************************
 * @file    Codecs.c
 * @version 1.0
 * @author  Till Heuer
 * @brief   Codec module.
 * @date 	Feb 27, 2024
 ******************************************************************************
 */

/** @addtogroup Core
 * @{
 */

#include <Codecs.h>

extern SPI_HandleTypeDef hspi1;

ak4619_Device_t codecs[NUMBER_OF_CODECS];

uint8_t InitialiseCodecs(void) {
	uint8_t status = 0;
	ak4619_AnalogInputMode_t inputModes[NUMBER_OF_CODECS] =
	{
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential,
			ak4619_PseudoDifferential
	};

#if NUMBER_OF_CODECS > 0
	HAL_GPIO_WritePin(EN_Codecs_1to4_GPIO_Port, EN_Codecs_1to4_Pin, GPIO_PIN_SET);
	codecs[0].NCSPin = NCS_Codec1_Pin;
	codecs[0].NCSPort = NCS_Codec1_GPIO_Port;
	codecs[0].inputMode = inputModes[0];
#endif
#if NUMBER_OF_CODECS > 1
	codecs[1].NCSPin = NCS_Codec2_Pin;
	codecs[1].NCSPort = NCS_Codec2_GPIO_Port;
	codecs[1].inputMode = inputModes[1];
#endif
#if NUMBER_OF_CODECS > 2
	codecs[2].NCSPin = NCS_Codec3_Pin;
	codecs[2].NCSPort = NCS_Codec3_GPIO_Port;
	codecs[2].inputMode = inputModes[2];
#endif
#if NUMBER_OF_CODECS > 3
	codecs[3].NCSPin = NCS_Codec4_Pin;
	codecs[3].NCSPort = NCS_Codec4_GPIO_Port;
	codecs[3].inputMode = inputModes[3];
#endif
#if NUMBER_OF_CODECS > 4
	HAL_GPIO_WritePin(EN_Codecs_5to8_GPIO_Port, EN_Codecs_5to8_Pin, GPIO_PIN_SET);
	codecs[4].NCSPin = NCS_Codec5_Pin;
	codecs[4].NCSPort = NCS_Codec5_GPIO_Port;
	codecs[4].inputMode = inputModes[4];
#endif
#if NUMBER_OF_CODECS > 5
	codecs[5].NCSPin = NCS_Codec6_Pin;
	codecs[5].NCSPort = NCS_Codec6_GPIO_Port;
	codecs[5].inputMode = inputModes[5];
#endif
#if NUMBER_OF_CODECS > 6
	codecs[6].NCSPin = NCS_Codec7_Pin;
	codecs[6].NCSPort = NCS_Codec7_GPIO_Port;
	codecs[6].inputMode = inputModes[6];
#endif
#if NUMBER_OF_CODECS > 7
	codecs[7].NCSPin = NCS_Codec8_Pin;
	codecs[7].NCSPort = NCS_Codec8_GPIO_Port;
	codecs[7].inputMode = inputModes[7];
#endif
	HAL_Delay(100);
	uint8_t muteAllADCs = 0b01100000;
	uint8_t turnOnADCs = 0b00000000;
	uint8_t muteAllDACs = 0b00110000;
	uint8_t enableAllCodecs = 0b00110111;
	uint8_t tdm128I2SMode[2] = {0b10101100, 0b00011100};
	uint8_t dacSharpFilter = 0b00000000;
	uint8_t spiInit = 0b00000000;
	HAL_SPI_Transmit(&hspi1, &spiInit, 1, 100);
	for(uint8_t i = 0; i < NUMBER_OF_CODECS; i++) {
		uint8_t adcInputMode = codecs[i].inputMode | (codecs[i].inputMode << 2) | (codecs[i].inputMode << 4) | (codecs[i].inputMode << 6);
		codecs[i].mcuInterface = ak4619_SPI;
		codecs[i].hspi = &hspi1;
		status = AK4619_Init(&codecs[i]);
		if(status)
			return status;
		/*
		status = AK4619_SetRegister_ADCMuteAndHPFControlReg(&codecs[i], &muteAllADCs);
		if(status)
			return status;
		status = AK4619_SetRegister_DACMuteAndFilterSettingsReg(&codecs[i], &muteAllDACs);
		if(status)
			return status;
		*/
		status = AK4619_SetRegister_PowerManagementReg(&codecs[i], &enableAllCodecs);
		if(status)
			return status;
		status = AK4619_SetRegister_AudioInterfaceFormatReg(&codecs[i], tdm128I2SMode);
		if(status)
			return status;
		status = AK4619_SetRegister_ADCAnalogInputReg(&codecs[i], &adcInputMode);
		if(status)
			return status;
		status = AK4619_SetRegister_DACMuteAndFilterSettingsReg(&codecs[i], &dacSharpFilter);
		if(status)
			return status;
		/*
		status = AK4619_SetRegister_ADCMuteAndHPFControlReg(&codecs[i], &turnOnADCs);
		if(status)
			return status;
		*/
		uint8_t test[2] = {0};
		AK4619_GetRegister_AudioInterfaceFormatReg(&codecs[i], test);
	}
	return EXIT_SUCCESS;
}

/**
 * @}
 */