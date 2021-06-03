/*
 * verwerker.h
 *
 *  Created on: 13 mei 2021
 *      Author: basti
 */

#ifndef PEE50_VERWERKER_H_
#define PEE50_VERWERKER_H_



#include <stdbool.h>

// interface for a buffer with int's

// put value i in buffer if buffer is not full
// returns true on success or false otherways
extern bool buffer_put(char i);

// get value from buffer and writes it to *p if buffer not empty
// returns true on success or false otherways
extern bool buffer_get(char *p);

extern bool buffer_del_char();

// returns true when buffer is full or false otherways
extern bool buffer_is_full(void);

// returns true when buffer is empty or false otherways
extern bool buffer_is_empty(void);

extern void converter(/*char *p,*/ int *getal);

extern void check_case(char *p);




#endif /* PEE50_VERWERKER_H_ */
