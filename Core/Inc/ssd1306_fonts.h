

#ifndef BALANCE_CAR_SSD1306_FONTS_H__
#define BALANCE_CAR_SSD1306_FONTS_H__

#include "ssd1306_conf.h"
#include "stm32f1xx_hal.h"

typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef;

extern FontDef Font_6x8;
#endif // BALANCE_CAR_SSD1306_FONTS_H__
