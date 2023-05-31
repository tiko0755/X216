;/**************************************************************************//**
; * @file     startup_gr55xx.s
; * @brief    CMSIS Cortex-M ARMv7-M based Core Device Startup File for
; *           Device GR55xx
; * @version  V1.00
; * @date     12. June 2018
; *
; * @note
; *
; ******************************************************************************/
;/* Copyright (c) 2016-2018, Shenzhen Huiding Technology Co., Ltd
;
;   All rights reserved.
;   Redistribution and use in source and binary forms, with or without
;   modification, are permitted provided that the following conditions are met:
;   - Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
;   - Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
;   - Neither the name of ARM nor the names of its contributors may be used
;     to endorse or promote products derived from this software without
;     specific prior written permission.
;   *
;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
;   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
;   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
;   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
;   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
;   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
;   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
;   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
;   POSSIBILITY OF SUCH DAMAGE.
;   ---------------------------------------------------------------------------*/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/
                IF :DEF:NO_SCATTER

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00008000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000400

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                ENDIF   ; NO_SCATTER


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size
                IMPORT  SysTick_Handler
                IMPORT  SVC_Handler
                IMPORT  BLE_IRQHandler
                IMPORT  BLESLP_IRQHandler
                IMPORT  BLE_SDK_Handler
                IF :DEF:NO_SCATTER

__Vectors       DCD     __initial_sp              ; Top of Stack

                ELSE

                IMPORT  |Image$$ARM_LIB_STACKHEAP$$ZI$$Limit|

__Vectors       DCD     |Image$$ARM_LIB_STACKHEAP$$ZI$$Limit| ; Top of Stack

                ENDIF

                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler
                DCD     BLE_SDK_Handler
                DCD     BLE_IRQHandler
                DCD     DMA_IRQHandler
                DCD     SPI_M_IRQHandler
                DCD     SPI_S_IRQHandler
                DCD     EXT0_IRQHandler
                DCD     EXT1_IRQHandler
                DCD     TIMER0_IRQHandler
                DCD     TIMER1_IRQHandler
                DCD     DUAL_TIMER_IRQHandler
                DCD     QSPI0_IRQHandler
                DCD     UART0_IRQHandler
                DCD     UART1_IRQHandler
                DCD     I2C0_IRQHandler
                DCD     I2C1_IRQHandler
                DCD     AES_IRQHandler
                DCD     HMAC_IRQHandler
                DCD     EXT2_IRQHandler
                DCD     RNG_IRQHandler
                DCD     PMU_IRQHandler
                DCD     PKC_IRQHandler
                DCD     XQSPI_IRQHandler
                DCD     QSPI1_IRQHandler
                DCD     PWR_CMD_IRQHandler
                DCD     BLESLP_IRQHandler
                DCD     SLPTIMER_IRQHandler
                DCD     COMP_IRQHandler
                DCD     AON_WDT_IRQHandler
                DCD     I2S_M_IRQHandler
                DCD     I2S_S_IRQHandler
                DCD     ISO7816_IRQHandler
                DCD     PRESENT_IRQHandler
                DCD     CALENDAR_IRQHandler

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  set_msp
                IMPORT  main_init
                LDR     R0, =set_msp
                BLX     R0
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =main_init
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  Default_Handler           [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  SPI_M_IRQHandler          [WEAK]
                EXPORT  SPI_S_IRQHandler          [WEAK]
                EXPORT  EXT0_IRQHandler           [WEAK]
                EXPORT  EXT1_IRQHandler           [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  DUAL_TIMER_IRQHandler     [WEAK]
                EXPORT  QSPI0_IRQHandler          [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  AES_IRQHandler            [WEAK]
                EXPORT  HMAC_IRQHandler           [WEAK]
                EXPORT  EXT2_IRQHandler           [WEAK]
                EXPORT  RNG_IRQHandler            [WEAK]
                EXPORT  PMU_IRQHandler            [WEAK]
                EXPORT  PKC_IRQHandler            [WEAK]
                EXPORT  XQSPI_IRQHandler          [WEAK]
                EXPORT  QSPI1_IRQHandler          [WEAK]
                EXPORT  PWR_CMD_IRQHandler        [WEAK]
                EXPORT  SLPTIMER_IRQHandler       [WEAK]
                EXPORT  COMP_IRQHandler           [WEAK]
                EXPORT  AON_WDT_IRQHandler        [WEAK]
                EXPORT  I2S_M_IRQHandler          [WEAK]
                EXPORT  I2S_S_IRQHandler          [WEAK]
                EXPORT  ISO7816_IRQHandler        [WEAK]
                EXPORT  PRESENT_IRQHandler        [WEAK]
                EXPORT  CALENDAR_IRQHandler       [WEAK]


WDT_IRQHandler
DMA_IRQHandler
SPI_M_IRQHandler
SPI_S_IRQHandler
EXT0_IRQHandler
EXT1_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
DUAL_TIMER_IRQHandler
QSPI0_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
AES_IRQHandler
HMAC_IRQHandler
EXT2_IRQHandler
RNG_IRQHandler
PMU_IRQHandler
PKC_IRQHandler
XQSPI_IRQHandler
QSPI1_IRQHandler
PWR_CMD_IRQHandler
SLPTIMER_IRQHandler
COMP_IRQHandler
AON_WDT_IRQHandler
I2S_M_IRQHandler
I2S_S_IRQHandler
ISO7816_IRQHandler
PRESENT_IRQHandler
CALENDAR_IRQHandler

                B       .

                ENDP


                ALIGN

                IF :DEF:NO_SCATTER

; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF  ; __MICROLIB

                ENDIF  ; NO_SCATTER


                END
