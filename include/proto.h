/* klib.asm */
PUBLIC void	out_byte(u16 port, u8 value);
PUBLIC u8	in_byte(u16 port);
PUBLIC void out_dword(u16 port,u32 dword);
PUBLIC u32	in_dword(u16 port);
PUBLIC u32  read_pci(u16 port ,u32 value);
PUBLIC void	disp_str(char * info);
PUBLIC void	disp_color_str(char * info, int color);
PUBLIC void	disable_irq(int irq);
PUBLIC void	enable_irq(int irq);
PUBLIC void	disable_int();
PUBLIC void	enable_int();
PUBLIC void task_idle();
PUBLIC void do_halt();
PUBLIC void	port_read(u16 port, void* buf, int n);
PUBLIC void	port_write(u16 port, void* buf, int n);

/* string.asm */
PUBLIC char*	strcpy(char* dst, const char* src);

/* protect.c */
PUBLIC void	init_prot();
PUBLIC u32	seg2phys(u16 seg);

/* klib.c */
PUBLIC void	delay(int time);
PUBLIC void	disp_int(int input);
PUBLIC char *	itoa(char * str, int num);

/* kernel.asm */
PUBLIC void restart();
PUBLIC void sys_call();  
PUBLIC void ring0_call();

/* kernel/hd.c */
PUBLIC void	task_hd();
PUBLIC void	hd_handler(int irq);

/* main.c */
PUBLIC int  get_ticks();
PUBLIC void TestA();
PUBLIC void TestB();
PUBLIC void panic(const char *fmt, ...);
PUBLIC void task_idle_main();


/* i8259.c */
PUBLIC void init_8259A();
PUBLIC void put_irq_handler(int irq, irq_handler handler);
PUBLIC void spurious_irq(int irq);

/* clock.c */
PUBLIC void clock_handler(int irq);
PUBLIC void init_clock();
PUBLIC void milli_delay(int milli_sec);
PUBLIC void task_clock();

/* keyboard.c */
PUBLIC void init_keyboard();
PUBLIC void keyboard_read(TTY* p_tty);

/* tty.c */
PUBLIC void task_tty();
PUBLIC void in_process(TTY* p_tty, u32 key);
//PUBLIC int	sys_write(char* buf,int len,PROCESS* p_proc);

/* systask.c */
PUBLIC void task_sys();
PUBLIC int do_getticks(MESSAGE* m);
PUBLIC int do_settimer(MESSAGE* m);
/* console.c */
PUBLIC void out_char(CONSOLE* p_con, char ch);
PUBLIC void scroll_screen(CONSOLE* p_con, int direction);
PUBLIC void select_console(int nr_console);
PUBLIC void init_screen(TTY* p_tty);
PUBLIC int  is_current_console(CONSOLE* p_con);

/* printf.c */
PUBLIC  int     printf(const char *fmt, ...);
#define	printl	printf

/* vsprintf.c */
PUBLIC  int     vsprintf(char *buf, const char *fmt, va_list args);
PUBLIC	int		sprintf(char *buf, const char *fmt, ...);

/* proc.c */
PUBLIC	void	schedule();
PUBLIC	void*	va2la(int pid, void* va);
PUBLIC	int		ldt_seg_linear(struct proc* p, int idx);
PUBLIC	void	reset_msg(MESSAGE* p);
PUBLIC	void	dump_msg(const char * title, MESSAGE* m);
PUBLIC	void	dump_proc(struct proc * p);
PUBLIC	int		send_recv(int function, int src_dest, MESSAGE* msg);
PUBLIC  void 	lock_notify(int src,int dst);
PUBLIC 	void 	dump_msg(const char * title, MESSAGE* m);
/* lib/misc.c */
PUBLIC void spin(char * func_name);

/* fs/main.c */
PUBLIC void task_fs();

/* ������ϵͳ������� */

/* proc.c */
PUBLIC	int	sys_sendrec(int function, int src_dest, MESSAGE* m, struct proc* p);
PUBLIC	int	sys_printx(int _unused1, int _unused2, char* s, struct proc * p_proc);

/* syscall.asm */
PUBLIC void printx(char* s);
PUBLIC int sendrec(int function,int src_dest,MESSAGE* p_msg);

     
//PUBLIC  int     get_ticks();
//PUBLIC  void 	write(char* buf,int len);

