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
  ;����
;----------------------------------------------------------------------------
wRootDirSizeForLoop	dw	RootDirSectors	; Root Directory ռ�õ�������, ��ѭ���л�ݼ�����.
wSectorNo		dw	0		; Ҫ��ȡ��������
bOdd			db	0		; ��������ż��

;============================================================================
;�ַ���
;----------------------------------------------------------------------------
LoaderFileName		db	"LOADER  BIN", 0	; LOADER.BIN ֮�ļ���
; Ϊ�򻯴���, ����ÿ���ַ����ĳ��Ⱦ�Ϊ MessageLength
BootMessage:		db	"Booting  ",13,10,0; 9�ֽ�, �������ÿո���. ��� 0
Message1		db	"Ready.   ",13,10,0; 9�ֽ�, �������ÿո���. ��� 1
Message2		db	"No LOADER",13,10,0; 9�ֽ�, �������ÿո���. ��� 2
;============================================================================

print: 
  lodsb            ;��SI��ַ������һ���ֽڵ�al           
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
  	int 13h    ;������λ
  
  	mov	word [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_BEGIN:
	cmp word[wRootDirSizeForLoop],0
	jz	LABEL_NO_LOADERBIN		; �� �жϸ�Ŀ¼���ǲ����Ѿ�����
	dec	word [wRootDirSizeForLoop]	; �� ��������ʾû���ҵ� LOADER.BIN
	mov	ax, BaseOfLoader
	mov	es, ax			; es <- BaseOfLoader
	mov	bx, OffsetOfLoader	; bx <- OffsetOfLoader	����, es:bx = BaseOfLoader:OffsetOfLoader
	mov	ax, [wSectorNo]	; ax <- Root Directory �е�ĳ Sector ��
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
	sub	esp, 2			; �ٳ������ֽڵĶ�ջ���򱣴�Ҫ����������: byte [bp-2]

	mov	byte [bp-2], cl
	push	bx			; ���� bx
	mov	bl, [BPB_SecPerTrk]	; bl: ����
	div	bl			; y �� al ��, z �� ah ��
	inc	ah			; z ++
	mov	cl, ah			; cl <- ��ʼ������
	mov	dh, al			; dh <- y
	shr	al, 1			; y >> 1 (��ʵ�� y/BPB_NumHeads, ����BPB_NumHeads=2)
	mov	ch, al			; ch <- �����
	and	dh, 1			; dh & 1 = ��ͷ��
	pop	bx			; �ָ� bx
	; ����, "�����, ��ʼ����, ��ͷ��" ȫ���õ� ^^^^^^^^^^^^^^^^^^^^^^^^
	mov	dl, [BS_DrvNum]		; �������� (0 ��ʾ A ��)
.GoOnReading:
	mov	ah, 2			; ��
	mov	al, byte [bp-2]		; �� al ������
	int	13h
	jc	.GoOnReading		; �����ȡ���� CF �ᱻ��Ϊ 1, ��ʱ�Ͳ�ͣ�ض�, ֱ����ȷΪֹ

	add	esp, 2
	pop	bp

	ret
GetFATEntry:
	push	es
	push	bx
	push	ax
	mov	ax, BaseOfLoader; `.
	sub	ax, 0100h	;  | �� BaseOfLoader �������� 4K �ռ����ڴ�� FAT
	mov	es, ax		; /
	pop	ax
	mov	byte [bOdd], 0
	mov	bx, 3
	mul	bx			; dx:ax = ax * 3
	mov	bx, 2
	div	bx			; dx:ax / 2  ==>  ax <- ��, dx <- ����
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [bOdd], 1
LABEL_EVEN:;ż��
	; ���� ax ���� FATEntry �� FAT �е�ƫ����,������
	; ���� FATEntry ���ĸ�������(FATռ�ò�ֹһ������)
	xor	dx, dx			
	mov	bx, [BPB_BytsPerSec]
	div	bx ; dx:ax / BPB_BytsPerSec
		   ;  ax <- �� (FATEntry ���ڵ���������� FAT ��������)
		   ;  dx <- ���� (FATEntry �������ڵ�ƫ��)��
	push	dx
	mov	bx, 0 ; bx <- 0 ����, es:bx = (BaseOfLoader - 100):00
	add	ax, SectorNoOfFAT1 ; �˾�֮��� ax ���� FATEntry ���ڵ�������
	mov	cl, 2
	call	ReadSector ; ��ȡ FATEntry ���ڵ�����, һ�ζ�����, �����ڱ߽�
			   ; ��������, ��Ϊһ�� FATEntry ���ܿ�Խ��������
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
