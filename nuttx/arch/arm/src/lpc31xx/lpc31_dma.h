/************************************************************************************************
 * arch/arm/src/lpc31xx/lpc31_dma.h
 *
 *   Copyright (C) 2009, 2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************************/

#ifndef __ARCH_ARM_SRC_LPC31XX_LPC31_DMA_H
#define __ARCH_ARM_SRC_LPC31XX_LPC31_DMA_H

/************************************************************************************************
 * Included Files
 ************************************************************************************************/

#include <nuttx/config.h>

#include "chip.h"
#include "lpc31_memorymap.h"

/************************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************************/

/* DMA register base address offset into the APB4 domain ****************************************/

#define LPC31_DMA_VBASE                (LPC31_APB4_VADDR+LPC31_APB4_DMA_OFFSET)
#define LPC31_DMA_PBASE                (LPC31_APB4_PADDR+LPC31_APB4_DMA_OFFSET)

/* DMA channel offsets (with respect to the DMA register base address) **************************/

#define LPC31_DMACHAN_OFFSET(n)        ((n)*0x020)
#define LPC31_DMACHAN0_OFFSET          0x000
#define LPC31_DMACHAN1_OFFSET          0x020
#define LPC31_DMACHAN2_OFFSET          0x040
#define LPC31_DMACHAN3_OFFSET          0x060
#define LPC31_DMACHAN4_OFFSET          0x080
#define LPC31_DMACHAN5_OFFSET          0x0a0
#define LPC31_DMACHAN6_OFFSET          0x0c0
#define LPC31_DMACHAN7_OFFSET          0x0e0
#define LPC31_DMACHAN8_OFFSET          0x100
#define LPC31_DMACHAN9_OFFSET          0x120
#define LPC31_DMACHAN10_OFFSET         0x140
#define LPC31_DMACHAN11_OFFSET         0x160

#define LPC31_DMACHAN_ALT_OFFSET(n)    (0x200+((n)*0x010))
#define LPC31_DMACHAN0_ALT_OFFSET      0x200
#define LPC31_DMACHAN1_ALT_OFFSET      0x210
#define LPC31_DMACHAN2_ALT_OFFSET      0x220
#define LPC31_DMACHAN3_ALT_OFFSET      0x230
#define LPC31_DMACHAN4_ALT_OFFSET      0x240
#define LPC31_DMACHAN5_ALT_OFFSET      0x250
#define LPC31_DMACHAN6_ALT_OFFSET      0x260
#define LPC31_DMACHAN7_ALT_OFFSET      0x270
#define LPC31_DMACHAN8_ALT_OFFSET      0x280
#define LPC31_DMACHAN9_ALT_OFFSET      0x290
#define LPC31_DMACHAN10_ALT_OFFSET     0x2a0
#define LPC31_DMACHAN11_ALT_OFFSET     0x2b0

/* DMA channel virtual base addresses ***********************************************************/

#define LPC31_DMACHAN_VBASE(n)         (LPC31_DMA_VBASE+LPC31_DMACHAN_OFFSET(n))
#define LPC31_DMACHAN0_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN0_OFFSET)
#define LPC31_DMACHAN1_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN1_OFFSET)
#define LPC31_DMACHAN2_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN2_OFFSET)
#define LPC31_DMACHAN3_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN3_OFFSET)
#define LPC31_DMACHAN4_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN4_OFFSET)
#define LPC31_DMACHAN5_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN5_OFFSET)
#define LPC31_DMACHAN6_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN6_OFFSET)
#define LPC31_DMACHAN7_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN7_OFFSET)
#define LPC31_DMACHAN8_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN8_OFFSET)
#define LPC31_DMACHAN9_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN9_OFFSET)
#define LPC31_DMACHAN10_VBASE          (LPC31_DMA_VBASE+LPC31_DMACHAN10_OFFSET)
#define LPC31_DMACHAN11_VBASE          (LPC31_DMA_VBASE+LPC31_DMACHAN11_OFFSET)

#define LPC31_DMACHAN_ALT_VBASE(n)     (LPC31_DMA_VBASE+LPC31_DMACHAN_ALT_OFFSET(n))
#define LPC31_DMACHAN0_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN0_ALT_OFFSET)
#define LPC31_DMACHAN1_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN1_ALT_OFFSET)
#define LPC31_DMACHAN2_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN2_ALT_OFFSET)
#define LPC31_DMACHAN3_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN3_ALT_OFFSET)
#define LPC31_DMACHAN4_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN4_ALT_OFFSET)
#define LPC31_DMACHAN5_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN5_ALT_OFFSET)
#define LPC31_DMACHAN6_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN6_ALT_OFFSET)
#define LPC31_DMACHAN7_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN7_ALT_OFFSET)
#define LPC31_DMACHAN8_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN8_ALT_OFFSET)
#define LPC31_DMACHAN9_VBASE           (LPC31_DMA_VBASE+LPC31_DMACHAN9_ALT_OFFSET)
#define LPC31_DMACHAN10_VBASE          (LPC31_DMA_VBASE+LPC31_DMACHAN10_ALT_OFFSET)
#define LPC31_DMACHAN11_VBASE          (LPC31_DMA_VBASE+LPC31_DMACHAN11_ALT_OFFSET)

/* DMA channel register offsets (with respect to the DMA channel register base) *****************/

#define LPC31_DMACHAN_SRCADDR_OFFSET   0x000 /* Source address register of DMA channel */
#define LPC31_DMACHAN_DESTADDR_OFFSET  0X004 /* Destination address register of DMA channel */
#define LPC31_DMACHAN_XFERLEN_OFFSET   0X008 /* Transfer length register for DMA channel */
#define LPC31_DMACHAN_CONFIG_OFFSET    0x00c /* Configuration register for DMA channel */
#define LPC31_DMACHAN_ENABLE_OFFSET    0x010 /* Enable register for DMA channel */
#define LPC31_DMACHAN_XFERCOUNT_OFFSET 0x01c /* Transfer counter register for DMA channel */

/* DMA global register offsets (with respect to the DMA register base) *************************/

#define LPC31_DMA_ALTENABLE_OFFSET     0x400 /* Alternative enable register */
#define LPC31_DMA_IRQSTATUSCLR_OFFSET  0x404 /* IRQ status clear register */
#define LPC31_DMA_IRQMASK_OFFSET       0x408 /* IRQ mask register */
#define LPC31_DMA_TESTSTATUS_OFFSET    0x40c /* Test FIFO response status register */
#define LPC31_DMA_SOFTINT_OFFSET       0x410 /* Software interrupt register */

/* DMA channel register (virtual) addresses *****************************************************/

#define LPC31_DMACHAN_SRCADDR(n)       (LPC31_DMACHAN_VBASE(n)+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN_DESTADDR(n)      (LPC31_DMACHAN_VBASE(n)+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN_XFERLEN(n)       (LPC31_DMACHAN_VBASE(n)+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN_CONFIG(n)        (LPC31_DMACHAN_VBASE(n)+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN_ENABLE(n)        (LPC31_DMACHAN_VBASE(n)+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN_XFERCOUNT(n)     (LPC31_DMACHAN_VBASE(n)+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN0_SRCADDR         (LPC31_DMACHAN0_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN0_DESTADDR        (LPC31_DMACHAN0_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN0_XFERLEN         (LPC31_DMACHAN0_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN0_CONFIG          (LPC31_DMACHAN0_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN0_ENABLE          (LPC31_DMACHAN0_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN0_XFERCOUNT       (LPC31_DMACHAN0_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN1_SRCADDR         (LPC31_DMACHAN1_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN1_DESTADDR        (LPC31_DMACHAN1_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN1_XFERLEN         (LPC31_DMACHAN1_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN1_CONFIG          (LPC31_DMACHAN1_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN1_ENABLE          (LPC31_DMACHAN1_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN1_XFERCOUNT       (LPC31_DMACHAN1_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN2_SRCADDR         (LPC31_DMACHAN2_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN2_DESTADDR        (LPC31_DMACHAN2_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN2_XFERLEN         (LPC31_DMACHAN2_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN2_CONFIG          (LPC31_DMACHAN2_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN2_ENABLE          (LPC31_DMACHAN2_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN2_XFERCOUNT       (LPC31_DMACHAN2_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN3_SRCADDR         (LPC31_DMACHAN3_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN3_DESTADDR        (LPC31_DMACHAN3_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN3_XFERLEN         (LPC31_DMACHAN3_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN3_CONFIG          (LPC31_DMACHAN3_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN3_ENABLE          (LPC31_DMACHAN3_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN3_XFERCOUNT       (LPC31_DMACHAN3_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN4_SRCADDR         (LPC31_DMACHAN4_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN4_DESTADDR        (LPC31_DMACHAN4_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN4_XFERLEN         (LPC31_DMACHAN4_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN4_CONFIG          (LPC31_DMACHAN4_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN4_ENABLE          (LPC31_DMACHAN4_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN4_XFERCOUNT       (LPC31_DMACHAN4_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN5_SRCADDR         (LPC31_DMACHAN5_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN5_DESTADDR        (LPC31_DMACHAN5_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN5_XFERLEN         (LPC31_DMACHAN5_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN5_CONFIG          (LPC31_DMACHAN5_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN5_ENABLE          (LPC31_DMACHAN5_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN5_XFERCOUNT       (LPC31_DMACHAN5_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN6_SRCADDR         (LPC31_DMACHAN6_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN6_DESTADDR        (LPC31_DMACHAN6_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN6_XFERLEN         (LPC31_DMACHAN6_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN6_CONFIG          (LPC31_DMACHAN6_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN6_ENABLE          (LPC31_DMACHAN6_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN6_XFERCOUNT       (LPC31_DMACHAN6_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN7_SRCADDR         (LPC31_DMACHAN7_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN7_DESTADDR        (LPC31_DMACHAN7_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN7_XFERLEN         (LPC31_DMACHAN7_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN7_CONFIG          (LPC31_DMACHAN7_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN7_ENABLE          (LPC31_DMACHAN7_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN7_XFERCOUNT       (LPC31_DMACHAN7_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)7

#define LPC31_DMACHAN8_SRCADDR         (LPC31_DMACHAN8_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN8_DESTADDR        (LPC31_DMACHAN8_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN8_XFERLEN         (LPC31_DMACHAN8_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN8_CONFIG          (LPC31_DMACHAN8_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN8_ENABLE          (LPC31_DMACHAN8_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN8_XFERCOUNT       (LPC31_DMACHAN8_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN9_SRCADDR         (LPC31_DMACHAN9_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN9_DESTADDR        (LPC31_DMACHAN9_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN9_XFERLEN         (LPC31_DMACHAN9_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN9_CONFIG          (LPC31_DMACHAN9_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN9_ENABLE          (LPC31_DMACHAN9_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN9_XFERCOUNT       (LPC31_DMACHAN9_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN10_SRCADDR        (LPC31_DMACHAN10_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN10_DESTADDR       (LPC31_DMACHAN10_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN10_XFERLEN        (LPC31_DMACHAN10_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN10_CONFIG         (LPC31_DMACHAN10_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN10_ENABLE         (LPC31_DMACHAN10_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN10_XFERCOUNT      (LPC31_DMACHAN10_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN11_SRCADDR        (LPC31_DMACHAN11_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN11_DESTADDR       (LPC31_DMACHAN11_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN11_XFERLEN        (LPC31_DMACHAN11_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN11_CONFIG         (LPC31_DMACHAN11_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)
#define LPC31_DMACHAN11_ENABLE         (LPC31_DMACHAN11_VBASE+LPC31_DMACHAN_ENABLE_OFFSET)
#define LPC31_DMACHAN11_XFERCOUNT      (LPC31_DMACHAN11_VBASE+LPC31_DMACHAN_XFERCOUNT_OFFSET)

#define LPC31_DMACHAN_ALT_SRCADDR(n)   (LPC31_DMACHAN_ALT_VBASE(n)+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN_ALT_DESTADDR(n)  (LPC31_DMACHAN_ALT_VBASE(n)+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN_ALT_XFERLEN(n)   (LPC31_DMACHAN_ALT_VBASE(n)+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN_ALT_CONFIG(n)    (LPC31_DMACHAN_ALT_VBASE(n)+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN0_ALT_SRCADDR     (LPC31_DMACHAN0_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN0_ALT_DESTADDR    (LPC31_DMACHAN0_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN0_ALT_XFERLEN     (LPC31_DMACHAN0_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN0_ALT_CONFIG      (LPC31_DMACHAN0_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN1_ALT_SRCADDR     (LPC31_DMACHAN1_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN1_ALT_DESTADDR    (LPC31_DMACHAN1_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN1_ALT_XFERLEN     (LPC31_DMACHAN1_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN1_ALT_CONFIG      (LPC31_DMACHAN1_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN2_ALT_SRCADDR     (LPC31_DMACHAN2_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN2_ALT_DESTADDR    (LPC31_DMACHAN2_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN2_ALT_XFERLEN     (LPC31_DMACHAN2_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN2_ALT_CONFIG      (LPC31_DMACHAN2_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN3_ALT_SRCADDR     (LPC31_DMACHAN3_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN3_ALT_DESTADDR    (LPC31_DMACHAN3_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN3_ALT_XFERLEN     (LPC31_DMACHAN3_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN3_ALT_CONFIG      (LPC31_DMACHAN3_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN4_ALT_SRCADDR     (LPC31_DMACHAN4_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN4_ALT_DESTADDR    (LPC31_DMACHAN4_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN4_ALT_XFERLEN     (LPC31_DMACHAN4_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN4_ALT_CONFIG      (LPC31_DMACHAN4_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN5_ALT_SRCADDR     (LPC31_DMACHAN5_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN5_ALT_DESTADDR    (LPC31_DMACHAN5_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN5_ALT_XFERLEN     (LPC31_DMACHAN5_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN5_ALT_CONFIG      (LPC31_DMACHAN5_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN6_ALT_SRCADDR     (LPC31_DMACHAN6_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN6_ALT_DESTADDR    (LPC31_DMACHAN6_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN6_ALT_XFERLEN     (LPC31_DMACHAN6_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN6_ALT_CONFIG      (LPC31_DMACHAN6_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN7_ALT_SRCADDR     (LPC31_DMACHAN7_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN7_ALT_DESTADDR    (LPC31_DMACHAN7_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN7_ALT_XFERLEN     (LPC31_DMACHAN7_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN7_ALT_CONFIG      (LPC31_DMACHAN7_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN8_ALT_SRCADDR     (LPC31_DMACHAN8_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN8_ALT_DESTADDR    (LPC31_DMACHAN8_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN8_ALT_XFERLEN     (LPC31_DMACHAN8_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN8_ALT_CONFIG      (LPC31_DMACHAN8_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN9_ALT_SRCADDR     (LPC31_DMACHAN9_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN9_ALT_DESTADDR    (LPC31_DMACHAN9_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN9_ALT_XFERLEN     (LPC31_DMACHAN9_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN9_ALT_CONFIG      (LPC31_DMACHAN9_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN10_ALT_SRCADDR    (LPC31_DMACHAN10_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN10_ALT_DESTADDR   (LPC31_DMACHAN10_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN10_ALT_XFERLEN    (LPC31_DMACHAN10_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN10_ALT_CONFIG     (LPC31_DMACHAN10_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

#define LPC31_DMACHAN11_ALT_SRCADDR    (LPC31_DMACHAN11_ALT_VBASE+LPC31_DMACHAN_SRCADDR_OFFSET)
#define LPC31_DMACHAN11_ALT_DESTADDR   (LPC31_DMACHAN11_ALT_VBASE+LPC31_DMACHAN_DESTADDR_OFFSET)
#define LPC31_DMACHAN11_ALT_XFERLEN    (LPC31_DMACHAN11_ALT_VBASE+LPC31_DMACHAN_XFERLEN_OFFSET)
#define LPC31_DMACHAN11_ALT_CONFIG     (LPC31_DMACHAN11_ALT_VBASE+LPC31_DMACHAN_CONFIG_OFFSET)

/* DMA global register (virtual) addresses ******************************************************/

#define LPC31_DMA_ALTENABLE            (LPC31_DMA_VBASE+LPC31_DMA_ALTENABLE_OFFSET)
#define LPC31_DMA_IRQSTATUSCLR         (LPC31_DMA_VBASE+LPC31_DMA_IRQSTATUSCLR_OFFSET)
#define LPC31_DMA_IRQMASK              (LPC31_DMA_VBASE+LPC31_DMA_IRQMASK_OFFSET)
#define LPC31_DMA_TESTSTATUS           (LPC31_DMA_VBASE+LPC31_DMA_TESTSTATUS_OFFSET)
#define LPC31_DMA_SOFTINT              (LPC31_DMA_VBASE+LPC31_DMA_SOFTINT_OFFSET)

/* DMA channel register bit definitions *********************************************************/

/* TRANSFER_LENGTH (addresses 0x17000008 (channel 0) to 0x17000168 (channel 11)) */

#define DMACHAN_XFRLEN_SHIFT             (0)   /* Bits 0-20: Transfer length */
#define DMACHAN_XFRLEN_MASK              (0x001fffff << DMACHAN_XFRLEN_SHIFT)

/* CONFIGURATION (addresses 0x1700000c (channel 0) to 0x1700016c (channel 11)) */

#define DMACHAN_CONFIG_CIRC              (1 << 18) /* Bit 18: Enable circular buffer */
#define DMACHAN_CONFIG_COMPCHENABLE      (1 << 17) /* Bit 17: Enable companion channel */
#define DMACHAN_CONFIG_COMPCHNR_SHIFT    (13)      /* Bits 13-15: Companion channel number */
#define DMACHAN_CONFIG_COMPCHNR_MASK     (7 << DMACHAN_CONFIG_COMPCHNR_SHIFT)
#define DMACHAN_CONFIG_INVENDIAN         (1 << 12) /* Bit 12: Invert endian-ness */
#define DMACHAN_CONFIG_XFERSIZE_SHIFT    (10)      /* Bits 10-11: Transfer size */
#define DMACHAN_CONFIG_XFERSIZE_MASK     (3 << DMACHAN_CONFIG_XFERSIZE_SHIFT)
#  define DMACHAN_CONFIG_XFERSIZE_WORDS  (0 << DMACHAN_CONFIG_XFERSIZE_SHIFT) /* Transfer words */
#  define DMACHAN_CONFIG_XFERSIZE_HWORDS (1 << DMACHAN_CONFIG_XFERSIZE_SHIFT) /* Transfer half-words */
#  define DMACHAN_CONFIG_XFERSIZE_BYTES  (2 << DMACHAN_CONFIG_XFERSIZE_SHIFT) /* Transfer bytes */
#  define DMACHAN_CONFIG_XFERSIZE_BURSTS (3 << DMACHAN_CONFIG_XFERSIZE_SHIFT) /* Transfer bursts */
#define DMACHAN_CONFIG_RDSLAVENR_SHIFT   (5)       /* Bits 5-9: Read slave enable */
#define DMACHAN_CONFIG_RDSLAVENR_MASK    (31 << DMACHAN_CONFIG_RDSLAVENR_SHIFT)
#define DMACHAN_CONFIG_WRSLAVENR_SHIFT   (0)       /* Bits 0-4: Write slave enable */
#define DMACHAN_CONFIG_WRSLAVENR_MASK    (31 << DMACHAN_CONFIG_WRSLAVENR_SHIFT)

/* ENABLE (addresses 0x17000010 (channel 0) to 0x17000170 (channel 11)) */

#define DMACHAN_ENABLE_BIT               (1 << 0)  /* Bit 0: Enable */

/* TRANSFER_COUNTER (addresses 0x1700001v (channel 0) to 0x1700017c (channel 11)) */

#define DMACHAN_XFRCOUNT_SHIFT           (0)       /* Bits 0-20: Transfer count */
#define DMACHAN_XFRCOUNT_MASK            (0x001fffff << DMACHAN_XFRCOUNT_SHIFT)

/* DMA global register bit definitions **********************************************************/

/* ALT_ENABLE (address 0x17000400) */

#define DMA_ALTENABLE_CHAN11             (1 << 11) /* Bit 11: Enable channel 11 */
#define DMA_ALTENABLE_CHAN10             (1 << 10) /* Bit 10: Enable channel 10 */
#define DMA_ALTENABLE_CHAN9              (1 << 9)  /* Bit 9:  Enable channel 9 */
#define DMA_ALTENABLE_CHAN8              (1 << 8)  /* Bit 8:  Enable channel 8 */
#define DMA_ALTENABLE_CHAN7              (1 << 7)  /* Bit 7:  Enable channel 7 */
#define DMA_ALTENABLE_CHAN6              (1 << 6)  /* Bit 6:  Enable channel 6 */
#define DMA_ALTENABLE_CHAN5              (1 << 5)  /* Bit 5:  Enable channel 5 */
#define DMA_ALTENABLE_CHAN4              (1 << 4)  /* Bit 4:  Enable channel 4 */
#define DMA_ALTENABLE_CHAN3              (1 << 3)  /* Bit 3:  Enable channel 3 */
#define DMA_ALTENABLE_CHAN2              (1 << 2)  /* Bit 2:  Enable channel 2 */
#define DMA_ALTENABLE_CHAN1              (1 << 1)  /* Bit 1:  Enable channel 1 */
#define DMA_ALTENABLE_CHAN0              (1 << 0)  /* Bit 0:  Enable channel 0 */

/* IRQ_STATUS_CLR (address 0x17000404) */

#define DMA_IRQSTATUSCLR_DMAABORT        (1 << 31) /* Bit 31: DMA abort */
#define DMA_IRQSTATUSCLR_SOFTINT         (1 << 30) /* Bit 30: Soft interrupt, scatter gather */
#define DMA_IRQSTATUSCLR_HALFWAY11       (1 << 23) /* Bit 23: Chan 11 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED11      (1 << 22) /* Bit 22: Chan 11 finished */
#define DMA_IRQSTATUSCLR_HALFWAY10       (1 << 21) /* Bit 21: Chan 10 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED10      (1 << 20) /* Bit 20: Chan 10 finished */
#define DMA_IRQSTATUSCLR_HALFWAY9        (1 << 19) /* Bit 19: Chan 9 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED9       (1 << 18) /* Bit 18: Chan 9 finished */
#define DMA_IRQSTATUSCLR_HALFWAY8        (1 << 17) /* Bit 17: Chan 8 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED8       (1 << 16) /* Bit 16: Chan 8 finished */
#define DMA_IRQSTATUSCLR_HALFWAY7        (1 << 15) /* Bit 15: Chan 7 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED7       (1 << 14) /* Bit 14: Chan 7 finished */
#define DMA_IRQSTATUSCLR_HALFWAY6        (1 << 13) /* Bit 13: Chan 6 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED6       (1 << 12) /* Bit 12: Chan 6 finished */
#define DMA_IRQSTATUSCLR_HALFWAY5        (1 << 11) /* Bit 11: Chan 5 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED5       (1 << 10) /* Bit 10: Chan 5 finished */
#define DMA_IRQSTATUSCLR_HALFWAY4        (1 << 9)  /* Bit 9:  Chan 4 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED4       (1 << 8)  /* Bit 8:  Chan 4 finished */
#define DMA_IRQSTATUSCLR_HALFWAY3        (1 << 7)  /* Bit 7:  Chan 3 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED3       (1 << 6)  /* Bit 6:  Chan 3 finished */
#define DMA_IRQSTATUSCLR_HALFWAY2        (1 << 5)  /* Bit 5:  Chan 2 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED2       (1 << 4)  /* Bit 4:  Chan 2 finished */
#define DMA_IRQSTATUSCLR_HALFWAY1        (1 << 3)  /* Bit 3:  Chan 1 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED1       (1 << 2)  /* Bit 2:  Chan 1 finished */
#define DMA_IRQSTATUSCLR_HALFWAY0        (1 << 1)  /* Bit 1:  Chan 0 more than half finished */
#define DMA_IRQSTATUSCLR_FINISHED0       (1 << 0)  /* Bit 0:  Chan 0 finished */

/* IRQ_MASK (address 0x17000404) */

#define DMA_IRQMASK_DMAABORT             (1 << 31) /* Bit 31: DMA abort */
#define DMA_IRQMASK_SOFTINT              (1 << 30) /* Bit 30: Soft interrupt, scatter gather */
#define DMA_IRQMASK_HALFWAY11            (1 << 23) /* Bit 23: Chan 11 more than half finished */
#define DMA_IRQMASK_FINISHED11           (1 << 22) /* Bit 22: Chan 11 finished */
#define DMA_IRQMASK_HALFWAY10            (1 << 21) /* Bit 21: Chan 10 more than half finished */
#define DMA_IRQMASK_FINISHED10           (1 << 20) /* Bit 20: Chan 10 finished */
#define DMA_IRQMASK_HALFWAY9             (1 << 19) /* Bit 19: Chan 9 more than half finished */
#define DMA_IRQMASK_FINISHED9            (1 << 18) /* Bit 18: Chan 9 finished */
#define DMA_IRQMASK_HALFWAY8             (1 << 17) /* Bit 17: Chan 8 more than half finished */
#define DMA_IRQMASK_FINISHED8            (1 << 16) /* Bit 16: Chan 8 finished */
#define DMA_IRQMASK_HALFWAY7             (1 << 15) /* Bit 15: Chan 7 more than half finished */
#define DMA_IRQMASK_FINISHED7            (1 << 14) /* Bit 14: Chan 7 finished */
#define DMA_IRQMASK_HALFWAY6             (1 << 13) /* Bit 13: Chan 6 more than half finished */
#define DMA_IRQMASK_FINISHED6            (1 << 12) /* Bit 12: Chan 6 finished */
#define DMA_IRQMASK_HALFWAY5             (1 << 11) /* Bit 11: Chan 5 more than half finished */
#define DMA_IRQMASK_FINISHED5            (1 << 10) /* Bit 10: Chan 5 finished */
#define DMA_IRQMASK_HALFWAY4             (1 << 9)  /* Bit 9:  Chan 4 more than half finished */
#define DMA_IRQMASK_FINISHED4            (1 << 8)  /* Bit 8:  Chan 4 finished */
#define DMA_IRQMASK_HALFWAY3             (1 << 7)  /* Bit 7:  Chan 3 more than half finished */
#define DMA_IRQMASK_FINISHED3            (1 << 6)  /* Bit 6:  Chan 3 finished */
#define DMA_IRQMASK_HALFWAY2             (1 << 5)  /* Bit 5:  Chan 2 more than half finished */
#define DMA_IRQMASK_FINISHED2            (1 << 4)  /* Bit 4:  Chan 2 finished */
#define DMA_IRQMASK_HALFWAY1             (1 << 3)  /* Bit 3:  Chan 1 more than half finished */
#define DMA_IRQMASK_FINISHED1            (1 << 2)  /* Bit 2:  Chan 1 finished */
#define DMA_IRQMASK_HALFWAY0             (1 << 1)  /* Bit 1:  Chan 0 more than half finished */
#define DMA_IRQMASK_FINISHED0            (1 << 0)  /* Bit 0:  Chan 0 finished */

/* SOFT_INT (address 0x1700040c) */

#define DMA_SOFTINT_ENABLE               (1 << 0)  /* Bit 0:  Enable soft interrupt */

/************************************************************************************************
 * Public Types
 ************************************************************************************************/

/************************************************************************************************
 * Public Data
 ************************************************************************************************/

/************************************************************************************************
 * Public Functions
 ************************************************************************************************/

#endif /* __ARCH_ARM_SRC_LPC31XX_LPC31_DMA_H */