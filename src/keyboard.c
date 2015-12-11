#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/input.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "keyboard.h"

void *keyboard(KEY_INFOV *key)
{
    int keys_fd;
    char ret[2];
    struct input_event t;
 	fd_set readfds;
 	struct timeval tv;
	int retval; 

    keys_fd = open("/dev/input/event3", O_RDONLY);
    if (keys_fd <= 0) {
        printf ("open /dev/input/event3 device error!\n");
        exit(1);
    }

    while (1) {
        //setting time
        tv.tv_sec = 10; 
        tv.tv_usec = 0; 
        FD_ZERO(&readfds); 
        FD_SET(keys_fd, &readfds); 
        retval = select(keys_fd + 1, &readfds, NULL, NULL, &tv); 

        if (retval == 0) { 
            //printf( "Time out!\n" ); 
        }

        if(FD_ISSET(keys_fd, &readfds)) {
            if (read (keys_fd, &t, sizeof (t)) == sizeof (t)) {
                if (t.type == EV_KEY)
                    if (t.value == 0 || t.value == 1) {
                        //printf ("key %d %s\n", t.code, (t.value) ? "Pressed" : "Released");
                        key->status = 1;
                        key->code = t.code;
                        key->value = t.value;
                        //if(t.code==KEY_ESC)
                        //    break;
                    }
            }
        }
    }
    close (keys_fd);
	pthread_exit((void *)2);
}
