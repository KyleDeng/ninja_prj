/**
 * @file hal_led.h
 * @brief led hardware abstraction layer
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-02
 */
#ifndef __HAL_LED_H__
#define __HAL_LED_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief hal_led_set, 
 *        set led status
 *
 * @param n: number
 * @param s: 0-on 1-off
 *
 * @return 0
 */
int hal_led_set(int n, int s);


#ifdef __cplusplus
}
#endif

#endif
