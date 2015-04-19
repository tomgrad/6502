; test keyboard and random number generator
*= $0600

loop:
 lda $fe	; rnd
 sta $0200
 lda $ff	; last key
 sta $0201
 jmp loop
