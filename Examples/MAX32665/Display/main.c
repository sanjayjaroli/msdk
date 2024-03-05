/**
 * @file    main.c
 * @brief   Display Demo
 * @details Bouncing text and blinking virtual LEDs demo.
 *          
 */

/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "board.h"
#include "mxc_device.h"
#include "mxc_delay.h"
#include "lv_conf.h"
#include "lvgl.h"

#include "sharp_mip.h"
#include "test_screen.h"
#include "resources/ecg_data.h"

/***** Definitions *****/
#define DISPLAY_HOR_RES (128)
#define DISPLAY_VER_RES (128)

// LVGL Definitions
#define DRAW_BUF_SIZE BUF_SIZE(DISPLAY_HOR_RES, DISPLAY_VER_RES)
#define TEXT_BOUNCE_DELAY 10

// LVGL DISPLAY DRIVER VARIABLES
static lv_disp_draw_buf_t disp_buf;
static lv_disp_drv_t disp_drv;
static lv_disp_t *disp;
static lv_color_t disp_buf1[DRAW_BUF_SIZE];

extern sharp_mip_dev ls013b7dh03_controller;

//============================================================================
static void set_px_cb(struct _lv_disp_drv_t *disp_drv, uint8_t *buf, lv_coord_t buf_w, lv_coord_t x,
                      lv_coord_t y, lv_color_t color, lv_opa_t opa)
{
    sharp_mip_set_buffer_pixel_util(&ls013b7dh03_controller, buf, buf_w, x, y, color.full,
                                    (LV_COLOR_SCREEN_TRANSP != opa));
}

static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    sharp_mip_flush_area(&ls013b7dh03_controller, (display_area_t *)area, (uint8_t *)color_p);
    lv_disp_flush_ready(disp_drv);
}

static void rounder_cb(struct _lv_disp_drv_t *disp_drv, lv_area_t *area)
{
    area->x1 = 0;
    area->x2 = DISPLAY_HOR_RES - 1;
}

//============================================================================
void lvgl_setup()
{
    /* LittlevGL setup */
    lv_init();

    /* Initialize `disp_buf` with the buffer(s). */
    lv_disp_draw_buf_init(&disp_buf, disp_buf1, NULL, DRAW_BUF_SIZE);

    /* Display driver setup */
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = flush_cb;
    disp_drv.set_px_cb = set_px_cb;
    disp_drv.rounder_cb = rounder_cb;
    disp_drv.hor_res = DISPLAY_HOR_RES;
    disp_drv.ver_res = DISPLAY_VER_RES;
    disp = lv_disp_drv_register(&disp_drv);

    lv_theme_t *th = lv_theme_mono_init(disp, FALSE, LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, th);
}

//============================================================================
int main(void)
{
    uint32_t last_tick;

    printf("Display demo\n");
    printf("This example uses LVGL graphics library to manage display\n");
    printf("For more demos please check: https://github.com/lvgl/lvgl\n");

    sharp_mip_init(&ls013b7dh03_controller);

    lvgl_setup();

    test_screen();

    last_tick = lv_tick_get();

    int count = 0;
    int ecg_data_size = sizeof(ecg_sample) / sizeof(ecg_sample[0]);
    while (1) {
        MXC_Delay(MXC_DELAY_MSEC(1));
        lv_tick_inc(1);

        lv_chart_set_next_value(chart, ser, ecg_sample[count++ % ecg_data_size]);

        if ((last_tick + 10) < lv_tick_get()) {
            /* The timing is not critical but should be between 1..10 ms */
            lv_task_handler();
            last_tick = lv_tick_get();
        }
    }

    return E_NO_ERROR;
}
