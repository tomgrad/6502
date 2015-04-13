*= $0600

LDA #$01
STA $20
LDA #$02
STA $21
LDA #$03
STA $22
LDA #$04
STA $23

; $20,$21 + $22,$23 = $24,$25

clc
lda $20
adc $22
sta $24
lda $21
adc $23
sta $25


LDA #$06
STA $40
LDA #$04
STA $41
LDA #$05
STA $42
LDA #$03
STA $43

; $40,$41 i $42,$43 = $44,$45

sec
lda $40
sbc $42
sta $44
lda $41
sbc $43
sta $45


