*= $0600

screen = $0200
curpos = $00f0

lda #<ch_a
sta 4
lda #>ch_a
sta 5

lda #<ch_b
sta 6
lda #>ch_b
sta 7

lda #<ch_c
sta 8
lda #>ch_c
sta 9

lda #<ch_d
sta 10
lda #>ch_d
sta 11

lda #<ch_e
sta 12
lda #>ch_e
sta 13

lda #<ch_f
sta 14
lda #>ch_f
sta 15

 ldx #0
 ldy #0

loop:

 clc
 lda #<screen
 adc curpos
 sta 0
 lda #>screen
 sta 1

 lda napis,y
 bmi end
 asl
 tax
 lda 4,x
 sta 2
 lda 5,x
 sta 3
 jsr draw
 iny
 jmp loop

end:
 brk

draw:
 txa
 pha
 tya
 pha
 ldx #5
 ldy #0
npix:
 lda (2),y
 sta (0),y
 iny
 cpy #25
 beq drawend
 
 dex
 bne npix

nrow:
 ldx #5
 lda 0
 clc
 adc #27
 sta 0
 bcc npix
 inc 1
 jmp npix

drawend:
 pla
 tay
 pla
 tax
 lda curpos
 adc #6
 sta curpos
 rts


brk

napis:
.byt 0,1,2,3,$ff

ch_a:
 .byt 0,0,1,0,0
 .byt 0,1,0,1,0
 .byt 1,0,0,0,1
 .byt 1,1,1,1,1
 .byt 1,0,0,0,1

ch_b:
 .byt 1,1,1,1,0
 .byt 1,0,0,0,1
 .byt 1,1,1,1,0
 .byt 1,0,0,0,1
 .byt 1,1,1,1,0

ch_c:
 .byt 0,1,1,1,0
 .byt 1,0,0,0,1
 .byt 1,0,0,0,0
 .byt 1,0,0,0,1
 .byt 0,1,1,1,0

ch_d:
 .byt 1,1,1,1,0
 .byt 1,0,0,0,1
 .byt 1,0,0,0,1
 .byt 1,0,0,0,1
 .byt 1,1,1,1,0
 
ch_e:
 .byt 1,1,1,1,1
 .byt 1,0,0,0,0
 .byt 1,1,1,1,1
 .byt 1,0,0,0,0
 .byt 1,1,1,1,1
 
ch_f:
 .byt 1,1,1,1,1
 .byt 1,0,0,0,0
 .byt 1,1,1,1,1
 .byt 1,0,0,0,0
 .byt 1,0,0,0,0
 
ch_g:
 .byt 0,1,1,1,0
 .byt 1,0,0,0,0
 .byt 1,0,0,1,1
 .byt 1,0,0,0,1
 .byt 0,1,1,1,0

ch_t:
 .byt 1,1,1,1,1
 .byt 0,0,1,0,0
 .byt 0,0,1,0,0
 .byt 0,0,1,0,0
 .byt 0,0,1,0,0
 
ch_m:
 .byt 1,0,0,0,1
 .byt 1,1,0,1,1
 .byt 1,0,1,0,1
 .byt 1,0,0,0,1
 .byt 1,0,0,0,1
