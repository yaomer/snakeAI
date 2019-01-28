#include <curses.h>
#include <time.h>
#include <sys/time.h>

void
set_color(void)
{
    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_RED, COLOR_RED);
    init_pair(3, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, COLOR_WHITE);
}

/*
 * 设置间隔计时器
 */
int
set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;

    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;   /*  设置初始间隔  */
    new_timeset.it_interval.tv_usec = n_usecs;

    new_timeset.it_value.tv_sec = n_sec;      /*  设置重复间隔  */
    new_timeset.it_value.tv_usec = n_usecs;
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
