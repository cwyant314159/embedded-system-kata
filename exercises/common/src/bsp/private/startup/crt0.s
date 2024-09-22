;
; vector table entry macro
;
; The handler macro parameter is given weak linkage to the default_vector__
; function. If user applications define a custom handler using the ISR() macro
; provided by <avr/io.h>, that function will be called.
;
.macro vector_table_entry handler
    .WEAK \handler
    .SET  \handler, default_vector__
    jmp   \handler
.endm


;
; interrupt vector table
;
; The vector table must be placed at the beginning of flash so the processor can
; properly jump to the startup routine/reset vector. This detail is handled by
; the linker script assuming the vector table is marked with the appropriate
; section directive.
;
.SECTION    .ld__vector_table_section,"ax",@progbits
.GLOBAL     vector_table__
.FUNC       vector_table__
vector_table__:
    jmp                 startup_asm__   ; reset
    vector_table_entry  __vector_1      ; INT0
    vector_table_entry  __vector_2      ; INT1
    vector_table_entry  __vector_3      ; PCINT0
    vector_table_entry  __vector_4      ; PCINT1
    vector_table_entry  __vector_5      ; PCINT2
    vector_table_entry  __vector_6      ; WDT
    vector_table_entry  __vector_7      ; TIMER2 COMPA
    vector_table_entry  __vector_8      ; TIMER2 COMPB
    vector_table_entry  __vector_9      ; TIMER2 OVF
    vector_table_entry  __vector_10     ; TIMER1 CAPT
    vector_table_entry  __vector_11     ; TIMER1 COMPA
    vector_table_entry  __vector_12     ; TIMER1 COMPB
    vector_table_entry  __vector_13     ; TIMER1 OVF
    vector_table_entry  __vector_14     ; TIMER0 COMPA
    vector_table_entry  __vector_15     ; TIMER0 COMPB
    vector_table_entry  __vector_16     ; TIMER0 OVF
    vector_table_entry  __vector_17     ; SPI, STC
    vector_table_entry  __vector_18     ; USART, RX
    vector_table_entry  __vector_19     ; USART, UDRE
    vector_table_entry  __vector_20     ; USART, TX
    vector_table_entry  __vector_21     ; ADC
    vector_table_entry  __vector_22     ; EE READY
    vector_table_entry  __vector_23     ; ANALOG COMP
    vector_table_entry  __vector_24     ; TWI
    vector_table_entry  __vector_25     ; SPM READY
.ENDFUNC


;
; The default_vector__ is called when the processor attempts to execute an ISR
; that has not been provided a custom handler via the ISR() macro.
;
; The default handler gives the symbol default_isr_handler__ weak linkage to the
; the vector table and then calls the default_isr_handler__. This basically does
; as reset as the first entry in the vector table is the reset handler.
;
; User applications can provide a definition to default_isr_handler__ to perform
; custom actions INSTEAD of resetting.
;
    .text
    .GLOBAL     default_vector__
    .FUNC       default_vector__
default_vector__:
    .WEAK	default_isr_handler__
    .SET	default_isr_handler__, vector_table__
    call	default_isr_handler__
    .ENDFUNC


;
; Reset handler, application entry point, whatever you want to call it. It all
; starts here.
;
    .FUNC   startup_asm__
startup_asm__:

    ;
    ; External symbols needed by this routine
    ;
    .EXTERN startup_c__     ; C startup routine
    .EXTERN ld__init_sp     ; initial stack pointer symbol from linker

    ;
    ; Hardware address constants needed by this routine
    ;
    .EQU SREG, 0x3F         ; processor status register
    .EQU SPH,  0x3E         ; stack pointer high register
    .EQU SPL,  0x3D         ; stack pointer low register

    ;
    ; Before running any C functions a few things MUST be done in assembly:
    ;   1. ensure R1 contains the value 0
    ;   2. clear the SREG hardware register
    ;   3. initialize the stack pointer (high byte first) with the value defined
    ;      in the linker script
    ;
    ; Once these items have been taken care of, we are free to call a C function
    ; to finish initializing the system.

    eor  r1,   r1                ;   r1   = r1 ^ r1 = 0
    out  SREG, r1                ;   SREG = r1 = 0
    ldi  r28,  lo8(ld__init_sp)  ;   r28  = (ld__init_sp >> 0) & 0xFF
    ldi  r29,  hi8(ld__init_sp)  ;   r29  = (ld__init_sp >> 8) & 0xFF
    out  SPH,  r29               ;   SPH  = r29
    out  SPL,  r28               ;   SPL  = r28

    call startup_c__             ;   startup_c__()

.ENDFUNC
