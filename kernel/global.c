

#define GLOBAL_VARIABLES_HERE

#include "type.h"
#include "const.h"
#include "protect.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "proc.h"
#include "global.h"
#include "proto.h"



PUBLIC	PROCESS			proc_table[NR_TASKS+NR_PROCS];

PUBLIC	char			task_stack[STACK_SIZE_TOTAL];

PUBLIC TIMER*			timerlist;
PUBLIC void* 			ring0_func;
PUBLIC int 				idle_status;
PUBLIC int				nextexpiretime;
PUBLIC 	TASK			task_table[NR_TASKS]={
											
											{task_clock,STACK_SIZE_CLOCK,"clock"},
											{task_tty,STACK_SIZE_TTY,"tty"},
											{task_sys, STACK_SIZE_SYS, "SYS"},
											{task_idle,STACK_SIZE_IDLE,"IDLE"},
											{task_hd,STACK_SIZE_HD,"HD"},
											{task_fs,STACK_SIZE_FS,"FS"}
											};
											
PUBLIC 	TASK			user_proc_table[NR_PROCS]={{TestA,STACK_SIZE_TESTA, "TESTA"},
													{TestB,STACK_SIZE_TESTB,"TESTB"}};
							
PUBLIC	irq_handler	   irq_table[NR_IRQ];

PUBLIC	system_call	sys_call_table[NR_SYS_CALL] = {sys_printx,sys_sendrec};

PUBLIC TTY 				tty_table[NR_CONSOLES];

PUBLIC CONSOLE			console_table[NR_CONSOLES];

/* FS related below */
/*****************************************************************************/
/**
 * For dd_map[k],
 * `k' is the device nr.\ dd_map[k].driver_nr is the driver nr.
 *
 * Remeber to modify include/const.h if the order is changed.
 *****************************************************************************/
struct dev_drv_map dd_map[] = {
	/* driver nr.		major device nr.
	   ----------		---------------- */
	{INVALID_DRIVER},	/**< 0 : Unused */
	{INVALID_DRIVER},	/**< 1 : Reserved for floppy driver */
	{INVALID_DRIVER},	/**< 2 : Reserved for cdrom driver */
	{TASK_HD},		/**< 3 : Hard disk */
	{TASK_TTY},		/**< 4 : TTY */
	{INVALID_DRIVER}	/**< 5 : Reserved for scsi disk driver */
};

/**
 * 6MB~7MB: buffer for FS
 */
PUBLIC	u8 *		fsbuf		= (u8*)0x600000;
PUBLIC	const int	FSBUF_SIZE	= 0x100000;

