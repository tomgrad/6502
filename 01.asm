*= $0600

;immediate & absolute
lda #$02
sta $0020
lda #$a0
sta $0021

lda $0021
sta $0220

;zeropage
lda $20
sta $30

;absolute,X
ldx #$20
lda $0200,X
sta $0210,X

;absolute,Y
ldy #$10
lda $0210,Y
sta $0212,Y

;zeropage,X
ldx #$11
lda $10,X
sta $13,X

;zeropage,Y
ldy #$05
lda $2b,Y
sta $60,Y

;indirect
lda #$66
sta $a002
ldx #$01
ldy #$05
lda ($1f,X)

lda #$77
sta $a007
lda ($20),Y

lda #$34
ldx #$1
sta ($63,X)

lda #$99
ldy #$02
sta ($64),Y

;ADC
lda #$80
adc #$ff



