/**
 * @file sdk_led.h
 * @brief LED的相关操作接口
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#ifndef __SDK_LED_H__
#define __SDK_LED_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef int led_status_t;
#define LED_STATUS_OFF 0
#define LED_STATUS_ON  1

typedef int led_num_t;
#define LED_NUM_NULL 0
#define LED_NUM_1    1
#define LED_NUM_2    2
#define LED_NUM_3    3
#define LED_NUM_ALL  4

/**
 * @brief sdk_led_init 
 *
 * @return 
 */
operate_t sdk_led_init(void);

const char* sdk_led_deinit(void);
operate_t sdk_led_set_status(led_num_t n, led_status_t s);
operate_t sdk_led_get_status(led_num_t n, led_status_t *s);
operate_t sdk_led_flicker(led_num_t n);


#ifdef __cplusplus
}
#endif

#endif
