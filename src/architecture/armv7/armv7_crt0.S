// EPOS ARMv7 Program Starter

        .file "armv7_crt0.S"

        // Entry point
        .section .text
        .align  2
        .global _start
        .type   _start, function
_start:
        // Set a temporary Stack Pointer for INIT
        // Main's stack will be allocated by Thread::init()
        ldr     r0, =__mem_top__
        mov     sp, r0

        // Clear the BSS
        eor     r0, r0
        ldr     r1, =__bss_start__
        ldr     r2, =__bss_end__
.L1:    str     r0, [r1]
        add     r1, #4
        cmp     r1, r2
        blt     .L1

        // Call _init
        bl      _init

        // Call main
        .align  2
        .globl  __epos_app_entry
        .type   __epos_app_entry, function
__epos_app_entry:
        bl      main
        push    {r0}         // save main's return value to be used by exit()
        bl      _fini
        bl      _exit
