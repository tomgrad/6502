*= $0600

ldx #$5
ldy #$20

decrement:
DEX
bne decrement

dey
de2:
bne loop
brk

loop:
dey
jmp de2
