/*
 * This file is part of arpilot.
 *
 * Copyright (C) 2012  D.Herrendoerfer
 *
 *   arpilot is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   arpilot is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with arpilot.  If not, see <http://www.gnu.org/licenses/>.
 */

/* This is a short example of how to use the library for other
 * puropses.
 * Joystick racer example.
 *
 * With this example the drone can be controlled by a Microsoft XBOX 360
 * controller with the PC receiver. Layout is like in a racing game.
 * START starts the drone and BACK lands it.
 *
 * Enjoy :)*/


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#include "drone.h"

#include "command.h"
#include "controller.h"
#include "init.h"
#include "log.h"
#include "network.h"
#include "web.h"

#define JOY_DEV "/dev/input/js0"

axis_to_val(int axis)
{
	int retval = 0;

	if (abs(axis) < 10000)
		return 0;

	retval = abs(axis) - 10000;
	retval = retval*1000 / 22767;

	if (axis < 0)
		retval = retval * -1;

	return retval;
}

pedal_to_val(int pedal)
{
	int retval = 0;

	retval = pedal + 32767;

	if (retval <  10000)
		return 0;

	retval = (retval-10000)*1000 / 55534;

	return retval;
}

int main()
{
        int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
        char *button=NULL, name_of_joystick[80];
        struct js_event js;

        int pitch,roll,gaz,yaw;

        if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
        {
                printf( "Couldn't open joystick\n" );
                return -1;
        }

        ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
        ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
        ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

        axis = (int *) calloc( num_of_axis, sizeof( int ) );
        button = (char *) calloc( num_of_buttons, sizeof( char ) );

        printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
                , name_of_joystick
                , num_of_axis
                , num_of_buttons );

        fcntl( joy_fd, F_SETFL, O_NONBLOCK );   /* use non-blocking mode */

        /* Init libarpilot */
        if (setup_net())
        	exit(1);
        printf("libarpilot:net \n");

        if (init_navdata())
        	exit(1);
        printf("libarpilot:nav \n");

        if (config_init())
        	exit(1);
        printf("libarpilot:ini \n");

        if (init_web())
        	exit(1);
        printf("libarpilot:web \n");

        // Start the main loop

        while( 1 )      /* infinite loop */
        {
                /* read the joystick state */
                read(joy_fd, &js, sizeof(struct js_event));

                /* see what to do with the event */
                switch (js.type & ~JS_EVENT_INIT)
                {
                        case JS_EVENT_AXIS:
                                axis   [ js.number ] = js.value;
                                break;
                        case JS_EVENT_BUTTON:
                                button [ js.number ] = js.value;
                                break;
                }

                printf( "X: %6d  Y: %6d  ", axis_to_val(axis[0]), axis_to_val(axis[1]) );
                printf("Z1: %6d  ", pedal_to_val(axis[2]) );
                printf("Z2: %6d  ", pedal_to_val(axis[5]) );

                roll = axis_to_val(axis[3]);
                pitch = pedal_to_val(axis[2]) - pedal_to_val(axis[5]);
                gaz = axis_to_val(axis[1]);
                yaw = axis_to_val(axis[0]);

                printf("move: %6d,%6d,%6d,%6d,", roll,pitch,gaz,yaw);
                command_move(roll,pitch,gaz,yaw); /*Move the drone*/

                if (button[7] == 1 ) {
                	command_state(1); /*Start the drone*/
                	printf(" START ");
                }

                if (button[6] == 1 ){
                	command_state(0);/*Land the drone*/
                	printf(" LAND ");
                }

                printf("  \r");
                fflush(stdout);

                process(); /*Let the drone controller do its thing*/
        }

        close( joy_fd );
        return 0;
}