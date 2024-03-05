/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

// ARM wrapper code
// imagenet-riscv
// This file was @generated by ai8xize.py --test-dir sdk/Examples/MAX78002/CNN --prefix imagenet-riscv --checkpoint-file trained/ai87-imagenet-effnet2-q.pth.tar --config-file networks/ai87-imagenet-effnet2.yaml --riscv --riscv-debug --device MAX78002 --timer 0 --display-checkpoint --verbose --overwrite

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include "mxc.h"
#include "gcfr_regs.h"
#include "fcr_regs.h"
#include "sema_regs.h"

extern volatile void const *__FlashStart_; // Defined in linker file

void WakeISR(void)
{
    MXC_SEMA->irq0 = MXC_F_SEMA_IRQ0_EN & ~MXC_F_SEMA_IRQ0_CM4_IRQ;
}

int main(void)
{
    MXC_ICC_Enable(MXC_ICC0); // Enable cache

    // Switch to 120 MHz clock
    MXC_SYS_Clock_Select(MXC_SYS_CLOCK_IPO);
    MXC_GCR->ipll_ctrl |= MXC_F_GCR_IPLL_CTRL_EN; // Enable IPLL
    SystemCoreClockUpdate();

    MXC_FCR->urvbootaddr = (uint32_t)&__FlashStart_; // Set RISC-V boot address
    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_SMPHR); // Enable Sempahore clock
    MXC_NVIC_SetVector(RISCV_IRQn, WakeISR); // Set wakeup ISR

    // DO NOT DELETE THIS LINE:
    MXC_Delay(SEC(2)); // Let debugger interrupt if needed

    MXC_SYS_ClockEnable(MXC_SYS_PERIPH_CLOCK_CPU1); // Enable RISC-V clock

    MXC_LP_EnterSleepMode(); // Let RISC-V run

    return 0;
}
