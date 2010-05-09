

#include "type.h"
#include "const.h"
#include "protect.h"
#include "fs.h"
#include "tty.h"
#include "console.h"

#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"

/*======================================================================*
                            kernel_main
 *======================================================================*/
PUBLIC int kernel_main()
{
	disp_str("haha ~\"kernel_main\" begins-----\n");
	TASK *			p_task			= task_table;
	PROCESS*		p_proc			= proc_table;
	char*			p_task_stack		= task_stack+STACK_SIZE_TOTAL;
	u16			selector_ldt		= SELECTOR_LDT_FIRST;
	
	int 	i;
		 u8              privilege;
         u8              rpl;
         int             eflags;
         int   prio;
	for(i=0;i<NR_TASKS+NR_PROCS;i++)
	{
		if(i<NR_TASKS)
		{
			p_task		= task_table+i;
			privilege	= PRIVILEGE_TASK;
			rpl			= RPL_TASK;
			eflags 		= 0x1202;
			prio      = 15;
		}
		else
		{
			p_task		= user_proc_table+(i-NR_TASKS);
			privilege	= PRIVILEGE_USER;
			rpl			= RPL_USER;
			eflags 		= 0x202;
			prio      = 5;
		}
		strcpy(p_proc->name,p_task->name);
		p_proc->pid = i;
		p_proc->ldt_sel	= selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS>>3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;	// change the DPL
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS>>3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;	// change the DPL
		p_proc->regs.cs	= (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss	= (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		p_proc->regs.eip= (u32)p_task->initial_eip;
		p_proc->regs.esp= (u32)p_task_stack ;
		p_proc->regs.eflags = eflags;	// IF=1, IOPL=1, bit 2 is always 1.
	
		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->q_sending = 0;
		p_proc->next_sending = 0;

		p_proc->ticks = p_proc->priority = prio;


		p_task_stack-=p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt+=1<<3;
		
		p_proc->nr_tty=0;
	}
	
	proc_table[3].ticks=2147483647;
	timerlist=0;
	nextexpiretime=2147483647;
	proc_table[NR_TASKS].nr_tty=1;
	proc_table[NR_TASKS+1].nr_tty=1;
	proc_table[2].nr_tty=2;
	//proc_table[2].priority=0;
	k_reenter=0;
	
	next_proc=p_proc_ready	= proc_table;
	
	/* init CLOCK_IRQ */
	init_clock();
	/* init KEYBOARD_IRQ */
	init_keyboard();	

 	/* init ticks */
	ticks=0;	




	disp_pos=0;
	
	for(i=0;i<80*25;i++)
	{
		disp_str(" ");
	}
	disp_pos=0;
	
	/* start the first process */
	restart();

	while(1){}
}

/*************************************************************************
                                sleep()
 ************************************************************************/
PUBLIC void sleep(int millisecs)
{

	MESSAGE m;
	reset_msg(&m);
	m.type=SET_TIMER;
	m.EXPIRETIME=millisecs/1000*HZ;
	send_recv(BOTH,TASK_SYS,&m);
	reset_msg(&m);
	printf("%s beging to sleep.ticks:%d...",p_proc_ready->name,ticks);
	send_recv(RECEIVE,ANY,&m);
	printf("%s wake up ticks:%d",p_proc_ready->name,ticks); 
}
/*****************************************************************************
 *                                get_ticks
 *****************************************************************************/
PUBLIC int get_ticks()
{
	//return 1;
	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	send_recv(BOTH, TASK_SYS, &msg);
	return msg.RETVAL;
}

/*======================================================================*
                               TestA
 *======================================================================*/
void TestA()
{
	while (1) {
	
		printf("<TestA:%d>", ticks);
		sleep(10000);
		printf("do sth");
	}

}

/*======================================================================*
								TESTB
 *======================================================================*/
void TestB()
{
	int i=0x1000;
	MESSAGE m;
	reset_msg(&m);
	while(1)
	{
		printf("<%s:%d>",p_proc_ready->name,ticks);
		//disp_color_str("B.",BRIGHT|MAKE_COLOR(BLACK,RED));
		//disp_int(get_ticks());
	
		sleep(20000);	
	}



}

/*****************************************************************************
 *                                panic
 *****************************************************************************/
PUBLIC void panic(const char *fmt, ...)
{
	int i;
	char buf[256];

	/* 4 is the size of fmt in the stack */
	va_list arg = (va_list)((char*)&fmt + 4);

	i = vsprintf(buf, fmt, arg);

	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	/* should never arrive here */
	__asm__ __volatile__("ud2");
}

