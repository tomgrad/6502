*= $0600

LDX #$08
decrement:
DEX
STX $0200
CPX #$03
BNE decrement
STX $0201
clc
ldx #$00
lda #$00
petla:
adc #$01
sta $0220,X
inx
cpx #$10
bne petla

clc
ldx #$00
czytaj:
lda dane,X
sta $0240,X
inx
cpx #$03
bne czytaj

BRK
dane:
.byt $66,$77,$88
