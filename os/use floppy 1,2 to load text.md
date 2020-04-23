# use floppy 1,2 to load text

boot.asm

```
org  0x8000;


entry:
    mov  ax, 0
    mov  ss, ax
    mov  ds, ax
    mov  es, ax
    mov  si, msg

putloop:
    mov  al, [si]
    add  si, 1
    cmp  al, 0
    je   fin
    mov  ah, 0x0e
    mov  bx, 15
    int  0x10
    jmp  putloop

fin:
    HLT
    jmp  fin

msg:
    DB    0x0a,  0x0a
    db    "hello, world form 0x8000"
    db    0x0a
    db    0
```



kernel.asm

```assembly
org  0x7c00;


entry:
    mov  ax, 0
    mov  ss, ax
    mov  ds, ax
    mov  es, ax
    mov  si, msg


readFloppy:
    mov          CH, 1        ;CH 用来存储柱面号
    mov          DH, 0        ;DH 用来存储磁头号
    mov          CL, 2        ;CL 用来存储扇区号

    mov          BX, msg       ; ES:BX 数据存储缓冲区

    mov          AH, 0x02      ;  AH = 02 表示要做的是读盘操作
    mov          AL,  1        ; AL 表示要练习读取几个扇区
    mov          DL, 0         ;驱动器编号，一般我们只有一个软盘驱动器，所以写死   
                               ;为0
    INT          0x13          ;调用BIOS中断实现磁盘读取功能

    jc           error

putloop:
    mov  al, [si]
    add  si, 1
    cmp  al, 0
    je   fin
    mov  ah, 0x0e
    mov  bx, 15
    int  0x10
    jmp  putloop



fin:
    HLT
    jmp  fin

error:
    mov si, errmsg
    jmp   putloop

msg:
    RESB   64
errmsg:
    DB "error"

```

