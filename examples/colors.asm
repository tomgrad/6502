screen = $0200

*= $0600

 ldx #0
 ldy #0
 ;init screen
 lda #<screen
 sta $0
 lda #>screen
 sta $1
 
loop:
 lda colors,x
 bpl ok
 inc $0
 ldx #0
 lda colors,x
ok:
 inx
 sta ($0),y
 iny
 bne ok2
 inc $1
ok2:
 jmp loop

colors:
 .byt 0,2,0,2,2,8,2,8,8,7,8,7,7,1,7,1,1,7,1,7,7,8,7,8,8,2,8,2,2,0,2,0
 .byt 2,2,8,2,8,8,7,8,7,7,1,7,1,1,1,1,1,1,1,1,7,1,7,7,8,7,8,8,2,8,2,2,$ff

