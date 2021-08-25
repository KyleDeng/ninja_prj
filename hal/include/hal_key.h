/**
 * @file hal_key.h
 * @brief key hardware abstraction layer
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-02
 */
#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief hal_key_set 设置按键状态
 *
 * @param n: number
 * @param s: 0-up 1-down
 *
 * @return 0
 *
 * @note 不会返回操作是否成功
 */
int hal_key_set(int n, int s);


#ifdef __cplusplus
}
#endif

#endif
