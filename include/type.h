
#ifndef	_ORANGES_TYPE_H_
#define	_ORANGES_TYPE_H_

typedef	unsigned long long	u64;
typedef	unsigned int		u32;
typedef	unsigned short		u16;
typedef	unsigned char		u8;
typedef struct descriptor DESCRIPTOR;
typedef struct s_stackframe STACK_FRAME;
typedef struct proc			PROCESS;
typedef struct gate			GATE;
typedef struct tss			TSS;

typedef	void	(*int_handler)	();
typedef void 	(*task_f) ();
typedef void 	(*irq_handler)	(int irq);
typedef char*	va_list;
typedef void*	system_call;
struct timer;
struct timer
{
	int exptime;
	struct timer * next;
	u32 pid;
};
typedef struct timer TIMER;
struct mess1 {
	int m1i1;
	int m1i2;
	int m1i3;
	int m1i4;
};
struct mess2 {
	void* m2p1;
	void* m2p2;
	void* m2p3;
	void* m2p4;
};
struct mess3 {
	int	m3i1;
	int	m3i2;
	int	m3i3;
	int	m3i4;
	u64	m3l1;
	u64	m3l2;
	void*	m3p1;
	void*	m3p2;
};
typedef struct {
	int source;
	int type;
	union {
		struct mess1 m1;
		struct mess2 m2;
		struct mess3 m3;
	} u;
} MESSAGE;

struct pci_dev_func{
/*00*/	u16		provider;    //供应商
		u16		dev_id;		 //设备id
/*04*/	u16		command_reg;     //命令寄存器
		u16		status_reg;	//状态寄存器
/*08*/	u8		version;    
		u8		programable;   //  类代码：可编程
		u8		sub_type;	   //		  子类
		u8		base_type;    //         主类
/*0c*/	u8		cashe_size;    
		u8		delay_time; 
		u8		conf_head;   //配置头类型
		u8		selftest;       //内建自测
/*10*/	u32		base_reg0;      //基地址寄存器
		u32		base_reg1;
		u32		base_reg2;
		u32		base_reg3;
		u32		base_reg4;
		u32		base_reg5;
/*28*/	u32		card_bus_ptr;   //卡总线指针
/*2c*/	u16		sub_system_provider;   //子系统供应商id
		u16		sub_system_version;    
/*30*/	u32		expand_rom_base_reg;   //扩展rom基地址寄存器
/*34*/	u16		perform_ptr;           //性能指针
		u16		reserved0;			//保留
/*38*/	u32		reserved1;
/*3c*/	u8		int_line;          //中断线
		u8		int_nr;            //中断引脚
		u8		tick_req;		   //时间片请求
		u8		priv_req;		   //优先级请求
		u32     other[48];
};//共有64个32位配置寄存器
typedef struct pci_dev_func		PCI;
#endif /* _ORANGES_TYPE_H_ */
