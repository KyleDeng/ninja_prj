/**
 * @file sdk_key.h
 * @brief 按键相关操作
 * @author huatuo
 * @version 1.0.0
 * @date 2021-04-07
 */
#ifndef __SDK_KEY_H__
#define __SDK_KEY_H__


#ifdef __cplusplus
extern "C" {
#endif

typedef enum sdk_key_status_t {
    KEY_STATUS_UP   = 0,
    KEY_STATUS_DOWN,
}sdk_key_status_t;

typedef enum sdk_key_num_t {
    KEY_NUM_1 = 1,
    KEY_NUM_2,
    KEY_NUM_3,
    KEY_NUM_ALL,
}sdk_key_num_t;

/**
 * @brief sdk_key_set_status 
 *
 * @param n
 * @param s
 *
 * @return 0: success
 *         -1: fail
 */
operate_t sdk_key_set_status(sdk_key_num_t n, sdk_key_status_t s);

/**
 * @brief sdk_key_get_status 
 *
 * @param n
 * @param s
 *
 * @return 0: 成功
 *         -1: 失败
 */
operate_t sdk_key_get_status(sdk_key_num_t n, sdk_key_status_t *s);


#ifdef __cplusplus
}
#endif

#endif
