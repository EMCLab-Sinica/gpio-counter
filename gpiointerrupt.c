/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== gpiointerrupt.c ========
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <inttypes.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART2.h>

/* Driver configuration */
#include "ti_drivers_config.h"

static UART2_Handle uart;

void print2uartlength(const char* s, int length) {
    size_t bytesWritten = 0;
    UART2_write(uart, s, length, &bytesWritten);
}

static char print2uart_new_buf[64];
void my_printf(const char* format,...)
{
    va_list arg;
    va_start(arg, format);

    vsnprintf(print2uart_new_buf, 64, format, arg);
    print2uartlength(print2uart_new_buf, strlen(print2uart_new_buf));

    va_end(arg);
}

void timerCallback(Timer_Handle myHandle, int_fast16_t status) {
    static uint32_t last_value_power = 0, last_value_inference = 0, last_value_layer = 0;
    static uint32_t counter_power = 0, counter_inference = 0, counter_layer = 0;
    uint8_t new_value_power = GPIO_read(CONFIG_GPIO_INPUT_POWER),
            new_value_inference = GPIO_read(CONFIG_GPIO_INPUT_INFERENCE),
            new_value_layer = GPIO_read(CONFIG_GPIO_INPUT_LAYER);
    if (new_value_inference && !last_value_inference && new_value_power) {
        my_printf(".%" PRIu32 "\r\n", counter_inference);
        counter_inference = 0;
    }
    if (new_value_layer && !last_value_layer && new_value_power) {
        my_printf("L%" PRIu32 "\r\n", counter_layer);
        counter_layer = 0;
    }
    if (!new_value_power) {
        counter_power++;
        if (last_value_power) {
            //my_printf("POWER_OFF\r\n");
        }
    } else if (new_value_power && !last_value_power) {
        //my_printf("POWER_ON\r\n");
        //my_printf("R%" PRIu32 "\r\n", counter_power);
        counter_power = 0;
    }
    counter_inference++;
    counter_layer++;
    last_value_power = new_value_power;
    last_value_inference = new_value_inference;
    last_value_layer = new_value_layer;
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0) {
    Timer_Handle timer0;
    Timer_Params params;
    UART2_Params uartParams;

    /* Call driver init functions */
    GPIO_init();
    Timer_init();

    /* Configure the the button pin */
    GPIO_setConfig(CONFIG_GPIO_INPUT_POWER,     GPIO_CFG_INPUT | GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_NONE);
    GPIO_setConfig(CONFIG_GPIO_INPUT_INFERENCE, GPIO_CFG_INPUT | GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_NONE);
    GPIO_setConfig(CONFIG_GPIO_INPUT_LAYER,     GPIO_CFG_INPUT | GPIO_CFG_IN_PD | GPIO_CFG_IN_INT_NONE);

    /*
     * Setting up the timer in continuous callback mode that calls the callback
     * function every 1,000 microseconds, or 1 millisecond.
     */
    Timer_Params_init(&params);
    params.period = 1000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;

    timer0 = Timer_open(CONFIG_TIMER_0, &params);

    if (timer0 == NULL) {
        /* Failed to initialized timer */
        while (1) {}
    }

    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
        /* Failed to start timer */
        while (1) {}
    }

    /* Create a UART where the default read and write mode is BLOCKING */
    UART2_Params_init(&uartParams);
    uartParams.baudRate = 9600;

    uart = UART2_open(CONFIG_UART2_0, &uartParams);

    if (uart == NULL) {
        /* UART2_open() failed */
        while (1);
    }

    return (NULL);
}
