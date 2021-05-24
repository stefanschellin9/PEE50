/*
 * main.h
 *
 *  Created on: 17 mei 2021
 *      Author: stefa
 */

#ifndef MAIN_H_PEE50
#define MAIN_H_PEE50

struct send_data {
    float stroom;
    float spanning_voor;
    float spanning_na;
};

enum system_status {
	reset,
	wacht,
	gereed,
	start,
	nood
}


#endif /* MAIN_H_PEE50 */
