org 0x7c00
  
  BaseOfStack equ 07c00h
  		
%include "load.inc"
jmp short start
  nop
%include "fat12hdr.inc"
	
  
start:
  mov ax,cs         
  mov ds,ax
  mov es,ax
  mov ss,ax
  mov sp,BaseOfStack
  mov ax,0600h
  mov bx,0700h
  mov cx,0
  mov dx,0184fh
  int 10h
  mov dx,0
  mov bh,0
  mov ah,02h
  int 10h
  jmp LABEL_Start           
  ;变量
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory 占用的扇区数, 在循环中会递减至零.
wSectorNo		dw	0		; 要读取的扇区号
bOdd			db	0		; 奇数还是偶数

;============================================================================
;字符串
;----------------------------------------------------------------------------
LoaderFileName		db	"LOADER  BIN", 0	; LOADER.BIN 之文件名
; 为简化代码, 下面每个字符串的长度均为 MessageLength
BootMessage:		db	"Booting  ",13,10,0; 9字节, 不够则用空格补齐. 序号 0
Message1		db	"Ready.   ",13,10,0; 9字节, 不够则用空格补齐. 序号 1
Message2		db	"No LOADER",13,10,0; 9字节, 不够则用空格补齐. 序号 2
;============================================================================

print: 
  lodsb            ;从SI地址处加载一个字节到al           
  or al,al         
  jz pe            
  mov ah,0x0e           
  mov bx,0x0002        
  int 0x10         
  jmp print       
pe:           
  ret

echo:           
  mov si,Message2   
  call print         
  jmp $        
ret
LABEL_Start:          
  	mov si,BootMessage
  	call print    
  	xor ah,ah
  	xor dl,dl
  	int 13h    ;软驱复位
  
  	mov	word [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_BEGIN:
	cmp word[wRootDirSizeForLoop],0
	jz	LABEL_NO_LOADERBIN		; ┣ 判断根目录区是不是已经读完
	dec	word [wRootDirSizeForLoop]	; ┛ 如果读完表示没有找到 LOADER.BIN
	mov	ax, BaseOfLoader
	mov	es, ax			; es <- BaseOfLoader
	mov	bx, OffsetOfLoader	; bx <- OffsetOfLoader	于是, es:bx = BaseOfLoader:OffsetOfLoader
	mov	ax, [wSectorNo]	; ax <- Root Directory 中的某 Sector 号
	mov	cl, 1
  	call ReadSector
  	
  	mov si,LoaderFileName
  	mov di,OffsetOfLoader
  	cld
  	mov dx,10h
LABEL_SEARCH_FOR_LOADER:
	cmp dx,0
	jz  LABEL_GOTO_NEXT_SECTOR
	dec dx
	mov cx,11
LABEL_CMP_FILENAME:
	cmp cx,0
	jz  LABEL_FILENAME_FOUND
	dec cx
	lodsb
	cmp al,byte[es:di]
	jz  LABEL_GO_ON
	jmp LABEL_DIFFERENT
	
LABEL_GO_ON:
	inc di
	jmp LABEL_CMP_FILENAME
LABEL_DIFFERENT:
	and di, 0ffe0h
	add di,20h
	mov si,LoaderFileName
	jmp LABEL_SEARCH_FOR_LOADER
	
LABEL_GOTO_NEXT_SECTOR:
	add	word [wSectorNo], 1
	jmp	LABEL_SEARCH_BEGIN
LABEL_NO_LOADERBIN:
    mov si,Message2
    call print
    call echo
LABEL_FILENAME_FOUND:
	mov ax,RootDirSectors
	and di,0ffe0h
	add di,01ah
	mov cx,word [es:di]
	push cx
	add cx,ax
	add cx,DeltaSectorNo
	mov ax,BaseOfLoader
	mov es,ax
	mov bx,OffsetOfLoader
	mov ax,cx

LABEL_GOON_LOADING_FILE:
	mov cl,1
	call ReadSector
	pop ax	
	call GetFATEntry
	cmp ax,0fffh
	jz LABEL_FILE_LOADED
	push ax
	mov dx,RootDirSectors
	add ax,dx
	add ax,DeltaSectorNo
	add bx,[BPB_BytsPerSec]
	jmp LABEL_GOON_LOADING_FILE
	
LABEL_FILE_LOADED:
	mov si,Message1
	call print
	jmp BaseOfLoader:OffsetOfLoader
ReadSector:
	push	bp
	mov	bp, sp
	sub	esp, 2			; 辟出两个字节的堆栈区域保存要读的扇区数: byte [bp-2]

	mov	byte [bp-2], cl
	push	bx			; 保存 bx
	mov	bl, [BPB_SecPerTrk]	; bl: 除数
	div	bl			; y 在 al 中, z 在 ah 中
	inc	ah			; z ++
	mov	cl, ah			; cl <- 起始扇区号
	mov	dh, al			; dh <- y
	shr	al, 1			; y >> 1 (其实是 y/BPB_NumHeads, 这里BPB_NumHeads=2)
	mov	ch, al			; ch <- 柱面号
	and	dh, 1			; dh & 1 = 磁头号
	pop	bx			; 恢复 bx
	; 至此, "柱面号, 起始扇区, 磁头号" 全部得到 ^^^^^^^^^^^^^^^^^^^^^^^^
	mov	dl, [BS_DrvNum]		; 驱动器号 (0 表示 A 盘)
.GoOnReading:
	mov	ah, 2			; 读
	mov	al, byte [bp-2]		; 读 al 个扇区
	int	13h
	jc	.GoOnReading		; 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止

	add	esp, 2
	pop	bp

	ret
GetFATEntry:
	push	es
	push	bx
	push	ax
	mov	ax, BaseOfLoader; `.
	sub	ax, 0100h	;  | 在 BaseOfLoader 后面留出 4K 空间用于存放 FAT
	mov	es, ax		; /
	pop	ax
	mov	byte [bOdd], 0
	mov	bx, 3
	mul	bx			; dx:ax = ax * 3
	mov	bx, 2
	div	bx			; dx:ax / 2  ==>  ax <- 商, dx <- 余数
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [bOdd], 1
LABEL_EVEN:;偶数
	; 现在 ax 中是 FATEntry 在 FAT 中的偏移量,下面来
	; 计算 FATEntry 在哪个扇区中(FAT占用不止一个扇区)
	xor	dx, dx			
	mov	bx, [BPB_BytsPerSec]
	div	bx ; dx:ax / BPB_BytsPerSec
		   ;  ax <- 商 (FATEntry 所在的扇区相对于 FAT 的扇区号)
		   ;  dx <- 余数 (FATEntry 在扇区内的偏移)。
	push	dx
	mov	bx, 0 ; bx <- 0 于是, es:bx = (BaseOfLoader - 100):00
	add	ax, SectorNoOfFAT1 ; 此句之后的 ax 就是 FATEntry 所在的扇区号
	mov	cl, 2
	call	ReadSector ; 读取 FATEntry 所在的扇区, 一次读两个, 避免在边界
			   ; 发生错误, 因为一个 FATEntry 可能跨越两个扇区
	pop	dx
	add	bx, dx
	mov	ax, [es:bx]
	cmp	byte [bOdd], 1
	jnz	LABEL_EVEN_2
	shr	ax, 4
LABEL_EVEN_2:
	and	ax, 0FFFh

LABEL_GET_FAT_ENRY_OK:

	pop	bx
	pop	es
	ret

times 510-($-$$) db 0   
  dw 0xaa55
