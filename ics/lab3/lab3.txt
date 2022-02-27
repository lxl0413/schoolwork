        .ORIG   X0200
        LD      R6, OS_SP
        LD      R0, USER_PSR
        ADD     R6, R6, #-1
        STR     R0, R6, #0
        LD      R0, USER_PC
        ADD     R6, R6, #-1
        STR     R0, R6, #0
        
        ;Set the INTV table
        LD      R0,KB_IN
        STI     R0,IN_AD
        
        ;Set KBSR[14] 1 to enable interrupt
        LD  R0,KBSR_IN
        STI R0,KB_SR
        
        RTI
 
OS_SP           .FILL x3000
USER_PSR        .FILL x8002
USER_PC         .FILL x3000

KB_IN           .FILL x0800
IN_AD           .FILL x0180
KB_SR           .FILL xFE00
KBSR_IN         .FILL x4000     
        .END


;R2 and R5 are used in interrupt service and user program.  
;R0:for out put; R2:count; R3:store the input
        .ORIG   x0800
        ;Save R0,R1,R3 onto the stack
        ADD     R6,R6,#-1
        STR     R0,R6,#0
        ADD     R6,R6,#-1
        STR     R1,R6,#0
        ADD     R6,R6,#-1
        STR     R3,R6,#0

        LDI     R0,KBDR
        ADD     R3,R0,#0    ;use R3 to store the input        
        
        LD      R0,NL       ;print a newline
        OUT
        
        AND     R2,R2,#0    ;clear R2
        
        LD      R1,NL       ;check if input == ENTER        
        NOT     R1,R1
        ADD     R1,R1,#1
        ADD     R1,R1,R3
        BRz     c_ENTER
        
        LD      R1,ASCII_0  ;check if input == number
        ADD     R1,R1,R3
        BRn     c_OTHER
        LD      R1,ASCII_9
        ADD     R1,R1,R3
        BRp     c_OTHER
        BRnz    c_NUMBER
        
c_ENTER
        ADD     R5,R5,#0
        BRz     E_EXIT
        ADD     R5,R5,#-1
E_EXIT  BRnzp   EXIT

c_NUMBER
        AND     R5,R5,#0
        ADD     R5,R5,R3
        LD      R3,ASC      ;convert ASCII CODE TO NUMBER
        NOT     R3,R3
        ADD     R3,R3,#1
        ADD     R5,R5,R3
        BRnzp   EXIT
        
c_OTHER
        ADD     R0,R3,#0
COUNT   OUT
        ADD     R2,R2,#1
        LD      R1,Cy
        ADD     R1,R1,R2
        BRz     O_EXIT
        BRnp    COUNT
O_EXIT  LD      R0,NL
        OUT
        AND     R2,R2,#0
        BRnzp   EXIT
        
EXIT    LDR     R3,R6,#0
        ADD     R6,R6,#1
        LDR     R1,R6,#0
        ADD     R6,R6,#1
        LDR     R0,R6,#0
        ADD     R6,R6,#1
        
        RTI

PSR             .FILL XFFFC
DSR             .FILL XFE04
DDR             .FILL XFE06

KBSR            .FILL xFE00
KBDR            .FILL XFE02

Enter           .FILL X-D
NL              .FILL X000A
ASCII_0         .FILL X-30
ASCII_9         .FILL X-57

Cy              .FILL X-0028
ASC             .FILL X0030
        .END
        
        
        
        .ORIG   X3000
        AND     R2,R2,#0    ;clear R2
        AND     R5,R5,#0
        ADD     R5,R5,#7    ;initialize R5 as 7  
        
printTC
        LD      R0,ASCII    ;use R0 to convert number to ASCII char and do output
        ADD     R0,R0,R5    ;load R0 with the Task Counter
        OUT
        ADD     R2,R2,#1
        LD      R3,Cycle    ;if R2 != 40, if not, print Tasckcounter and continue
        ADD     R3,R3,R2
        BRnp    Next
        LD      R0,NewLine  ;if R2 == 40, print '\n' and clear R2
        OUT
        AND     R2,R2,#0    ;clear R2, start another cycle
Next    JSR     DELAY
        BRnzp   printTC
        
        
DELAY   ST      R1, DELAY_R1
        LD      R1, DELAY_COUNT
DELAY_LOOP  
        ADD     R1, R1, #-1
        BRnp    DELAY_LOOP
        LD      R1, DELAY_R1
        RET
        
;Values
DELAY_COUNT     .FILL #2000
DELAY_R1        .BLKW #1

NewLine         .FILL X000A
Cycle           .FILL X-0028
ASCII           .FILL X0030     ;convert number to ASCII char  
Stack           .FILL X3000
        .END