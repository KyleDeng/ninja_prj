/**
 * @file hal_led.c
 * @brief 
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#include <stdio.h>
#include "hal_led.h"

/**
 * @brief hal_led_set 
 *
 * @param n
 * @param s
 *
 * @return 
 */
int hal_led_set(int n, int s)
{
    if(0 == s) {
        printf("*** led %d on ***\n", n);
    } else {
        printf("--- led %d off ---\n", n);
    }

    return 0;
}
