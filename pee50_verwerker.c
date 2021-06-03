/*
 * verwerker.c
 *
 *  Created on: 13 mei 2021
 *      Author: basti
 */

#include <pee50_verwerker.h>
// implementation for a buffer with ints
// this very simple buffer can only hold one int

#define MAX_BUFF_SIZE   5
// shared variables within this file
static char buffer[MAX_BUFF_SIZE];
static bool full = false;
static int teller = 0;
//static int getter = 0;

bool buffer_put(char i)
{
    if (full) return false;
    if (i == ' ') return true;
    buffer[teller] = i;
    teller++;
    if(teller >= MAX_BUFF_SIZE){
        full = true;
    }
    return true;
}

bool buffer_get(char *p)
{
    if (teller <= 0) return false;
    full = false;
    *p = buffer[teller - 1];
    teller--;
    return true;
}

bool buffer_del_char()
{
    if (teller <= 0) return false;
    buffer[teller - 1] = '\0';
    teller--;
    return true;
}

bool buffer_is_full(void)
{
    if(teller >= MAX_BUFF_SIZE){
        return full;
    }

}

bool buffer_is_empty(void)
{
    if(teller == 0){
        return !full;
    }
}


void converter(/*char *p,*/ int *getal)
{
    char get;
    int digit0 = 0;
    int digit1 = 0;// tweede decimale getal, wordt met 10 vermenigvuldigd

    if(buffer_get(&get)) {
        digit0 = (get - '0');
    }
    if(buffer_get(&get)) {
        digit1 = 10*(get - '0');
    }

    *getal = digit0 + digit1;
//    *getal = *p - '0';
}

void check_case(char *p)
{
    if (*p <= 96)
        {
            *p += 32;
        }
}


