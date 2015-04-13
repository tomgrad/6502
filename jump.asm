*= $0600
lda #$01
sta $0200
; jmp label
; brk
;label:
; lda #$66
 jsr podpr
 ldy #$44
 brk
podpr:
 ldx #$77
 rts
