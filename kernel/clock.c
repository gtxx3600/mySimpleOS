
#include "type.h"
#include "const.h"
#include "protect.h"
#include "fs.h"
#include "tty.h"
#include "console.h"

#include "proc.h"
#include "global.h"
#include "proto.h"
PRIVATE int do_clockclick();
PRIVATE void exp_timer(TIMER** tl,int time);
PUBLIC void set_timer(MESSAGE* m)
{
	int expire=m->EXPIRETIME;
	PROCESS* src=pid2proc(m->source);
	TIMER* tp=&(src->p_timer);
	tp->next=0;
	tp->pid=m->source;
	tp->exptime=expire;
	
	if(!timerlist||timerlist->exptime>expire)
	{
		tp->next=timerlist;
		timerlist=tp;
		nextexpiretime=expire;
		printf("Timer seted <%d>",ticks);
		return;
	}
	TIMER* search=timerlist;
	while(search->next&&search->next->exptime<expire){search=search->next;}
	
	tp->next=search->next;
	search->next=tp;
	printf("Timer seted <%d>",ticks);

	
}

PUBLIC void init_clock()
{
	/* init 8253 8254 TIMER */
	out_byte(TIMER_MODE,RATE_GENERATOR);
	out_byte(TIMER0,(u8)(TIMER_FREQ/HZ));
	out_byte(TIMER0,(u8)((TIMER_FREQ/HZ)>>8));

	put_irq_handler(CLOCK_IRQ,clock_handler);
	enable_irq(CLOCK_IRQ);
	MESSAGE m;
	m.EXPIRETIME=2000000000;
	m.source=TASK_SYS;
	set_timer(&m);
	nextexpiretime=-1;
}
PUBLIC void task_clock()
{

	MESSAGE m;
	int result;
	
	while(TRUE)
	{	
		reset_msg(&m);
		send_recv(RECEIVE,ANY,&m);
	
		switch(m.type)
		{
			case  HARD_INT:
				
				result=do_clockclick();
				break;
			default:
				printf("CLOCK: illegal request from ...%d",m.source);
		}
	}

}
PRIVATE int do_clockclick()
{
//disp_str("ddddd\n");
	schedule();
	if(nextexpiretime<=ticks)
	{
		
		exp_timer(&timerlist,ticks);
		//printf("TIMER expire~");
		if(timerlist!=0)
		{
			nextexpiretime=timerlist->exptime;
			
		}
		else
		{
			nextexpiretime=2147483647;
		}
	}
	
	return 0;
}
PRIVATE void exp_timer(TIMER** tl,int time)
{
	while(*tl!=0&&(*tl)->exptime<time)
	{
		printf("%s's Timer expire<%d>",pid2proc((*tl)->pid)->name,ticks);
		lock_notify(TASK_CLOCK,(*tl)->pid);
		(*tl)=(*tl)->next;
		
	}	

}

PUBLIC void clock_handler(int irq)
{
	

	ticks++;
	p_proc_ready->ticks--;

	if(ticks%100==0)
	{
	
		printf("!");
		
	}
	if(ticks==300)
	{
		lock_notify(INTERRUPT,TASK_FS);
	}
	//if(k_reenter!=0)
	//{
		//disp_int(k_reenter);
		
		//return;
	//}
	//printf("send notify");
	//printf("%s %d",p_proc_ready->name,p_proc_ready->ticks);
	if(p_proc_ready->ticks<=0||nextexpiretime<ticks)
	{	//printf("%s",p_proc_ready->name);
		lock_notify(INTERRUPT,TASK_CLOCK);
	}//schedule();
//	p_proc_ready++;
//	if(p_proc_ready>=proc_table+NR_TASKS)
//	{proc_table[1].priority=5;
//		p_proc_ready = proc_table;
//	}


}
/*PUBLIC void milli_delay(int msec)*/
/*{*/
/*	int i=get_ticks();*/
/*	while(((sys_get_ticks()-i)*1000/HZ) <msec)*/
/*	{*/
/*		//disp_str("milli:");*/
/*		//disp_int(sys_get_ticks()-i);*/
/*	  // asm("hlt");*/
/*	   */
/*	}*/

/*}*/
