/* time_console.c */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    time_t *ptimer = malloc(sizeof(*ptimer));
    //ptimer = malloc(sizeof(time_t));

    time(ptimer);
    printf("time:%s", ctime(ptimer));

    free(ptimer);
    return 0;
}