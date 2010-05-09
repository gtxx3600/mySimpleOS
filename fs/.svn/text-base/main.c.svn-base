#include "type.h"
#include "config.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

#include "hd.h"


PUBLIC void task_fs()
{
	

	/* open the device: hard disk */
	
	
	MESSAGE driver_msg;
	
	
	//send_recv(BOTH, dd_map[MAJOR(ROOT_DEV)].driver_nr, &driver_msg);
	while(1){
		send_recv(RECEIVE,INTERRUPT,&driver_msg);
		
		driver_msg.type = DEV_OPEN;
		driver_msg.DEVICE = MINOR(ROOT_DEV);
		assert(dd_map[MAJOR(ROOT_DEV)].driver_nr != INVALID_DRIVER);
		send_recv(BOTH, dd_map[MAJOR(ROOT_DEV)].driver_nr, &driver_msg);
	
	}

	spin("FS");
}

