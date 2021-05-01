#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"

/*
 *  ============= mainThread ===============
 *
 *  Dit is de 'main' functie van het project
 *  Debugger is ingesteld om hier te te pauzeren
 *  i.p.v. bij de originele main-functie
 *
 *  Tips:
 *  1. Controleer de 'Linked Resources'
 *     onder 'project properties' om
 *     compilerproblemen te voorkomen
 *
 *  2. Gebruik 'Board.html' uit het project om
 *     de pinnamen te zien van de driver
 *
 */
void *mainThread(void *arg0)
{
    /*
     *  code komt hier
     *
     */
    GPIO_init();

    while(1)
    {
        GPIO_toggle(Board_GPIO_LED0);
    }
}
