/**
 * @file hal_key.c
 * @brief 
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#include <stdio.h>
#include "hal_key.h"

/**
 * @brief hal_key_set 
 *
 * @param n
 * @param s
 *
 * @return 
 */
int hal_key_set(int n, int s)
{
    if(0 == s) {
        printf("^^^ key %d up ^^^\n", n);
    } else {
        printf("___ key %d down ___\n", n);
    }

    return 0;
}
