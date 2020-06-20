// GUET-Class-Schedule.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>

// the time when course sections starts/ends
#define one_two_start "084500"
#define one_two_end "101000"
#define three_four_start "111000"
#define three_four_end "123500"
#define five_six_start "144500"
#define five_six_end "161000"
#define seven_eight_start "171000"
#define seven_eight_end "183500"
#define nine_ten_start "193000"
#define nine_ten_end "211000"

// the first day of the first week
#define fyear 2020
#define fmonth 4
#define fday 20

// MUST EXIST [REPLACE THE '；' WITH ' ']
#define timetable_file_name "c.txt"
// OPTION [NO EMPTY LINE(S) BETWEEN NONEMPTY LINES]
#define addtion_file_name "add.txt"
// THE FILE NAME OF THE GENERATED CALENDAR [SUFFIXED WITH .ics]
#define calendar_file_name "calendar.ics"

// TODO: 在此处引用程序需要的其他标头。
