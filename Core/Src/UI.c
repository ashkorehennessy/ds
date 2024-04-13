//
// Created by ashkore on 2024/2/1.
//

#include <string.h>
#include <stdio.h>
#include "UI.h"
#include "switch.h"
#include "ssd1306.h"
#include "math.h"
#include "mpu6050.h"
#include "sr04.h"
#include "IR.h"
#include "tasks.h"
#include "sercom.h"
#include "PID.h"

char buf[32];
UI_item items[8][SCREEN_H / FONT_H - 1];
int empty = 0;
int cursor_pos = 0;
int exponent = 0;
int ui_state = 0;  // 0:移动光标 1:修改数值 2:修改数值的指数
int key_pressed = 0;
extern uint16_t tof_distance;
extern uint32_t perf_time;
extern double fan_speed;
extern int16_t  tfDist;
extern float pidout;
extern PID_Incremental fan_pid;
extern int convert_pos;


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
    }
    return 0;
}

void UI_item_set_value(UI_item *item, double value) {
    if(item->type == EMPTY) return;
    switch (item->type) {
        case INT32:
            *item->var_p.int32_p = (int32_t) value;
            break;
        case INT16:
            *item->var_p.int16_p = (int16_t) value;
            break;
        case INT8:
            *item->var_p.int8_p = (int8_t) value;
            break;
        case UINT32:
            *item->var_p.uint32_p = (uint32_t) value;
            break;
        case UINT16:
            *item->var_p.uint16_p = (uint16_t) value;
            break;
        case UINT8:
            *item->var_p.uint8_p = (uint8_t) value;
            break;
        case DOUBLE:
            *item->var_p.double_p = value;
            break;
        case FLOAT:
            *item->var_p.float_p = (float) value;
            break;
    }
}

void UI_item_show_name(UI_item *item, uint16_t x, uint16_t y, FontDef font) {
    if(item->type == EMPTY) {
        ssd1306_SetCursor(x, y);
        ssd1306_WriteString("               ",font,White);
    }
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(item->name, font, White);
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
                sprintf(buf, "%10.0f", value);
            }
            break;
        case DOUBLE:
        case FLOAT:
            if(value > 9999999.99 || value < -999999.99){
                sprintf(buf, " Out range");
            } else {
                sprintf(buf, "%10.2f", UI_item_get_value(item));
            }
            break;
    }
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(buf, font, White);
}

void UI_init(){
    // 初始化所有的item为EMPTY
    for(uint8_t page = 0; page < 8; page++){
        for(int item = 0; item < SCREEN_H / FONT_H - 1; item++){
            items[page][item].type = EMPTY;
        }
    }
    UI_item_init(&items[7][0], "AngX ", DOUBLE, &mpu6050.KalmanAngleX);
    UI_item_init(&items[7][1], "AngY ", DOUBLE, &mpu6050.KalmanAngleY);
    UI_item_init(&items[7][2], "AngZ ", DOUBLE, &mpu6050.AngleZ);
    UI_item_init(&items[7][3], "fan  ", DOUBLE, &fan_speed);
    UI_item_init(&items[7][4], "vldis", UINT16, &tof_distance);
    UI_item_init(&items[7][5], "time ", UINT32, &perf_time);
    UI_item_init(&items[7][6], "IR   ", INT32, &ir.state);
    UI_item_init(&items[6][0], "Apos ", FLOAT, &A_pos);
    UI_item_init(&items[6][1], "Bpos ", FLOAT, &B_pos);
    UI_item_init(&items[6][2], "Cpos ", FLOAT, &C_pos);
    UI_item_init(&items[6][3], "Dpos ", FLOAT, &D_pos);
    UI_item_init(&items[6][4], "offst", FLOAT, &pos_offset);
    UI_item_init(&items[6][4], "taskT", FLOAT, &task_running_time);
    UI_item_init(&items[6][5], "tfdis", INT16, &tfDist);
    UI_item_init(&items[5][0], "targt", FLOAT, &target_pos);
    UI_item_init(&items[5][1], "tfdis", INT16, &tfDist);
    UI_item_init(&items[5][2], "pos  ", FLOAT, &convert_pos);
    UI_item_init(&items[5][3], "index", UINT8, &task_index);
    UI_item_init(&items[5][4], "input", FLOAT, &input_pos);
    UI_item_init(&items[5][5], "keept", UINT32, &keep_time);
    UI_item_init(&items[5][6], "pidO ", FLOAT, &pidout);

    UI_item_init(&items[1][0], "Keep ", FLOAT, &keep_time_sec);
    UI_item_init(&items[1][1], "High ", FLOAT, &convert_pos);

}

void UI_show(){
    static uint8_t last_dip_switch = 8;
    static uint8_t last_ui_state = 3;
    uint8_t dip_switch = DIP_SWITCH;
    uint8_t show_static_part = (dip_switch != last_dip_switch) || (ui_state != last_ui_state) || key_pressed;

    // 显示静态部分
    if(show_static_part) {
        last_dip_switch = dip_switch;
        last_ui_state = ui_state;
        key_pressed = 0;

        ssd1306_Fill(Black);
        // 顶部
        sprintf(buf, "Page%d", dip_switch);
        ssd1306_SetCursor(0, 0);
        ssd1306_WriteString(buf, Font_6x8, White);
        sprintf(buf, "10%+d", exponent);
        ssd1306_SetCursor(SCREEN_W - FONT_W * 4, 0);
        ssd1306_WriteString(buf, Font_6x8, White);
        for (int i = 0; i < SCREEN_H / FONT_H - 1; i++) {
            ssd1306_SetCursor(0, FONT_H * i + FONT_H);
            ssd1306_WriteChar('|', Font_6x8, White);
        }

        // 显示光标
        switch (ui_state) {
            case 0:
                ssd1306_SetCursor(0, FONT_H * cursor_pos + FONT_H);
                ssd1306_WriteChar('>', Font_6x8, White);
                break;
            case 1:
                ssd1306_SetCursor(0, FONT_H * cursor_pos + FONT_H);
                ssd1306_WriteChar('*', Font_6x8, White);
                break;
            case 2:
                ssd1306_SetCursor(0, FONT_H * cursor_pos + FONT_H);
                ssd1306_WriteChar('^', Font_6x8, White);
                break;
        }
        // 显示名字
        for(int i = 0; i < SCREEN_H / FONT_H - 1; i++){
            UI_item_show_name(&items[dip_switch][i], FONT_W, FONT_H * i + FONT_H, Font_6x8);
        }
        ssd1306_UpdateScreen();
    }

    //显示值
    for(int i = 0; i < SCREEN_H / FONT_H - 1; i++){
        UI_item_show_value(&items[dip_switch][i], SCREEN_W - FONT_W * 10, FONT_H * i + FONT_H, Font_6x8);
    }

    // 显示自定义部分
    UI_show_custom_part();
    ssd1306_UpdateScreen();

}

void UI_show_custom_part(){
    switch (DIP_SWITCH){
        case 1:
            UI_item_show_value(&items[1][0], 17, 8, Font_11x18);
            UI_item_show_value(&items[1][1], 17, 32, Font_11x18);
            UI_item_show_name(&items[1][0], 6, 8, Font_11x18);
            UI_item_show_name(&items[1][1], 6, 32, Font_11x18);
    }
}

void UI_key_process(){
    static int8_t key_forward_pressed = 0;
    static int8_t key_up_pressed = 0;
    static int8_t key_down_pressed = 0;
    static int8_t key_back_pressed = 0;

    uint8_t dip_switch = DIP_SWITCH;
    UI_item *item = &items[dip_switch][cursor_pos];
    // 切换模式
    if(KEY_FORWARD && !key_forward_pressed){
        key_forward_pressed = 1;
        key_pressed = 1;
        switch (ui_state) {
            case 0:
                ui_state = 1;
                break;
            case 1:
                ui_state = 2;
                break;
            case 2:
                ui_state = 0;
                break;
        }
    } else if(!KEY_FORWARD && key_forward_pressed){
        key_forward_pressed = 0;
    }

    // 0:上移光标 1:增加数值 2:增加数值的指数
    if(KEY_UP && !key_up_pressed){
        key_up_pressed = 1;
        key_pressed = 1;
        switch (ui_state) {
            case 0:
                cursor_pos = (cursor_pos + SCREEN_H / FONT_H - 2) % (SCREEN_H / FONT_H - 1);
                break;
            case 1:
                UI_item_set_value(item, UI_item_get_value(item) + pow(10, exponent));
                break;
            case 2:
                if(exponent < 7) exponent++;
                break;
        }
    } else if(!KEY_UP && key_up_pressed){
        key_up_pressed = 0;
    }

    // 0:下移光标 1:减少数值 2:减少数值的指数
    if(KEY_DOWN && !key_down_pressed){
        key_down_pressed = 1;
        key_pressed = 1;
        switch (ui_state) {
            case 0:
                cursor_pos = (cursor_pos + 1) % (SCREEN_H / FONT_H - 1);
                break;
            case 1:
                UI_item_set_value(item, UI_item_get_value(item) - pow(10, exponent));
                break;
            case 2:
                if(exponent > -7) exponent--;
                break;
        }
    } else if(!KEY_DOWN && key_down_pressed){
        key_down_pressed = 0;
    }

    // 切换模式（反）
    if(KEY_BACK && !key_back_pressed){
        key_back_pressed = 1;
        key_pressed = 1;
        if(task_running == 0) {
            task_running = 1;
            task_running_time = 0;
            task_start_running_time = HAL_GetTick();
        } else {
            task_running = 0;
        }
    } else if(!KEY_BACK && key_back_pressed){
        key_back_pressed = 0;
    }
}

