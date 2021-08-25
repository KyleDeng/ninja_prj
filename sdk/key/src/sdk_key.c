/**
 * @file sdk_key.c
 * @brief 
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#include "operate_err_code.h"
#include "hal_key.h"
#include "sdk_key.h"
#include "sdk_led.h"

static sdk_key_status_t sdk_key_list[KEY_NUM_ALL-1] = {KEY_STATUS_UP};

/**
 * @brief sdk_key_set_status 
 *
 * @param n
 * @param s
 *
 * @return 
 */
operate_t sdk_key_set_status(sdk_key_num_t n, sdk_key_status_t s)
{
    led_status_t led_s;
    sdk_led_get_status(n, &led_s);
    if(LED_STATUS_ON != led_s)  {
        return OPERATE_FAIL;
    }

    sdk_key_list[n-1] = s;
    hal_key_set(n, s);

    return OPERATE_OK;
}

/**
 * @brief sdk_key_get_status 
 *
 * @param n
 * @param s
 *
 * @return 
 */
operate_t sdk_key_get_status(sdk_key_num_t n, sdk_key_status_t *s)
{
    *s = sdk_key_list[n-1];

    return OPERATE_OK;
}
