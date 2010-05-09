#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "fs.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"


PUBLIC int (*call_vec[SYS_CALL_VEC])(MESSAGE* m_ptr)={do_settimer,do_getticks};
/*****************************************************************************
 *                                task_sys
 *****************************************************************************/
/**
 * <Ring 1> The main loop of TASK SYS.
 * 
 *****************************************************************************/
PUBLIC void task_sys()
{
	//*call_vec[SYS_CALL_VEC]={}
	MESSAGE msg;
	
	while (1) {
		int result=0;
		reset_msg(&msg);
		send_recv(RECEIVE, ANY, &msg);
		int src = msg.source;
		printf("sys receive from %d ticks: %d", src,ticks);
		switch (msg.type) {
		
		case GET_TICKS:
			result=(*call_vec[1])(&msg);
			//msg.RETVAL = ticks;
			//send_recv(SEND, src, &msg);
			break;
		case SET_TIMER:
			result=(*call_vec[0])(&msg);
			break;
		case HARD_INT:
			printf("hardint msg type from %d",msg.source);
			break;
		default:
			printf("unknown msg type from %d",msg.source);
			break;
		}
		if(result)
		{	
			//msg.source=TASK_SYS;
			printf("sys send to %d ticks: %d", src,ticks);
			send_recv(SEND,src,&msg);
		}
	}
}
PUBLIC int do_getticks(MESSAGE* m)
{
m->RETVAL=ticks;
return 1;
}
PUBLIC int do_settimer(MESSAGE* m)
{

	if(m->EXPIRETIME>0)
	{
		m->EXPIRETIME+=ticks;
		
		set_timer(m);
	}
return 1;
}
