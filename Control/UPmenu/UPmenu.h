#ifndef __UPMENU_H
#define __UPMENU_H

#include "sys.h"

#define led_test_mode 1
#define beep_test_mode 2
#define sonic_test_mode 3
#define mpu_test_mode 4
#define motor_test_mode 5
#define encoder_test_mode 6
#define cover_oledmean() menu_level = 4

extern u8 upmenumode;
extern u8 level_flag;
extern u8 receive_data[33];
extern u8 receive_data_saveA;
extern u8 receive_data_saveB;
extern u8 receive_data_saveC;
extern u8 receive_data_saveD;
void Upmenumode_get(u8 choice);
void dataprocess(u8 data);
void Upmenu(void);

#endif
