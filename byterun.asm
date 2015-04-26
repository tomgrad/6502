; testing byterun compression

*= $0600
 
start:
  lda #<logo
  sta $0
  lda #>logo
  sta $1
  lda #$00
  sta $2
  lda #$02
  sta $3

decrunchLoop:
  lda $3
  cmp #$6
  bne moreWork 
  rts
moreWork:
  ldy #0
  lda ($0),y
  cmp #$ff
  bne notCrunched
  iny
  lda ($0),y ; repeat #
  sta $4
  iny
  lda ($0),y ; color
  ldy $4
drawLoop:
  ldx #0
  sta ($2,x)
  jsr nextPixel
  dey
  bne drawLoop
  jsr getNextByte
  jsr getNextByte
  jmp decrunchLoop
notCrunched:
  ldx #0
  sta ($2,x)
  jsr nextPixel
  jsr getNextByte
  jmp decrunchLoop

getNextByte:
  inc $0
  lda $0
  cmp #$00
  bne notHi
  inc $1
notHi:
  rts

nextPixel:
  inc $2
  ldx $2
  cpx #$00
  bne notNextLine
  inc $3
notNextLine:
  rts


logo:
 .byt $ff,43,1,$f,$f,$f,$c,$f,$f,$f,$ff,24,1,$c,$f,$c,0
 .byt $c,$f,$c,$ff,24,1,0,$f,$c,0,$c,$f,$c,$ff,24,1
 .byt $c,$f,$c,0,$c,$f,$c,$ff,24,1,0,$f,$c,0,$c,$f,$c
 .byt $ff,24,1,$c,$f,0,0,$c,$f,$c,$ff,24,1,0,$f,$c,0
 .byt $c,$f,$c,$ff,24,1,0,$f,$c,0,$c,$f,0,$ff,24,1
 .byt 0,$f,$c,0,$c,$f,0,$ff,23,1,$f,0,$f,$c,0,$c,$f,0,$f
 .byt $ff,22,1,$c,0,1,$c,0,$c,$f,0,$c,$ff,21,1
 .byt $f,0,0,1,0,0,$c,1,0,0,$ff,21,1,$c,0,$c,1,$c,0
 .byt $c,1,$c,0,$c,$ff,19,1,$f,0,0,$f,1,$c,0
 .byt $c,1,$f,0,0,$f,$ff,17,1,$f,0,0,0,1,1,$c,0
 .byt $c,1,1,0,0,0,$ff,16,1,$f,0,0,0,$f,1,1,0,0
 .byt $c,1,1,$f,0,0,0,$f,$ff,13,1
 .byt $c,0,0,0,$c,1,1,1,$c,0,$c,1,1,1,$c,0,0,0,$c
 .byt $ff,10,1,$c,0,0,0,0,$c,1,1,1,1,0,0
 .byt $c,1,1,1,1,0,0,0,0,0,$c,$ff,8,1
 .byt 0,0,0,0,$c,1,1,1,1,1,0,0
 .byt $c,1,1,1,1,1,$c,0,0,0,0,1,1,1,1,1
 .byt 1,1,1,1,0,0,$c,1,1,1,1,1,1,1,$c,0
 .byt $c,1,1,1,1,1,1,$f,$c,0,0,$ff,18,1,$f
 .byt $ff,53,1,0,$f,1,0,0,0,0,0,$f,1,$c
 .byt $c,1,1,1,$c,0,0,0,1,1,0,$f,$f,1,1,1
 .byt 1,1,1,1,$c,0,0,1,1,1,0,$f,1,1,$f,0
 .byt 0,$f,1,1,0,$f,1,$c,$c,1,0,$f,1,1,1,1
 .byt 1,1,1,1,0,$f,0,$f,1,1,0,$f,1,1,$f,$c
 .byt $c,$c,1,1,0,1,1,$f,0,1,0,$f,1,1,1,1
 .byt 1,1,1,1,0,1,$c,$f,1,1,$c,$f,1,1,0,$f
 .byt $f,0,1,1,0,$f,$f,0,$f,1,0,$f,1,1,1,1
 .byt 1,1,1,$c,0,$c,0,0,1,1,0,$f,1,1,0,$c
 .byt $c,0,$f,1,0,$f,0,$f,1,1,0,$f,1,1,1,1
 .byt 1,1,1,0,$c,$f,$f,0,$f,1,$c,$f,1,$c,$c,$f
 .byt $f,$c,$c,1,0,1,$f,$c,1,1,0,$f,1,1,1,1
 .byt 1,1,$f,0,1,1,1,$c,$c,1,0,$f,1,0,$f,1
 .byt 1,$f,0,1,0,$f,1,0,$f,1,0,$f,$ff,16,1
 .byt $f,$ff,5,1,$f,1,1,1,$f,$ff,38,1


