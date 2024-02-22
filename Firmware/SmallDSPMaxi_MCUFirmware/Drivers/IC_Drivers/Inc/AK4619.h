/**
 ******************************************************************************
 * @file    AK4619.h
 * @version 1.0
 * @author  Till Heuer
 * @brief   Headerfile for driver for AKM AK4619 4 channel audio codec.
 * @date 	Feb 20, 2024
 * @verbatim
 * Datasheet: https://www.akm.com/content/dam/documents/products/audio/audio-codec/ak4619vn/ak4619vn-en-datasheet.pdf
 * @endverbatim
 ******************************************************************************
 */

#ifdef __cplusplus
	extern "C" {
#endif

/** @addtogroup IC_Drivers
 * @{
 */

/** @addtogroup Audio_Codec
 * @{
 */

#ifndef IC_DRIVERS_INC_AK4619_H_
#define IC_DRIVERS_INC_AK4619_H_

#include <stdint.h>
#include <stdlib.h>
#include <stm32f0xx_hal.h>

#define AK4619_COM_MAX_ATTEMPTS 		3
#define AK4619_COM_TIMEOUT 			100
#define AK4619_SPI_ATTEMPT_DELAY		1
#define AK4619_I2C_ATTEMPT_DELAY		10

/* 7Bit I2C slave address */
#define AK4619_I2C_DEVICE_ADDRESS 	0b0010000

/* Defines for register addresses. */
#define AK4619_REG_POWER_MANAGEMENT 				0x00
#define AK4619_REG_AUDIO_INTERFACE_FORMAT_A 		0x01
#define AK4619_REG_AUDIO_INTERFACE_FORMAT_B 		0x02
#define AK4619_REG_SYSTEM_CLOCK_SETTING 			0x03
#define AK4619_REG_MIC_AMP_GAIN_A				0x04
#define AK4619_REG_MIC_AMP_GAIN_B				0x05
#define AK4619_REG_ADC1_L_DIGITAL_VOLUME	 		0x06
#define AK4619_REG_ADC1_R_DIGITAL_VOLUME	 		0x07
#define AK4619_REG_ADC2_L_DIGITAL_VOLUME	 		0x08
#define AK4619_REG_ADC2_R_DIGITAL_VOLUME	 		0x09
#define AK4619_REG_ADC_DIGITAL_FILTER_SETTING  	0x0A
#define AK4619_REG_ADC_ANALOG_INPUT_SETTING		0x0B
#define AK4619_REG_ADC_MUTE_AND_HPF_CONTROL 		0x0D
#define AK4619_REG_DAC1_L_DIGITAL_VOLUME	 		0x0E
#define AK4619_REG_DAC1_R_DIGITAL_VOLUME	 		0x0F
#define AK4619_REG_DAC2_L_DIGITAL_VOLUME	 		0x10
#define AK4619_REG_DAC2_R_DIGITAL_VOLUME	 		0x11
#define AK4619_REG_DAC_INPUT_SELECT_SETTING 		0x12
#define AK4619_REG_DAC_DEEMPHASIS_SETTING 		0x13
#define AK4619_REG_DAC_MUTE_AND_FILTER_SETTING 	0x14

/* Define for digital volume mute setting */
#define AK4619_MUTE	-115.5f

typedef enum {
	ak4619_Uninit,
	ak4619_I2C,
	ak4619_SPI
}ak4619_MCUInterface_t;

typedef enum {
	ak4619_A0_GND,
	ak4619_A0_Vdd
}ak4619_A0_t;

typedef enum {
	ak4619_ResetState,
	ak4619_Enabled
}ak4619_ResetMode_t;

typedef enum {
	ak4619_PowerDownState,
	ak4619_NormalOperation
}ak4619_PowerMode_t;

typedef enum {
	ak4619_StereoMode,
	ak4619_TDMMode
}ak4619_TDMMode_t;

typedef enum {
	ak4619_Stereo_I2SMode,
	ak4619_Stereo_MSBJustifiedMode,
	ak4619_Stereo_PCMShortFrameMode,
	ak4619_Stereo_PCMLongFrameMode,
	ak4619_TDM128_I2SMode,
	ak4619_TDM128_MSBJustifiedMode,
	ak4619_TDM256_I2SMode,
	ak4619_TDM256_MSBJustifiedMode
}ak4619_AudioInterfaceFormat_t;

typedef enum {
	ak4619_24Bit,
	ak4619_20Bit,
	ak4619_16Bit,
	ak4619_32Bit
}ak4619_WordLength_t;

typedef enum {
	ak4619_BCLK_FallingEdge,
	ak4619_BCLK_RisingEdge
}ak4619_BCLKEdge_t;

typedef enum {
	ak4619_SDOUT_SlowMode,
	ak4619_SDOUT_FastMode
}ak4619_SDOUTFastMode_t;

typedef enum {
	ak4619_SDOUT_LRCKEdgeBasisSlotStart,
	ak4619_SDOUT_SlotLengthBasisSlotStart
}ak4619_SlotStartPosition_t;

typedef enum {
	ak4619_256Fs_48kHz,
	ak4619_256Fs_96kHz,
	ak4619_384Fs_48kHz,
	ak4619_512Fs_48kHz,
	ak4619_128Fs_192kHz
}ak4619_SystemClockSetting_t;

typedef enum {
	ak4619_N6dB_MicGain,
	ak4619_N3dB_MicGain,
	ak4619_0dB_MicGain,
	ak4619_3dB_MicGain,
	ak4619_6dB_MicGain,
	ak4619_9dB_MicGain,
	ak4619_12dB_MicGain,
	ak4619_15dB_MicGain,
	ak4619_18dB_MicGain,
	ak4619_21dB_MicGain,
	ak4619_24dB_MicGain,
	ak4619_27dB_MicGain
}ak4619_MicGainSetting_t;

typedef enum {
	ak4619_SharpRollOffFilter,
	ak4619_SlowRollOffFilter,
	ak4619_ShortDelaySharpRollOffFilter,
	ak4619_ShortDelaySlowRollOffFilter,
	ak4619_VoiceFilter
}ak4619_DigitalFilter_t;

typedef enum {
	ak4619_Differential,
	ak4619_SingleEnded1,
	ak4619_SingleEnded2,
	ak4619_PseudoDifferential
}ak4619_AnalogInputMode_t;

typedef enum {
	ak4619_4overFs_TransitionTime,
	ak4619_16overFs_TransitionTime
}ak4619_DigitalVolumeTransitionTime_t;

typedef enum {
	ak4619_SoftMuteDisabled,
	ak4619_SoftMuteEnabled
}ak4619_SoftMuteSetting_t;

typedef enum {
	ak4619_DCBlockingFilterEnabled,
	ak4619_DCBlockingFilterDisabled
}ak4619_DCBlockingFilterMode_t;

typedef enum {
	ak4619_SDIN1,
	ak4619_SDIN2,
	ak4619_SDOUT1,
	ak4619_SDOUT2
}ak4619_DACSourceSetting_t;

typedef enum {
	ak4619_Deemphasis_44_1kHz,
	ak4619_Deemphasis_Off,
	ak4619_Deemphasis_48kHz,
	ak4619_Deemphasis_32kHz
}ak4619_DeemphasisMode_t;

typedef enum {
	ak4619_ADC1,
	ak4619_ADC2,
	ak4619_DAC1,
	ak4619_DAC2
}ak4619_Converter_t;

typedef enum {
	ak4619_ADC1L,
	ak4619_ADC1R,
	ak4619_ADC2L,
	ak4619_ADC2R
}ak4619_ADC_t;

typedef enum {
	ak4619_ADC,
	ak4619_DAC
}ak4619_ADCDAC_t;

typedef struct {
	ak4619_MCUInterface_t mcuInterface;
	union {
		I2C_HandleTypeDef *hi2c;
		SPI_HandleTypeDef *hspi;
	};
	union {
		ak4619_A0_t a0;
		struct {
			uint16_t NCSPin;
			GPIO_TypeDef* NCSPort;
		};
	};
}ak4619_Device_t;

uint8_t AK4619_ActivateSPIComunication(ak4619_Device_t *device);
uint8_t AK4619_WriteSPI(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data);
uint8_t AK4619_ReadSPI(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data);
uint8_t AK4619_WriteI2C(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data);
uint8_t AK4619_ReadI2C(ak4619_Device_t *device, uint8_t registerAddress, uint8_t *data);

uint8_t AK4619_Init(ak4619_Device_t *device);

/**
 * Access Registers as a whole:
 */
uint8_t AK4619_SetRegister_PowerManagementReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_PowerManagementReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_AudioInterfaceFormatReg(ak4619_Device_t *device, uint8_t registerValue[2]);
uint8_t AK4619_GetRegister_AudioInterfaceFormatReg(ak4619_Device_t *device, uint8_t registerValue[2]);
uint8_t AK4619_SetRegister_SystemClockReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_SystemClockReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_MicAmpReg(ak4619_Device_t *device, uint8_t registerValue[2]);
uint8_t AK4619_GetRegister_MicAmpReg(ak4619_Device_t *device, uint8_t registerValue[2]);
uint8_t AK4619_SetRegister_ADCDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]);
uint8_t AK4619_GetRegister_ADCDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]);
uint8_t AK4619_SetRegister_ADCDigitalFilterReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_ADCDigitalFilterReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_ADCAnalogInputReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_ADCAnalogInputReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_ADCMuteAndHPFControlReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_ADCMuteAndHPFControlReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_DACDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]);
uint8_t AK4619_GetRegister_DACDigitalVolumeReg(ak4619_Device_t *device, uint8_t registerValue[4]);
uint8_t AK4619_SetRegister_DACInputSettingsReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_DACInputSettingsReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_DACDeemphasisReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_DACDeemphasisReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_SetRegister_DACMuteAndFilterSettingsReg(ak4619_Device_t *device, uint8_t *registerValue);
uint8_t AK4619_GetRegister_DACMuteAndFilterSettingsReg(ak4619_Device_t *device, uint8_t *registerValue);

/**
 * Set register settings individually.
 */
uint8_t AK4619_SetPowerSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_PowerMode_t powerMode);
uint8_t AK4619_GetPowerSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_PowerMode_t *powerMode);
uint8_t AK4619_SetResetMode(ak4619_Device_t *device, ak4619_ResetMode_t resetMode);
uint8_t AK4619_GetResetMode(ak4619_Device_t *device, ak4619_ResetMode_t *resetMode);
uint8_t AK4619_SetTDMMode(ak4619_Device_t *device, ak4619_TDMMode_t tdmMode);
uint8_t AK4619_GetTDMMode(ak4619_Device_t *device, ak4619_TDMMode_t *tdmMode);
uint8_t AK4619_SetAudioInterfaceFormat(ak4619_Device_t *device, ak4619_AudioInterfaceFormat_t ifFormat);
uint8_t AK4619_GetAudioInterfaceFormat(ak4619_Device_t *device, ak4619_AudioInterfaceFormat_t *ifFormat);
uint8_t AK4619_SetSlotLength(ak4619_Device_t *device, ak4619_WordLength_t slotLength);
uint8_t AK4619_GetSlotLength(ak4619_Device_t *device, ak4619_WordLength_t *slotLength);
uint8_t AK4619_SetBCLKEdge(ak4619_Device_t *device, ak4619_BCLKEdge_t bclkEdge);
uint8_t AK4619_GetBCLKEdge(ak4619_Device_t *device, ak4619_BCLKEdge_t *bclkEdge);
uint8_t AK4619_SetFastModeSetting(ak4619_Device_t *device, ak4619_SDOUTFastMode_t fastMode);
uint8_t AK4619_GetFastModeSetting(ak4619_Device_t *device, ak4619_SDOUTFastMode_t *fastMode);
uint8_t AK4619_SetSlotStart(ak4619_Device_t *device, ak4619_SlotStartPosition_t slotStart);
uint8_t AK4619_GetSlotStart(ak4619_Device_t *device, ak4619_SlotStartPosition_t *slotStart);
uint8_t AK4619_SetADCWordLength(ak4619_Device_t *device, ak4619_WordLength_t wordLength);
uint8_t AK4619_GetADCWordLength(ak4619_Device_t *device, ak4619_WordLength_t *wordLength);
uint8_t AK4619_SetDACWordLength(ak4619_Device_t *device, ak4619_WordLength_t wordLength);
uint8_t AK4619_GetDACWordLength(ak4619_Device_t *device, ak4619_WordLength_t *wordLength);
uint8_t AK4619_SetSystemClockSetting(ak4619_Device_t *device, ak4619_SystemClockSetting_t sysClock);
uint8_t AK4619_GetSystemClockSetting(ak4619_Device_t *device, ak4619_SystemClockSetting_t *sysClock);
uint8_t AK4619_SetADCVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t tranTime);
uint8_t AK4619_GetADCVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t *tranTime);
uint8_t AK4619_SetDACVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t tranTime);
uint8_t AK4619_GetDACVolumeTransitionTime(ak4619_Device_t *device, ak4619_DigitalVolumeTransitionTime_t *tranTime);
uint8_t AK4619_SetSoftMuteSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_SoftMuteSetting_t softMute);
uint8_t AK4619_GetSoftMuteSetting(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_SoftMuteSetting_t *softMute);
uint8_t AK4619_SetADCDCBlockingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DCBlockingFilterMode_t hpf);
uint8_t AK4619_GetADCDCBlockingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DCBlockingFilterMode_t *hpf);
uint8_t AK4619_SetDACInputSource(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DACSourceSetting_t source);
uint8_t AK4619_GetDACInputSource(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DACSourceSetting_t *source);
uint8_t AK4619_SetDACDeemphasisFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DeemphasisMode_t deemp);
uint8_t AK4619_GetDACDeemphasisFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DeemphasisMode_t *deemp);
uint8_t AK4619_SetMicAmpGain(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_MicGainSetting_t micGain);
uint8_t AK4619_GetMicAmpGain(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_MicGainSetting_t *micGain);
uint8_t AK4619_SetDigitalVolume(ak4619_Device_t *device, ak4619_Converter_t converter, uint8_t volume[2]);
uint8_t AK4619_GetDigitalVolume(ak4619_Device_t *device, ak4619_Converter_t converter, uint8_t volume[2]);
uint8_t AK4619_SetAntiAliasingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DigitalFilter_t aaFilter);
uint8_t AK4619_GetAntiAliasingFilter(ak4619_Device_t *device, ak4619_Converter_t converter, ak4619_DigitalFilter_t *aaFilter);
uint8_t AK4619_SetADCInputMode(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_AnalogInputMode_t inputMode);
uint8_t AK4619_GetADCInputMode(ak4619_Device_t *device, ak4619_ADC_t adc, ak4619_AnalogInputMode_t *inputMode);

uint8_t AK4619_DigitalVolume_dBtoBinary(ak4619_ADCDAC_t adcdac, float volume_dB);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
	}
#endif

#endif /* IC_DRIVERS_INC_AK4619_H_ */