            .ORIG   x3000
;Print"Enter a name:"
Print       LEA     R0,Prompt
            TRAP    X22
            
;Get name input
;R1:adress of the String
Input       LEA     R1,Name
            LD      R2,NewLine  ;get x-000A
            Not     R2,R2
            ADD     R2,R2,#1
Loop_Input  TRAP    X20
            TRAP    X21
            STR     R0,R1,#0
            ADD     R0,R0,R2
            BRz     EndInput    ;if char is '\n', input has finished
            ADD     R1,R1,#1
            BRnzp   Loop_Input
EndInput    STR     R0,R1,#0    ;change the '\n' at the end of the STRING to x0000

;Iterate over the linked list
;R1:adress of the Node;R5:store the result
Iterate     LDI     R1,Head
            BRz     Exit
            AND     R3,R3,#0    ;use R3 to store the number of target nodes
Loop_iter   JSR     CheckNode
            Add     R5,R5,#0    ;use R5 to store the return value(1-match; 0-not match)
            BRp     PrintNode
Continue    LDR     R1,R1,#0    ;check if the next node is x000
            BRz     Exit
            BRnzp   Loop_iter
;
Exit        ADD     R3,R3,#0
            BRp     ExitAll     ;if R3 == 0, there is no node matched, print "Not found"
            LEA     R0,NF
            TRAP    X22
ExitAll     TRAP    X25

;Check Node to see if the name can match
;R1:adress of the Node;R5:store the result(whether two names are the same);R7:return adress
CheckNode   
            ST      R1,SAVER1
            ST      R3,SAVER3
            ST      R7,SAVER7
            
            AND     R5,R5,#0    ;clear R5, result will be stored in R5
            ADD     R1,R1,#2
            JSR     CheckName
            ADD     R5,R5,#0
            BRp     CNode_exit
            LD      R1,SAVER1
            ADD     R1,R1,#3
            JSR     CheckName
            BRnzp   CNode_exit
;
CNode_exit  LD      R1,SAVER1
            LD      R3,SAVER3
            LD      R7,SAVER7
            RET

;Cheak Name to see if they are the same
;R1:adress of Name in node -> R3; R2:adress of name -> R4
CheckName   
            LEA     R2,Name     
            LDR     R1,R1,#0
LoopCName   LDR     R4,R2,#0
            LDR     R3,R1,#0
            NOT     R4,R4
            ADD     R4,R4,#1
            ADD     R4,R4,R3
            BRnp    Name_Fail   ;two characters are different, so two names are not the same
            ADD     R3,R3,#0
            BRz     Name_Sc
            ADD     R2,R2,#1
            ADD     R1,R1,#1
            BRnzp   LoopCName
;
Name_Fail   RET
Name_Sc     Add     R5,R5,#1
            RET

;Print the Node
;R3-store the number of node matched, R1-the adress of the Node
PrintNode   
            ADD     R3,R3,#0    ;if R3 == 1, there is no need to print '\n'
            BRz     StartPN
            LD      R0,NewLine
            TRAP    X21
StartPN     ADD     R3,R3,#1    ;find the target node, so R3++
            ADD     R0,R1,#2
            LDR     R0,R0,#0
            TRAP    X22
            LD      R0,Space    ;print ' ' to separate strings
            TRAP    X21
            ADD     R0,R1,#3
            LDR     R0,R0,#0
            TRAP    X22
            LD      R0,Space
            TRAP    X21
            ADD     R0,R1,#1
            LDR     R0,R0,#0
            TRAP    X22
            BRnzp   Continue    ;back to the main programm

;Values
SAVER1  .BLKW   1
SAVER3  .BLKW   1
SAVER7  .BLKW   1
NewLine .FILL   X000A   ;the ASCII of '\n' is x000A
Head    .FILL   X4000
Space   .FILL   X20
Prompt  .STRINGZ    "Enter a name: " 
NF      .STRINGZ    "Not found"
Name    .BLKW   17      ;store the input there
        .END