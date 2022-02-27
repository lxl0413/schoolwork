        .ORIG   X3000
        AND     R0,R0,#0    ;intialize memdist array
        LEA     R1,MemDist
        AND     R2,R2,#0
MemSet  LD      R3,MAXN
        ADD     R3,R3,R2
        BRz     START
        STR     R0,R1,#0
        ADD     R2,R2,#1
        ADD     R1,R1,#1
        BRnzp   MemSet
        
START   LD      R6, USER_SP ;initialze sp
        ADD     R5,R6,#0    ;initialize control link
        
start        AND     R3,R3,#0
        AND     R4,R4,#0
        AND     R1,R1,#0
CHECK_Loop
XM      LDI     R0,M        ;R1 <- -(M-1)
        NOT     R0,R0
        ADD     R0,R0,#2
        ADD     R0,R0,R3
        BRnz    YN
        AND     R3,R3,#0
        ADD     R4,R4,#1
YN      LDI     R0,N        ;R1 <- -(N-1)
        NOT     R0,R0
        ADD     R0,R0,#2
        ADD     R0,R0,R4
        BRp     TERMINATE
        
        ADD     R6,R6,#-3   ;push arguments and return value onto the stack
        STR     R1,R6,#2
        STR     R4,R6,#1
        STR     R3,R6,#0
        
        JSR     CCL_DiST    ;Call and get the return value
        LDR     R2,R6,#0
        LDR     R3,R6,#1
        LDR     R4,R6,#2
        LDR     R1,R6,#3
        ADD     R6,R6,#4

        LD      R0,MaxDist  ;Compare distance returned to current max distance
        NOT     R0,R0
        ADD     R0,R0,#1
        ADD     R0,R0,R2
        BRnz    Next_Loop
        ST      R2,MaxDist
        
Next_Loop
        ADD     R3,R3,#1
        ADD     R1,R1,#1
        BRnzp   CHECK_Loop
    
TERMINATE
        LD      R2,MaxDist
        HALT
        
USER_SP         .FILL   XFE00  
MaxDist         .FILL   X0000
MAXN            .FILL   #-50

N               .FILL   x4000
M               .FILL   x4001
Head            .FILL   x4002
MemDist         .BLKW   #50

CCL_Dist    ;Caculate distance 
        ADD     R6,R6,#-1   ;leave space for return value
        ADD     R6,R6,#-1   ;push return adress onto the stack
        STR     R7,R6,#0
        ADD     R6,R6,#-1   ;push dyn link
        STR     R5,R6,#0
        ADD     R5,R6,#-1   ;set new frame pointer
        ADD     R6,R6,#-1   ;allocate space for local
        
        ;callee gets arguments from caller
        LDR     R3,R5,#4
        LDR     R4,R5,#5
        LDR     R1,R5,#6
        
   ;initialize tmp
        AND     R0,R0,#0    ;R2 <- max distance to this point
        ADD     R0,R0,#1
        ;caller saved tmp
        STR     R0,R5,#0
        
        LEA     R0,MemDist  ;check the memdist, if it is not 0, then return it
        ADD     R0,R0,R1
        LDR     R0,R0,#0
        BRnz    CHECK_NORTH
        STR     R0,R5,#0
        BRnzp   EXIT_CD
       
CHECK_NORTH
        ADD     R4,R4,#0
        BRz     CHECK_EAST  ;the point is at the first row, can't go north
        
        LDI     R0,M        ;check if M[y-1][x] < M[y][x]
        NOT     R0,R0
        ADD     R0,R0,#1    ;R0 <- -M
        
        LD      R7,Head     ;get next al
        ADD     R7,R7,R1
        ADD     R7,R7,R0
        LDR     R7,R7,#0    ;R7 <- M[y-1][x]
        
        NOT     R7,R7
        ADD     R7,R7,#1
        
        LD      R2,Head     ;get cur al
        ADD     R2,R2,R1
        LDR     R2,R2,#0
        
        ADD     R7,R7,R2    ;compare als
        BRnz    CHECK_EAST
        
CALL1   ADD     R4,R4,#-1   ;call next subroutine
        ADD     R1,R1,R0
        
        ADD     R6,R6,#-3   ;push arguments onto the stack
        STR     R1,R6,#2
        STR     R4,R6,#1
        STR     R3,R6,#0
        JSR     CCL_Dist
        
        LDR     R0,R6,#0    ;R0 <- return value (result of DFS)
        ADD     R0,R0,#1
        ADD     R6,R6,#4
        
        LDR     R3,R5,#4
        LDR     R4,R5,#5
        LDR     R1,R5,#6
        
        LDR     R2,R5,#0    ;compare current max value to return value
        NOT     R2,R2
        ADD     R2,R2,#1    ;R2 <- -R2
        ADD     R2,R2,R0
        BRnz    CHECK_EAST
        STR     R0,R5,#0
        
CHECK_EAST
        LDI     R0,M
        NOT     R0,R0
        ADD     R0,R0,#2
        ADD     R0,R0,R3
        BRz     CHECK_SOUTH  ;x == M-1, can't go east

        LD      R7,Head     ;get next al
        ADD     R7,R7,R1
        ADD     R7,R7,#1
        LDR     R7,R7,#0
        
        NOT     R7,R7
        ADD     R7,R7,#1
        
        LD      R2,Head     ;get cur al
        ADD     R2,R2,R1
        LDR     R2,R2,#0
        
        ADD     R7,R7,R2    ;compare als
        BRnz    CHECK_SOUTH
        
CALL2   ADD     R3,R3,#1
        ADD     R1,R1,#1
        
        ADD     R6,R6,#-3   ;push arguments and return value onto the stack
        STR     R1,R6,#2
        STR     R4,R6,#1
        STR     R3,R6,#0
        JSR     CCL_Dist
        
        LDR     R0,R6,#0    ;R0 <- return value (result of DFS)
        ADD     R0,R0,#1
        ADD     R6,R6,#4
        
        LDR     R3,R5,#4
        LDR     R4,R5,#5
        LDR     R1,R5,#6
        
        LDR     R2,R5,#0    ;compare current max value to return value
        NOT     R2,R2
        ADD     R2,R2,#1    ;R2 <- -R2
        ADD     R2,R2,R0
        BRnz    CHECK_SOUTH
        STR     R0,R5,#0
        
CHECK_SOUTH
        LDI     R0,N
        NOT     R0,R0
        ADD     R0,R0,#2
        ADD     R0,R0,R4
        BRz     CHECK_WEST  ;the point is at the last row, can't go south
    
        LDI     R0,M        ;check if M[y+1][x] < M[y][x]
        
        LD      R7,Head
        ADD     R7,R7,R1
        ADD     R7,R7,R0
        LDR     R7,R7,#0    ;R7 <- M[y+1][x]
        
        NOT     R7,R7
        ADD     R7,R7,#1
        
        LD      R2,Head
        ADD     R2,R2,R1
        LDR     R2,R2,#0
        
        ADD     R7,R7,R2    ;compare als
        BRnz    CHECK_WEST
        
CALL3   ADD     R4,R4,#1    ;call next subroutine
        ADD     R1,R1,R0
        
        ADD     R6,R6,#-3   ;push arguments and return value onto the stack
        STR     R1,R6,#2
        STR     R4,R6,#1
        STR     R3,R6,#0
        JSR     CCL_Dist

        LDR     R0,R6,#0    ;R0 <- return value (result of DFS)
        ADD     R0,R0,#1
        ADD     R6,R6,#4
        
        LDR     R3,R5,#4
        LDR     R4,R5,#5
        LDR     R1,R5,#6
        
        LDR     R2,R5,#0    ;compare current max value to return value
        NOT     R2,R2
        ADD     R2,R2,#1    ;R2 <- -R2
        ADD     R2,R2,R0
        BRnz    CHECK_WEST
        STR     R0,R5,#0
        
CHECK_WEST
        ADD     R3,R3,#0
        BRz     Exit_CD     ;x == 0, can't go west
        
        LD      R7,Head     ;get next al
        ADD     R7,R7,R1
        ADD     R7,R7,#-1
        LDR     R7,R7,#0    ;R7 <- M[y][x-1]
        
        NOT     R7,R7
        ADD     R7,R7,#1
        
        LD      R2,Head     ;get cur al
        ADD     R2,R2,R1
        LDR     R2,R2,#0
        
        ADD     R7,R7,R2    ;compare als
        BRnz    EXIT_CD
        
CALL4   ADD     R3,R3,#-1   ;call next subroutine
        ADD     R1,R1,#-1
        
        ADD     R6,R6,#-3   ;push arguments and return value onto the stack
        STR     R1,R6,#2
        STR     R4,R6,#1
        STR     R3,R6,#0
        JSR     CCL_Dist
        
        LDR     R0,R6,#0    ;R0 <- return value (result of DFS)
        ADD     R0,R0,#1
        ADD     R6,R6,#4
        
        LDR     R3,R5,#4
        LDR     R4,R5,#5
        LDR     R1,R5,#6
        
        LDR     R2,R5,#0    ;compare current max value to return value
        NOT     R2,R2
        ADD     R2,R2,#1    ;R2 <- -R2
        ADD     R2,R2,R0
        BRnz    EXIT_CD
        STR     R0,R5,#0

EXIT_CD LEA     R0,MemDist  ;store the result
        ADD     R0,R0,R1
        LDR     R2,R5,#0
        STR     R2,R0,#0
        STR     R2,R5,#3
        
        ADD     R6,R5,#1    ;pop local variable
        
        LDR     R5,R6,#0    ;pop dyn link
        ADD     R6,R6,#1
        
        LDR     R7,R6,#0
        ADD     R6,R6,#1    ;pop return address
        RET
        .END