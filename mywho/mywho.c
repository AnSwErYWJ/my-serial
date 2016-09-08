/*************************************************************************
	> File Name: mywho.c
	> Author: weijie.yuan
	> Mail: 
	> Created Time: Tue 02 Aug 2016 04:07:26 PM CST
 ************************************************************************/

#include <stdio.h>
#include "mywho.h"
#include <utmp.h>
#include <time.h>

/* var/run/utmp  /var/log/wtmp */
int mywho()
{
    struct utmp *um;
    
    /* option q, count all login names and number of users logged on */
    if (opt_q)
    {
        int users = 0;

        /* open utmp file */
        while ((um = getutent()))
        {
            /* get user process */
            if(um->ut_type != USER_PROCESS)
                continue;
            printf("%s  ",um->ut_user);
            users += 1;
        }
        printf("\n# users=%d\n",users);
        /* close utmp file */
        endutent();
        return 0;
    }

    /* print line of column headings */
    if (opt_H)
        printf("%-12s%-12s%-20.20s  %s\n","NAME","LINE","TIME","COMMENT");    

    /* time of last system boot */
    if (opt_b)
    {
        int n = 0;
        time_t tm;
        char timebuf[24];
        
        utmpname(_PATH_WTMP);//set wtmp path

        /* open wtmp file */
        while ((um = getutent()))
        {
            /* get boot time */
            if(um->ut_type != BOOT_TIME)
                continue;
            n++;
        }

        setutent();// read from head

        while (n--)
        {
            um = getutent();

            if (um->ut_type != BOOT_TIME)
            {
                n++;
                continue;
            }
        }
        
        tm = (time_t)(um->ut_tv.tv_sec);
        strftime(timebuf,24,"%F %R",localtime(&tm));

        printf("system boot  %-20.20s\n",timebuf);

        /* close wtmp file */
        endutent();
        return 0;
    }

    /* no option,open utmp */
    while ((um = getutent()))
    {
        if(um->ut_type != USER_PROCESS)
            continue;
        
        time_t tm;
        char timebuf[24];
        
        tm = (time_t)(um->ut_tv.tv_sec);
        strftime(timebuf,24,"%F %R",localtime(&tm));
        
        printf("%-12s%-12s%-20.20s  (%s)\n",um->ut_user,um->ut_line,timebuf,um->ut_host);
    }

    endutent();
    return 0;
}
