/*************************************************************************
*                            -=-=-= sched.c =-=-=-
*
* THIS FILE WAS CREATED FOR ELEC4710 - REALTIME SYSTEMS
* MAJOR PROJECT ASSIGNMENT
*
* This file contains code responsible for the scheduling of the processes
* and threads in the operating system
*
* Compiler: gcc
*
* Author: Colin Goudie
**************************************************************************/
#include <os/taskm/sched.h>
#include <os/taskm/taskm.h>

/** Current PID to give out */
static ulong    current_pid = 0;
/** Current TID to give out */
static ulong    current_tid = 0;
/** This is the array of processes */
static struct process process_list[MAX_PROCESSES];



void init_sched()
{
    //Initialise our idle task
    //strcpy( process_list[0].name, "Idle Task" );
    process_list[0].pid = get_pid();
    process_list[0].state = TASK_WAITING;


}

ulong get_pid()
{
    return current_pid++;
}

ulong get_tid()
{
    return current_tid++;
}

