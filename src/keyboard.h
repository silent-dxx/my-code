/*
 * File Name:    keyboard.h
 * Author:       Silent_DXX
 * Mail:         <silent_dxx@163.com>
 * Created Time: 2015年06月11日 星期四 23时35分16秒
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

struct KEY_INFOV {
    int code;
    int value;
    int status;
};
typedef struct KEY_INFOV KEY_INFOV;

void *keyboard(KEY_INFOV *key);

#endif
