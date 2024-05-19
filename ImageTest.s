.LC0:
        .string "IDAT \354\262\255\355\201\254\353\245\274 \354\260\276\354\235\204 \354\210\230 \354\227\206\354\212\265\353\213\210\353\213\244."
FindIDATChunk(_IO_FILE*):
        stp     x29, x30, [sp, -48]!
        mov     x29, sp
        str     x0, [sp, 24]
        mov     w0, 2
        str     w0, [sp, 40]
        str     wzr, [sp, 44]
        b       .L2
.L6:
        add     x0, sp, 32
        ldr     x3, [sp, 24]
        mov     x2, 2
        mov     x1, 1
        bl      fread
        ldrb    w0, [sp, 32]
        cmp     w0, 73
        bne     .L3
        ldrb    w0, [sp, 33]
        cmp     w0, 68
        bne     .L3
        add     x0, sp, 32
        ldr     x3, [sp, 24]
        mov     x2, 2
        mov     x1, 1
        bl      fread
        ldrb    w0, [sp, 32]
        cmp     w0, 65
        bne     .L3
        ldrb    w0, [sp, 33]
        cmp     w0, 84
        bne     .L3
        ldr     w0, [sp, 44]
        lsl     w0, w0, 1
        add     w0, w0, 5
        b       .L7
.L3:
        ldrb    w0, [sp, 33]
        cmp     w0, 73
        bne     .L5
        add     x0, sp, 32
        ldr     x3, [sp, 24]
        mov     x2, 2
        mov     x1, 1
        bl      fread
        ldrb    w0, [sp, 32]
        cmp     w0, 68
        bne     .L5
        ldrb    w0, [sp, 33]
        cmp     w0, 65
        bne     .L5
        add     x0, sp, 32
        ldr     x3, [sp, 24]
        mov     x2, 2
        mov     x1, 1
        bl      fread
        ldrb    w0, [sp, 32]
        cmp     w0, 84
        bne     .L5
        ldr     w0, [sp, 44]
        add     w0, w0, 3
        lsl     w0, w0, 1
        b       .L7
.L5:
        ldr     w0, [sp, 44]
        add     w0, w0, 1
        str     w0, [sp, 44]
.L2:
        ldr     w0, [sp, 44]
        cmp     w0, 50
        ble     .L6
        adrp    x0, .LC0
        add     x1, x0, :lo12:.LC0
        adrp    x0, _ZSt4cout
        add     x0, x0, :lo12:_ZSt4cout
        bl      std::basic_ostream<char, std::char_traits<char> >& std::operator<< <std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&, char const*)
        mov     w0, -1
.L7:
        ldp     x29, x30, [sp], 48
        ret
.LC1:
        .string "rb"
.LC2:
        .string "2024S_MA_image_rgba.png"
.LC3:
        .string "wb"
.LC4:
        .string "output_rgb.png"
main:
        stp     x29, x30, [sp, -80]!
        mov     x29, sp
        mov     w0, 4
        str     w0, [sp, 68]
        mov     w0, 10000
        str     w0, [sp, 64]
        adrp    x0, .LC1
        add     x1, x0, :lo12:.LC1
        adrp    x0, .LC2
        add     x0, x0, :lo12:.LC2
        bl      fopen
        str     x0, [sp, 56]
        adrp    x0, .LC3
        add     x1, x0, :lo12:.LC3
        adrp    x0, .LC4
        add     x0, x0, :lo12:.LC4
        bl      fopen
        str     x0, [sp, 48]
        ldr     x0, [sp, 56]
        bl      FindIDATChunk(_IO_FILE*)
        str     w0, [sp, 44]
        adrp    x0, .LC1
        add     x1, x0, :lo12:.LC1
        adrp    x0, .LC2
        add     x0, x0, :lo12:.LC2
        bl      fopen
        str     x0, [sp, 56]
        str     wzr, [sp, 76]
        b       .L9
.L10:
        add     x0, sp, 16
        ldr     x3, [sp, 56]
        mov     x2, 1
        mov     x1, 1
        bl      fread
        add     x0, sp, 16
        ldr     x3, [sp, 48]
        mov     x2, 1
        mov     x1, 1
        bl      fwrite
        ldr     w0, [sp, 76]
        add     w0, w0, 1
        str     w0, [sp, 76]
.L9:
        ldr     w0, [sp, 44]
        sub     w0, w0, #1
        ldr     w1, [sp, 76]
        cmp     w1, w0
        blt     .L10
        str     wzr, [sp, 72]
        b       .L11
.L14:
        add     x0, sp, 16
        ldr     x3, [sp, 56]
        mov     x2, 4
        mov     x1, 1
        bl      fread
        ldrb    w0, [sp, 16]
        str     w0, [sp, 40]
        ldrb    w0, [sp, 17]
        str     w0, [sp, 36]
        ldrb    w0, [sp, 18]
        str     w0, [sp, 32]
        ldrb    w0, [sp, 19]
        str     w0, [sp, 28]
        ldr     w1, [sp, 40]
        ldr     w0, [sp, 36]
        add     w1, w1, w0
        ldr     w0, [sp, 32]
        add     w0, w1, w0
        mov     w1, 21846
        movk    w1, 0x5555, lsl 16
        smull   x1, w0, w1
        lsr     x1, x1, 32
        asr     w0, w0, 31
        sub     w0, w1, w0
        str     w0, [sp, 24]
        ldr     w0, [sp, 24]
        cmp     w0, 127
        ble     .L12
        mov     w0, 1
        strb    w0, [sp, 16]
        b       .L13
.L12:
        strb    wzr, [sp, 16]
.L13:
        add     x0, sp, 16
        ldr     x3, [sp, 48]
        mov     x2, 1
        mov     x1, 1
        bl      fwrite
        ldr     w0, [sp, 72]
        add     w0, w0, 1
        str     w0, [sp, 72]
.L11:
        ldr     w1, [sp, 72]
        mov     w0, 9999
        cmp     w1, w0
        ble     .L14
        ldr     x0, [sp, 48]
        bl      fclose
        ldr     x0, [sp, 56]
        bl      fclose
        mov     w0, 0
        ldp     x29, x30, [sp], 80
        ret