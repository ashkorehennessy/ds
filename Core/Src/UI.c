//
// Created by ashkore on 2024/2/1.
//

#include "string.h"
#include "stdio.h"
#include "UI.h"
#include "st7735.h"
#include "mpu6050.h"

char buf[32];
UI_item items[8][8];
int empty = 0;
int cursor_pos = 0;
int exponent = 0;
int ui_state = 0;  // 0:移动光标 1:修改数值 2:修改数值的指数
int key_pressed = 0;
extern char stand;
extern char down;
extern double angle;
extern char trend;
extern char x_axis;
extern char y_axis;
extern int number;
extern int length;


void UI_item_init(UI_item *item, const char *name, int type, void *var_ptr) {
    strcpy(item->name, name);
    item->type = type;
    switch (item->type) {
        case INT32:
            item->var_p.int32_p = (int32_t *) var_ptr;
            break;
        case INT16:
            item->var_p.int16_p = (int16_t *) var_ptr;
            break;
        case INT8:
            item->var_p.int8_p = (int8_t *) var_ptr;
            break;
        case UINT32:
            item->var_p.uint32_p = (uint32_t *) var_ptr;
            break;
        case UINT16:
            item->var_p.uint16_p = (uint16_t *) var_ptr;
            break;
        case UINT8:
            item->var_p.uint8_p = (uint8_t *) var_ptr;
            break;
        case DOUBLE:
            item->var_p.double_p = (double *) var_ptr;
            break;
        case FLOAT:
            item->var_p.float_p = (float *) var_ptr;
            break;
        case EMPTY:
            item->var_p.int8_p = (int8_t *) var_ptr;
            break;
        case FUNC:
            item->var_p.func_p = var_ptr;
            break;
        case CHAR:
            item->var_p.char_p = (char *) var_ptr;
            break;
    }
}

double UI_item_get_value(UI_item *item) {
    switch (item->type) {
        case INT32:
            return (double) *item->var_p.int32_p;
        case INT16:
            return (double) *item->var_p.int16_p;
        case INT8:
            return (double) *item->var_p.int8_p;
        case UINT32:
            return (double) *item->var_p.uint32_p;
        case UINT16:
            return (double) *item->var_p.uint16_p;
        case UINT8:
            return (double) *item->var_p.uint8_p;
        case DOUBLE:
            return *item->var_p.double_p;
        case FLOAT:
            return (double) *item->var_p.float_p;
        case FUNC:
            return (double) item->var_p.func_p();
        case CHAR:
            return (double) *item->var_p.char_p;
    }
    return 0;
}

void UI_item_show_name(UI_item *item, uint16_t x, uint16_t y, FontDef font) {
    if(item->type == EMPTY) {
        ST7735_WriteString(x, y, "          ",font, ST7735_WHITE, ST7735_BLACK);
    }
    ST7735_WriteString(x, y, item->name, font, ST7735_YELLOW, ST7735_BLACK);
}

void UI_item_show_value(UI_item *item, uint16_t x, uint16_t y, FontDef font) {
    if(item->type == EMPTY) return;
    double value = UI_item_get_value(item);
    switch (item->type) {
        case INT32:
        case INT16:
        case INT8:
        case UINT32:
        case UINT16:
        case UINT8:
        case FUNC:
            if(value > 9999999999 || value < -999999999){
                sprintf(buf, " Out range");
            } else {
                sprintf(buf, "%5.0f", value);
            }
            break;
        case DOUBLE:
        case FLOAT:
            if(value > 9999999.99 || value < -999999.99){
                sprintf(buf, " Out range");
            } else {
                sprintf(buf, "%5.1f", UI_item_get_value(item));
            }
            break;
        case CHAR:
            sprintf(buf, "%5c", (char) value);
            break;
    }
    ST7735_WriteString(x, y,buf, font, ST7735_WHITE, ST7735_BLACK);
}

void UI_init(){
    // 初始化所有的item为EMPTY
    for(uint8_t page = 0; page < 8; page++){
        for(int item = 0; item < SCREEN_H / FONT_H - 1; item++){
            items[page][item].type = EMPTY;
        }
    }
    UI_item_init(&items[7][0], "Stand", CHAR, &stand);
    UI_item_init(&items[7][1], "Down", CHAR, &down);
    UI_item_init(&items[7][2], "Angle", DOUBLE, &angle);
    UI_item_init(&items[7][3], "Trend", CHAR, &trend);
    UI_item_init(&items[7][4], "X-axis", CHAR, &x_axis);
    UI_item_init(&items[7][5], "Y-axis", CHAR, &y_axis);
    UI_item_init(&items[7][6], "Number", INT32, &number);
    UI_item_init(&items[7][7], "Length", INT32, &length);

}

void UI_show(){
    static uint8_t last_dip_switch = 8;
    static uint8_t last_ui_state = 3;
    static uint8_t show_static_part = 1;

    // 显示静态部分
    if(show_static_part) {
        show_static_part = 0;
        last_ui_state = ui_state;
        key_pressed = 0;

        ST7735_FillScreen(ST7735_BLACK);
        for(int i = 0; i < 8; i++) {
            UI_item_show_name(&items[7][i], 5, i * FONT_H + 8, Font_11x18);
        }
    }

    for(int i = 0; i < 8; i++) {
        UI_item_show_value(&items[7][i], 70, i * FONT_H + 8, Font_11x18);
    }
    // 显示自定义部分
    UI_show_custom_part();

}

void UI_show_custom_part(){
}


