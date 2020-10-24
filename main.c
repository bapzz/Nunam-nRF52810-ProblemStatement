/**
 * Copyright (c) 2014 - 2020, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/**
 * @file main.c
 * @author Biswajeet_Sahoo
 * @date Oct 24, 2020
 * @brief 
 * @note This file contains the source code for periodic counter generation at 509us
 *
 */
/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <stdio.h>
#include "timer/app_timer.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/
APP_TIMER_DEF(my_timer);     /**< Handler for repeated timer used for count */

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
uint32_t counter_value_u32 = 0;
/******************************************************************************/
/*---------------------------Private Prototypes-------------------------------*/
/******************************************************************************/
static void log_init(void);
static void event_counter_handler(void * p_context);
static void my_timer_init(void);
static void my_timer_start(uint32_t my_time_us_u32);
static void my_timer_stop(void);
/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/* Code Starts Here */

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    return;
}

/**@brief Function for handling the Event Counter.
 *
 * @details This function will be called each time the timer expires.
 *
 * @param[in] p_context  Pointer used for passing some arbitrary information (context) from the
 *                       app_start_timer() call to the timeout handler.
 * @retval   void  - returns nothing           
 */
static void event_counter_handler(void * p_context) 
{
    /* API present in app_timer.c provides the current value of the RTC1 counter*/
    counter_value_u32 = app_timer_cnt_get();
    NRF_LOG_INFO("The Counter Value is :- %d\n", counter_value_u32);

    return;    
}

/**@brief Initialization of timer.
 * @param[in] void  - nothing
 * @retval   void  - returns nothing           
 */
static void my_timer_init(void)
{
    ret_code_t err_code;

    /* Initializatin of Timers */
    err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    /* Create timers */
    err_code = app_timer_create(&my_timer, APP_TIMER_MODE_REPEATED, event_counter_handler);
    APP_ERROR_CHECK(err_code);

    return;
}

/**@brief Function for starting timers.
 * @param[in] my_time_us_u32    - Timer Timeout in microseconds
 * @param[in] (*ptr)()          - Callback function for initalization of timer module
 * 
 * @retval   void  - returns nothing           
 */
static void my_timer_start(uint32_t my_time_us_u32, void (*ptr)())
{
    ret_code_t err_code;
    uint32_t Timer_Ticks_us_u32 = 0;

    (*ptr)(); /* Initialization of timer */

    /* There is an MACRO present in app_timer.h for generating TimerTicks in Miliseconds [APP_TIMER_TICKS(my_time_us_u32)] 
     */
    /* err_code = app_timer_start(my_timer, APP_TIMER_TICKS(my_time_us_u32), NULL); */

    /* The below will give ticks in microseconds. */
    Timer_Ticks_us_u32 =  ((uint32_t)ROUNDED_DIV( (my_time_us_u32) * (uint64_t)APP_TIMER_CLOCK_FREQ, 1000 * 1000 * (APP_TIMER_CONFIG_RTC_FREQUENCY + 1)));  

    /* Start The Timer */
    err_code = app_timer_start(my_timer, Timer_Ticks_us_u32, NULL);

    APP_ERROR_CHECK(err_code);

    return;
}
/**@brief Function to stop the timer.
 * @param[in] void  - nothing
 * 
 * @retval   void  - returns nothing           
 */
static void my_timer_stop(void) {

    /* Turning off the timer */
    app_timer_stop(my_timer);

    return;
}
/**@brief Main Function
 * @param[in] void  - nothing
 * 
 * @retval   int  - The main function should not return 0.        
 */
int main(void)
{
    /* The SDK i have used for this is -- nRF5_SDK_17.0.2_d674dde */
    uint32_t my_time_us_u32 = 509; /* Setting of timer for 509 microseconds */
    
    /* Declaring the timer_init as a function pointer*/
    static void (*ptr)() = &my_timer_init;

    /* Enabling the DEBUGOUT/Logs using the nRF Log Module */
    log_init();

    /* Initialization of the Timer Module using the library in app_timer.c file */
    /* my_timer_init(); */ /* Commented out as its getting called from the my_timer_start() */

    my_timer_start(my_time_us_u32, ptr);

    NRF_LOG_INFO("Timer Started\n");

    while (1) {
        /* Infinite Loop */
        __WFI();
    }

    return 0;
}
/* Code Ends Here */