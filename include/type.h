
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
/*00*/	u16		provider;    //��Ӧ��
		u16		dev_id;		 //�豸id
/*04*/	u16		command_reg;     //����Ĵ���
		u16		status_reg;	//״̬�Ĵ���
/*08*/	u8		version;    
		u8		programable;   //  ����룺�ɱ��
		u8		sub_type;	   //		  ����
		u8		base_type;    //         ����
/*0c*/	u8		cashe_size;    
		u8		delay_time; 
		u8		conf_head;   //����ͷ����
		u8		selftest;       //�ڽ��Բ�
/*10*/	u32		base_reg0;      //����ַ�Ĵ���
		u32		base_reg1;
		u32		base_reg2;
		u32		base_reg3;
		u32		base_reg4;
		u32		base_reg5;
/*28*/	u32		card_bus_ptr;   //������ָ��
/*2c*/	u16		sub_system_provider;   //��ϵͳ��Ӧ��id
		u16		sub_system_version;    
/*30*/	u32		expand_rom_base_reg;   //��չrom����ַ�Ĵ���
/*34*/	u16		perform_ptr;           //����ָ��
		u16		reserved0;			//����
/*38*/	u32		reserved1;
/*3c*/	u8		int_line;          //�ж���
		u8		int_nr;            //�ж�����
		u8		tick_req;		   //ʱ��Ƭ����
		u8		priv_req;		   //���ȼ�����
		u32     other[48];
};//����64��32λ���üĴ���
typedef struct pci_dev_func		PCI;
#endif /* _ORANGES_TYPE_H_ */
