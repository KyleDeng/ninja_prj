/**
 * @file sdk_led.c
 * @brief 
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#include "operate_err_code.h"
#include "hal_led.h"
#include "sdk_led.h"

#define CHECK_LED_INIT if(0==sdk_led_init_flag){return OPERATE_NO_INIT;}

#define CHECK_LED_NUM  if(n>=LED_NUM_ALL || n<=LED_NUM_NULL){return OPERATE_PARA_ERR;}

#define CHECK_LED_STATUS  if(s!=LED_STATUS_OFF && s!=LED_STATUS_ON){return OPERATE_PARA_ERR;}

static int sdk_led_init_flag = 0;
static int sdk_led_list[LED_NUM_ALL-1] = {0};

/**
 * @brief sdk_led_init 
 *
 * @return 
 */
operate_t sdk_led_init(void)
{
    sdk_led_init_flag = 1;
    return OPERATE_OK;
}

/**
 * @brief sdk_led_deinit 
 *
 * @return 
 */
const char* sdk_led_deinit(void)
{
    char *s = "deinit success";

    if(0 == sdk_led_init_flag) {
        s = "no need deinit";
        return s;
    }

    sdk_led_init_flag = 0;
    return s;
}

/**
 * @brief sdk_led_set_status 
 *
 * @param n
 * @param s
 *
 * @return 
 */
operate_t sdk_led_set_status(led_num_t n, led_status_t s)
{
    CHECK_LED_INIT;
    CHECK_LED_NUM;
    CHECK_LED_STATUS;

    sdk_led_list[n-1] = s;
    hal_led_set(n, s);
    return OPERATE_OK;
}

/**
 * @brief sdk_led_get_status 
 *
 * @param n
 * @param s
 *
 * @return 
 */
operate_t sdk_led_get_status(led_num_t n, led_status_t *s)
{
    CHECK_LED_INIT;
    CHECK_LED_NUM;

    *s = sdk_led_list[n-1];

    return OPERATE_OK;
}

/**
 * @brief sdk_led_flicker 
 *
 * @param n
 *
 * @return 
 */
operate_t sdk_led_flicker(led_num_t n)
{
    operate_t opt = OPERATE_OK;
    led_status_t s = LED_STATUS_ON;

    if((opt = sdk_led_get_status(n, &s)) != OPERATE_OK) {
        return opt;
    }

    for(int i = 0; i < 3; i++) {
        hal_led_set(n, LED_STATUS_ON);
        hal_led_set(n, LED_STATUS_OFF);
    }

    hal_led_set(n, s);

    return OPERATE_OK;
}
