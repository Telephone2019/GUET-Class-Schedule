// GUET-Class-Schedule.cpp: 定义应用程序的入口点。
//

#include "GUET-Class-Schedule.h"

enum section
{
    one = 1,
    two,
    three,
    four,
    five
};

enum weekday
{
    nullday,
    monday,
    tuesday,
    wednesday,
    thursday,
    friday,
    saturday,
    sunday
};

struct course
{
    weekday weekday;
    section section;
    char course_name[200];
    char location[50];
    char id[50];
    char teacher[50];
    int start_week;
    int end_week;
};

int getCourses(const char *, course *);

int makeFile(const char *, const char *);

void makeCalendar(const char *, const char *, course *, int);

void getDateT(int, int, int, int, weekday, char *);

int main()
{
    std::system("chcp 65001");
    std::system("del *.ics");
    if (makeFile(timetable_file_name, "__preprocessed_timetable_main.txt") != 0)
    {
        std::printf("error: no input file\n");
        std::getchar();
        std::system("del __*.txt");
        return -1;
    }
    course courses[150];
    int courses_num = getCourses("__preprocessed_timetable_main.txt", courses);
    for (size_t i = 0; i < courses_num; i++)
    {
        std::printf("节次：%d\n星期：%d\n%s\n(%d-%d)%s\n课号：%s\n教师：%s\n\n", courses[i].section, courses[i].weekday, courses[i].course_name, courses[i].start_week, courses[i].end_week, courses[i].location, courses[i].id, courses[i].teacher);
    }
    makeCalendar(addtion_file_name, calendar_file_name, courses, courses_num);
    std::getchar();
    std::system("del __*.txt");
    return 0;
}

int getCourses(const char *f_name, course *res)
{
    FILE *f = std::fopen(f_name, "rb");
    char line[200];
    int num = 0;
    course *res_start = res;
    while (std::fgets(line, sizeof(line), f),
           std::strcmp(line, "1,2节\n") &&
               std::strcmp(line, "3,4节\n") &&
               std::strcmp(line, "5,6节\n") &&
               std::strcmp(line, "7,8节\n") &&
               std::strcmp(line, "晚 上\n")) //break as long as any strcmp() return 0[that means equal]
        ;
    while (1)
    {
        section sec;
        if (!strcmp(line, "1,2节\n"))
        {
            sec = one;
        }
        else if (!std::strcmp(line, "3,4节\n"))
        {
            sec = two;
        }
        else if (!std::strcmp(line, "5,6节\n"))
        {
            sec = three;
        }
        else if (!std::strcmp(line, "7,8节\n"))
        {
            sec = four;
        }
        else if (!std::strcmp(line, "晚 上\n"))
        {
            sec = five;
        }
        weekday weekd = nullday;
        std::fgets(line, sizeof(line), f);
        while (1)
        {
            if (!std::strcmp(line, "\n"))
            {
                weekd = weekday(int(weekd) + 1);
            }
            else if (!std::strcmp(line, "1,2节\n") ||
                     !std::strcmp(line, "3,4节\n") ||
                     !std::strcmp(line, "5,6节\n") ||
                     !std::strcmp(line, "7,8节\n") ||
                     !std::strcmp(line, "晚 上\n"))
            {
                break;
            }
            else if (!std::strcmp(line, "备注\n"))
            {
                char teacher_line[1000];
                std::fgets(teacher_line, sizeof(teacher_line), f);
                std::fgets(teacher_line, sizeof(teacher_line), f);
                int teacher_num = 0;
                char teacher_name[200];
                char teacher_course[200];
                char *teacher_line_ch = teacher_line;
                while (*teacher_line_ch != '\n')
                {
                    if (*teacher_line_ch == ':')
                    {
                        teacher_num++;
                    }
                    teacher_line_ch++;
                }
                struct teacher
                {
                    char name[50];
                    char course[200];
                };
                teacher *teachers = (teacher *)std::malloc(sizeof(teacher) * teacher_num);
                teacher_line_ch = teacher_line;
                for (size_t i = 0; i < teacher_num; i++)
                {
                    std::sscanf(teacher_line_ch, "%[^:]:%s", teachers[i].course, teachers[i].name);
                    while (*teacher_line_ch != '\0' && *teacher_line_ch != ' ')
                    {
                        teacher_line_ch++;
                    }
                    teacher_line_ch++;
                }
                for (size_t i = 0; i < num; i++)
                {
                    for (size_t j = 0; j < teacher_num; j++)
                    {
                        if (!std::strcmp(res_start[i].course_name, teachers[j].course))
                        {
                            std::strcpy(res_start[i].teacher, teachers[j].name);
                            break;
                        }
                    }
                }
                std::free(teachers);
                std::fclose(f);
                return num;
            }
            while (1)
            {
                /*space continue, string make a course, else break*/
                std::fgets(line, sizeof(line), f);
                if (!std::strcmp(line, " \n"))
                {
                    continue;
                }
                else if (!std::strcmp(line, "1,2节\n") ||
                         !std::strcmp(line, "3,4节\n") ||
                         !std::strcmp(line, "5,6节\n") ||
                         !std::strcmp(line, "7,8节\n") ||
                         !std::strcmp(line, "晚 上\n") ||
                         !std::strcmp(line, "备注\n") ||
                         !std::strcmp(line, "\n"))
                {
                    break;
                }
                else
                {
                    (*res).weekday = weekd;
                    (*res).section = sec;
                    char *line_ch = line;
                    while (*line_ch != '\n')
                    {
                        line_ch++;
                    }
                    *line_ch = '\0';
                    std::strcpy((*res).course_name, line);
                    std::fgets(line, sizeof(line), f);
                    std::memset((*res).location, 0, sizeof((*res).location));
                    std::sscanf(line, "(%d-%d)%s", &((*res).start_week), &((*res).end_week), &((*res).location));
                    std::fgets(line, sizeof(line), f);
                    std::sscanf(line, "课号：%s", &((*res).id));
                    num++;
                    res++;
                }
            }
        }
    }
}

int makeFile(const char *source_name, const char *dest_name)
{
    FILE *source = std::fopen(source_name, "rb");
    if (source == NULL)
    {
        return -1;
    }
    std::fclose(source);

    char cmdline[200] = "\"GnuWin32\\bin\\iconv.exe\" -t UTF-8 ";
    std::strcat(cmdline, source_name);
    std::strcat(cmdline, " > __utf_8_makeFile.txt || copy /Y ");
    std::strcat(cmdline, source_name);
    std::strcat(cmdline, " __utf_8_makeFile.txt");

    std::system(cmdline);

    source = std::fopen("__utf_8_makeFile.txt", "rb");
    FILE *dest = std::fopen(dest_name, "wb+");
    int flag;
    while (flag = std::fgetc(source), flag != EOF)
    {
        if (flag == '\t')
        {
            std::fputc('\n', dest);
            std::fputc('\n', dest);
        }
        else if (flag == '\r')
        {
            ;
        }
        else
        {
            std::fputc(flag, dest);
        }
    }
    std::fclose(source);
    std::fclose(dest);
    return 0;
}

void makeCalendar(const char *pre_name, const char *dest_name, course *cs, int num)
{
    FILE *pre = std::fopen(pre_name, "rb");
    FILE *dest = std::fopen(dest_name, "wb+");
    std::fprintf(dest, "BEGIN:VCALENDAR\n"
                       "VERSION:2.0\n"
                       "PRODID:-//Telephone//Telephone//EN\n"
                       "METHOD:PUBLISH\n"
                       "BEGIN:VTIMEZONE\n"
                       "TZID:China Standard Time\n"
                       "BEGIN:STANDARD\n"
                       "DTSTART:16010101T000000\n"
                       "TZOFFSETFROM:+0000\n"
                       "TZOFFSETTO:+0000\n"
                       "END:STANDARD\n"
                       "END:VTIMEZONE\n"
                       "BEGIN:VTIMEZONE\n"
                       "TZID:中国标准时间\n"
                       "BEGIN:STANDARD\n"
                       "DTSTART:16010101T000000\n"
                       "TZOFFSETFROM:+0000\n"
                       "TZOFFSETTO:+0000\n"
                       "END:STANDARD\n"
                       "END:VTIMEZONE\n");
    if (pre)
    {
        std::fclose(pre);

        char cmdline[200] = "\"GnuWin32\\bin\\iconv.exe\" -t UTF-8 ";
        std::strcat(cmdline, pre_name);
        std::strcat(cmdline, " > __utf_8_makeCalendar.txt || copy /Y ");
        std::strcat(cmdline, pre_name);
        std::strcat(cmdline, " __utf_8_makeCalendar.txt");

        std::system(cmdline);

        pre = std::fopen("__utf_8_makeCalendar.txt", "rb");
        int flag;
        int flag_before = '\0';
        while (flag = std::fgetc(pre), flag != EOF)
        {
            if (flag == '\r')
            {
                ;
            }
            else if (flag_before == '\n' && flag == '\n')
            {
                break;
            }
            else
            {
                std::fputc(flag, dest);
                flag_before = flag;
            }
        }
        if (flag_before != '\0' && flag_before != '\n')
        {
            std::fputc('\n', dest);
        }
        std::fclose(pre);
    }
    char sections[6][2][7] = {
        {"", ""},
        {one_two_start, one_two_end},
        {three_four_start, three_four_end},
        {five_six_start, five_six_end},
        {seven_eight_start, seven_eight_end},
        {nine_ten_start, nine_ten_end}};
    char weekday_codes[8][3] = {
        "", "MO", "TU", "WE", "TH", "FR", "SA", "SU"};
    for (size_t i = 0; i < num; i++)
    {
        // BEGIN:VEVENT
        // CLASS:PUBLIC
        // DTEND;TZID="China Standard Time":20200311T160500
        // DTSTART;TZID="China Standard Time":20200311T143000
        // LOCATION:2教  208教室
        // RRULE:FREQ=WEEKLY;COUNT=8;BYDAY=WE
        // SUMMARY;LANGUAGE=zh-cn:单片机原理与应用（1920699）林科
        // TRANSP:OPAQUE
        // BEGIN:VALARM
        // TRIGGER:-PT15M
        // ACTION:DISPLAY
        // DESCRIPTION:Reminder
        // END:VALARM
        // END:VEVENT
        char date[20];
        getDateT(fyear, fmonth, fday, cs[i].start_week, cs[i].weekday, date);
        char *sec_end = sections[cs[i].section][1];
        char *sec_start = sections[cs[i].section][0];
        char *loc = cs[i].location;
        int count = cs[i].end_week - cs[i].start_week + 1;
        char *byday = weekday_codes[cs[i].weekday];
        char *course_name = cs[i].course_name;
        char *id = cs[i].id;
        char *teacher = cs[i].teacher;
        std::fprintf(dest, "BEGIN:VEVENT\n"
                           "CLASS:PUBLIC\n"
                           "DTEND;TZID=\" China Standard Time \":%sT%s\n"
                           "DTSTART;TZID=\" China Standard Time \":%sT%s\n"
                           "LOCATION:%s\n"
                           "RRULE:FREQ=WEEKLY;COUNT=%d;BYDAY=%s\n"
                           "SUMMARY;LANGUAGE=zh-cn:%s（%s）%s\n"
                           "TRANSP:OPAQUE\n"
                           "BEGIN:VALARM\n"
                           "TRIGGER:-PT15M\n"
                           "ACTION:DISPLAY\n"
                           "DESCRIPTION:Reminder\n"
                           "END:VALARM\n"
                           "END:VEVENT\n",
                     date, sec_end, date, sec_start, loc, count, byday, course_name, id, teacher);
    }
    std::fprintf(dest, "END:VCALENDAR\n");
    std::fclose(dest);
    std::printf("make calendar: done\n");
}

void getDateT(int ff_year, int ff_month, int ff_day, int week, weekday weekd, char *res)
{
    week--;
    int day_limits[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int month_limit = 12;
    if (ff_year % 400 != 0 && ff_year % 4 == 0)
    {
        day_limits[2] = 29;
    }
    else
    {
        day_limits[2] = 28;
    }
    for (size_t i = 0; i < (week * 7 + weekd - 1); i++)
    {
        ff_day++;
        if (ff_day > day_limits[ff_month])
        {
            ff_day = 1;
            ff_month++;
            if (ff_month > month_limit)
            {
                ff_month = 1;
                ff_year++;
                if (ff_year % 400 != 0 && ff_year % 4 == 0)
                {
                    day_limits[2] = 29;
                }
                else
                {
                    day_limits[2] = 28;
                }
            }
        }
    }
    std::sprintf(res, "%d%02d%02d", ff_year, ff_month, ff_day);
}
