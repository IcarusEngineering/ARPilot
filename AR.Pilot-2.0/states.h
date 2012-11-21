/*
 * This file is part of libarpilot.
 *
 * Copyright (C) 2012  D.Herrendoerfer
 *
 *   libarpilot is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   libarpilot is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser Public License
 *   along with libarpilot.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <netinet/in.h>
#include "drone.h"

#ifndef STATES_H_
#define STATES_H_
/*
char commandbuffer[BUFLEN];
int  commandbufferlen=0;
*/


extern int  seq;
extern int  config_confirm_wait;

// Autopilot command vars in command.c
extern int pilot_heading;
extern int pilot_head;

extern int pilot_altitude;
extern int pilot_alti;
extern int monitor_alti;

// Init finished in init.c
extern int  init_state;

// Socket init in network.c
extern int  vid_state;
extern int  nav_state;

// Drone State in command.c
extern int drone_init;  // Initialized
extern int drone_fly;   // Flying
extern int drone_roll;  // Roll
extern int drone_pitch; // Pitch
extern int drone_gaz;   // Gaz
extern int drone_yaw;   // Yaw

extern int is_recording;

// Drone correction values in command.c
extern int err_drone_altitude;
extern int err_drone_phi;

// PCMD and REF flags in command.c
extern int drone_pcmd_flags;
extern int drone_ref_flags;

// Network Sockets in network.c
extern int nav_sock;
extern int vid_sock;

// Navdata timestamp in controller.c
extern unsigned int last_timestamp_io;
extern unsigned int last_timestamp_nav;

// Network sockets in network.h
extern struct sockaddr_in si_nav, si_vid;
extern int slen;

//int exit_now = 0;

//int neterror = 0;

// Navdata handling:

extern uint8_t navdata_buffer[NAVDATA_BUFFER_SIZE];
extern int navdata_valid;

extern navdata_unpacked_t navdata_unpacked;

#endif /* STATES_H_ */