/**
 * @file main.c
 * @brief 
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#include "operate_err_code.h"
#include "sdk_key.h"
#include "sdk_led.h"

/**
 * @brief main 
 *
 * @param argc
 * @param argv[]
 *
 * @return 
 */
int main(int argc, char* argv[])
{
    sdk_led_init();
    sdk_led_set_status(LED_NUM_1, LED_STATUS_ON);
    sdk_key_set_status(KEY_NUM_1, KEY_STATUS_UP);
    sdk_led_set_status(LED_NUM_1, LED_STATUS_OFF);

    return 0;
}
