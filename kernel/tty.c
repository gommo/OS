/*************************************************************************
*                             -=-=-= tty.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains the functions that coordinate the i/o to the user
* via the screen & keyboard
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/tty.h>
#include <os/mm/mm.h>

/* Pointer to the Active tty */
static struct tty* active_tty = NULL;
/* Pointer to tty0 */
static struct tty* tty0 = NULL;

static struct tty* create_tty(uint number);

/**
* Initialises the tty module 
*/
void init_tty()
{
    /* Create tty0 */
    tty0 = create_tty(TTY_0);
}
/**
 * Creates a tty device
 *
 * @param number The number of the tty device to create
 * @return returns a pointer to the created tty device
 */
static struct tty* create_tty(uint number)
{
    struct tty* temp_tty = NULL;
    temp_tty = (struct tty*)k_malloc(sizeof(struct tty));

    temp_tty->number = number;
    temp_tty->line_number = 0;
    memset(temp_tty->screen_page, 0, sizeof(temp_tty->screen_page));
}
/**
* Returns a ptr to the tty device specified by the input number
*
* @param uint TTY number
* @return ptr to the tty device
*/
struct tty* get_tty(uint number)
{
    /* Currently will only return tty0 */
    if (number != TTY_0)
        return NULL;
    else
        return tty0;
}