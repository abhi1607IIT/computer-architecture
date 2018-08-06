.equ Printstr,0x69
.equ READ,0x6c
.equ Stdout,1
.equ PrintInt,0x6b
.equ EXIT,0x11
.equ Opening,0x66
.equ Closing,0x68
.text


ldr r0,=InFileName                    @  input file name
mov r1,#0                             @ mode
swi Opening                          @ open file
bcs InFileError                       @ for error!!
ldr r1,=InFileHandle                  @  file handle
str r0,[r1]                           @ saving

ldr r0,=InFileHandle
ldr r0,[r0]
swi 0x6c
@ bcs ReadError


    MOV r10,r0                         @ starting state
    MOV r11, r10                        @ lfsr
    MOV r12, #5                        @ loop counter
MAINFUNC:
    
 
    MOV r2,r11 ,LSR #0
    MOV r3,r11,LSR #2
    MOV r4,r11,LSR #3
    MOV r5,r11,LSR #5
    EOR r2,r2,r3
    EOR r4,r4,r5
    EOR r4,r4,r2
    AND r4,r4,#1
    MOV r9,r11,LSR #1
    MOV r4,r4,LSl #15
    ORR r11,r4,r9
    CMP r11,r10
    BNE SHIFTLOOP
    B Exit
    
SHIFTLOOP: 
    
    CMP r12,#0
    BNE Help   
    B MAINFUNC   

Help:
    ldr r0,=OutFileName               @  output file name
    mov r1,#2                             @ mode
    swi Opening                          @ open file
    bcs OutFileError                      @ for error!!
    ldr r1,=OutFileHandle                 @ file handle
    str r0,[r1]                           @ saving

    ldr r0,=OutFileHandle
    ldr r0,[r0]
    mov r1,r11
    swi PrintInt 
    @mov r0,#Stdout
    ldr r0,=OutFileHandle
    ldr r0,[r0]
    ldr r1,=NL
    swi Printstr
    
    ldr r0,=OutFileHandle
    ldr r0,[r0]
    swi Closing
    sub r12,r12,#1
    b MAINFUNC 


Exit:
swi EXIT






InFileError: .asciz "Unable to open input file\n"
    .align
OutFileError:.asciz "Unable to open output file\n"
    .align

.data
.align
InFileHandle:.word 0
.align
OutFileHandle:.word 0
NL: .asciz "   "
OutFileName: .asciz "output.txt"
InFileName: .asciz "input.txt"



