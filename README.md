# ARM-Assembly-Program-Simulator

In order to execute the program we should run the following on the terminal 
## g++ Assignment3.cpp
## ./a.out filename.s

#### Memory Size 100 words
#### Stack Size 30 Words
#### sp is initially set to 30
#### All the registers are having null value initially

#### The program will terminate when all the instructions sre executed or then it reaches SWI EXIT

### Our program supports the following instructions.
## KINDLY TAKE CARE OF THE SPACING . IT CAN BE MORE BUT NOT LESS THAN THE ONE SHOWN
## NOTE : when inserting labels like LOOP etc They should immediately be followed by :
## for ex--   LOOP: 

## LDR   ldr r2, [ r3, #c ]
## STR   str r2, [ r3, #c ]
## ADD   add r1, r2, r3  add sp, sp, #0
## SUB   sub r1, r2, r3  sub sp, sp, #2
## MUL   mul r1, r2, r3
## CMP   cmp r1, r2
## BNE   bne label
## BGE   bge label    
## B     b label
## BL    bl label
## MOV   mov r1, r2       mov r1, #4
 
