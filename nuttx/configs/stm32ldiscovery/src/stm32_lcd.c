/****************************************************************************
 * configs/stm32ldiscovery/src/stm32_lcd.c
 *
 *   Copyright (C) 2013 Gregory Nutt. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
 *
 * References:
 *   - Based on the NuttX LCD1602 driver.
 *   - "STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx advanced ARM-based
 *     32-bit MCUs", STMicroelectronics, RM0038
 *   - "STM32L1 discovery kits: STM32L-DISCOVERY and 32L152CDISCOVERY,"
 *     STMicroelectronics, UM1079
 *   - STM32L-Discovery Firmware Pack V1.0.2 (for character encoding)
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/ascii.h>
#include <nuttx/streams.h>
#include <nuttx/fs/fs.h>
#include <nuttx/lcd/slcd_codec.h>

#include "up_arch.h"
#include "stm32_gpio.h"
#include "chip/stm32_lcd.h"

#include "stm32ldiscovery.h"

#ifdef CONFIG_STM32_LCD

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/

/* Define CONFIG_DEBUG_LCD to enable detailed LCD debug output. Verbose debug
 * must also be enabled.
 */

#ifndef CONFIG_LIB_SLCDCODEC
#  error "This SLCD driver requires CONFIG_LIB_SLCDCODEC"
#endif

#ifndef CONFIG_DEBUG
#  undef CONFIG_DEBUG_VERBOSE
#  undef CONFIG_DEBUG_GRAPHICS
#  undef CONFIG_DEBUG_LCD
#endif

#ifndef CONFIG_DEBUG_VERBOSE
#  undef CONFIG_DEBUG_LCD
#endif

#ifndef MIN
#  define MIN(a,b) (a < b ? a : b)
#endif

#ifndef MAX
#  define MAX(a,b) (a > b ? a : b)
#endif

/* LCD **********************************************************************/
/* LCD.  The STM32L152RBT6 supports either a 4x32 or 8x28.  The STM32L-
 * Discovery has an LCD 24 segments, 4 commons.  See stm32ldiscovery.h for
 * the pin mapping.
 */

/* Macro to convert an LCD register offset and bit number into a bit-band
 * address:
 */

#define SLCD_OFFSET           (STM32_LCD_BASE - STM32_PERIPH_BASE)
#define SLCD_BBADDR(o,b)      (STM32_PERIPHBB_BASE + ((SLCD_OFFSET + (o)) << 5) + ((b) << 2))

/* Some useful bit-band addresses */

#define SLCD_CR_LCDEN_BB      SLCD_BBADDR(STM32_LCD_CR_OFFSET,0)
#define SLCD_SR_UDR_BB        SLCD_BBADDR(STM32_LCD_SR_OFFSET,2)

/* LCD characteristics */

#define SLCD_NCHARS           6

/* An ASCII character may need to be decorated with a colon or decimal point */

#define SCLD_DP               0x01
#define SCLD_COLON            0x02

/* Macros used for set/reset the LCD bar */

#define SCLD_BAR0_ON          g_slcdstate.bar[1] |= 8
#define SCLD_BAR0_OFF         g_slcdstate.bar[1] &= ~8
#define SCLD_BAR1_ON          g_slcdstate.bar[0] |= 8
#define SCLD_BAR1_OFF         g_slcdstate.bar[0] &= ~8
#define SCLD_BAR2_ON          g_slcdstate.bar[1] |= 2
#define SCLD_BAR2_OFF         g_slcdstate.bar[1] &= ~2
#define SCLD_BAR3_ON          g_slcdstate.bar[0] |= 2
#define SCLD_BAR3_OFF         g_slcdstate.bar[0] &= ~2

/* These definitions support the logic of slcd_writemem() */

#define SLCD_CHAR1_MASK       0xcffffffc
#define SLCD_CHAR1_UPDATE(s)  (((uint32_t)(s) & 0x0c) << 26) | \
                              ((uint32_t)(s) & 0x03)
#define SLCD_CHAR2_MASK       0xf3ffff03
#define SLCD_CHAR2_UPDATE(s)  (((uint32_t)(s) & 0x0c) << 24) | \
                              (((uint32_t)(s) & 0x02) << 6) | \
                              (((uint32_t)(s) & 0x01) << 2)
#define SLCD_CHAR3_MASK       0xfcfffcff
#define SLCD_CHAR3_UPDATE(s)  (((uint32_t)(s) & 0x0c) << 22) | \
                              (((uint32_t)(s) & 0x03) << 8)
#define SLCD_CHAR4_MASK       0xffcff3ff
#define SLCD_CHAR4_UPDATE(s)  (((uint32_t)(s) & 0x0c) << 18) | \
                              (((uint32_t)(s) & 0x03) << 10)
#define SLCD_CHAR5_MASKA      0xfff3cfff
#define SLCD_CHAR5_MASKB      0xfff3efff
#define SLCD_CHAR5_UPDATEA(s) (((uint32_t)(s) & 0x0c) << 16) | \
                              (((uint32_t)(s) & 0x03) << 12)
#define SLCD_CHAR5_UPDATEB(s) (((uint32_t)(s) & 0x0c) << 16) | \
                              (((uint32_t)(s) & 0x01) << 12)
#define SLCD_CHAR6_MASK       0xfffc3fff
#define SLCD_CHAR6_UPDATEA(s) (((uint32_t)(s) & 0x04) << 15) | \
                              (((uint32_t)(s) & 0x08) << 13) | \
                              (((uint32_t)(s) & 0x03) << 14)
#define SLCD_CHAR6_UPDATEB(s) (((uint32_t)(s) & 0x04) << 15) | \
                              (((uint32_t)(s) & 0x08) << 13) | \
                              (((uint32_t)(s) & 0x03) << 14)

/* Debug ********************************************************************/

#ifdef CONFIG_DEBUG_LCD
#  define lcddbg              dbg
#  define lcdvdbg             vdbg
#else
#  define lcddbg(x...)
#  define lcdvdbg(x...)
#endif

/****************************************************************************
 * Private Type Definition
 ****************************************************************************/

/* SLCD incoming stream structure */

struct slcd_instream_s
{
  struct lib_instream_s stream;
  FAR const char *buffer;
  ssize_t nbytes;
};

/* Global SLCD state */

struct stm32_slcdstate_s
{
  bool initialized;             /* True: Completed initialization sequence */
  uint8_t curpos;               /* The current cursor position */
  uint8_t buffer[SLCD_NCHARS];  /* SLCD ASCII content */
  uint8_t options[SLCD_NCHARS]; /* With colon or decimal point decoration */
#if 0 /* Not used */
  uint8_t bar[2];               /* Controls the bars on the far right of the SLCD */
#endif
};

/****************************************************************************
 * Private Function Protototypes
 ****************************************************************************/
/* Internal utilities */

static void slcd_clear(void);
static int slcd_getstream(FAR struct lib_instream_s *instream);
#if 0 /* Not used */
static uint8_t slcd_getcontrast(void);
static int slcd_setcontrast(uint8_t contrast);
static void slcd_writebar(void);
#endif
static inline uint16_t slcd_mapch(uint8_t ch);
static inline void slcd_writemem(uint16_t bitset, int curpos);
static void slcd_writech(uint8_t ch, uint8_t curpos, uint8_t options);
static inline void slcd_appendch(uint8_t ch, uint8_t options);
static inline void slcd_action(enum slcdcode_e code, uint8_t count);

/* Character driver methods */

static ssize_t slcd_read(FAR struct file *, FAR char *, size_t);
static ssize_t slcd_write(FAR struct file *, FAR const char *, size_t);
#ifndef CONFIG_DISABLE_POLL
static int slcd_poll(FAR struct file *filp, FAR struct pollfd *fds, bool setup);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This is the driver state structure (there is no retained state information) */

static const struct file_operations g_slcdops =
{
  0,             /* open */
  0,             /* close */
  slcd_read,     /* read */
  slcd_write,    /* write */
  0,             /* seek */
  0              /* ioctl */
#ifndef CONFIG_DISABLE_POLL
  , slcd_poll    /* poll */
#endif
};

/* LCD state data */

static struct stm32_slcdstate_s g_slcdstate;

/* LCD Mapping
 *
 *              A
 *          ---------    _
 *         |\   |J  /|  |_| COL
 *        F| H  |  K |B
 *         |  \ | /  |   _
 *         --G-- --M-+  |_| COL
 *         |   /| \  |
 *        E|  Q |  N |C
 *         | /  |P  \|   _
 *          ---------   |_| DP
 *              D
 *
 * LCD character 16-bit-encoding:
 * { E , D , P , N, M , C , COL , DP, B , A , K , J, G , F , Q , H   }
 */

#warning "Encodings for all punctuation are incomplete"

/* Space and ASCII punctuation: 0x20-0x2f */

static const uint16_t g_slcdpunct1[ASCII_0 -  ASCII_SPACE] =
{
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  /* <space> ! " # $ % & '  */
  0x0000, 0x0000, 0xa0dd, 0x0000, 0x0000, 0xa000, 0x0000, 0x00c0   /*       () * + , - . /  */
};

/* ASCII numerals 0-9: 0x30-0x39 */

static const uint16_t g_slcdnummap[ASCII_COLON - ASCII_0] =
{
  0x5f00, 0x4200, 0xf500, 0x6700, 0xea00, 0xaf00, 0xbf00, 0x4600,  /* 0-7 */
  0xff00, 0xef00                                                   /* 8-9 */
};

/* ASCII punctuation: 0x3a-0x40 */

static const uint16_t g_slcdpunct2[ASCII_A - ASCII_COLON] =
{
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000           /*  : ; < = > ? @   */
};

/* Upper case letters A-Z: 0x41-0x5a.  Also lower case letters a-z: 0x61-0x7a */

static const uint16_t g_slcdalphamap[ASCII_LBRACKET - ASCII_A] =
{
  0xfe00, 0x6714, 0x1d00, 0x4714, 0x9d00, 0x9c00, 0x3f00, 0xfa00,  /* A-H */
  0x0014, 0x5300, 0x9841, 0x1900, 0x5a48, 0x5a09, 0x5f00, 0xfc00,  /* I-P */
  0x5f01, 0xfc01, 0xaf00, 0x0414, 0x5b00, 0x18c0, 0x5a81, 0x00c9,  /* Q-X */
  0x0058, 0x05c0                                                   /* y-Z */
};

/* ASCII punctuation: 0x5b-0x60 */

static const uint16_t g_slcdpunct3[ASCII_a -  ASCII_LBRACKET] =
{
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000                   /*  [ \ ] ^ _  <right quote>  */
};

/* ASCII punctuation: 0x7b-0x7e */

static const uint16_t g_slcdpunct4[ASCII_DEL -  ASCII_LBRACE]=
{
  0x0000, 0x0000, 0x0000, 0x0000                                    /*  { | } ~  */
};

/* All GPIOs that need to be configured for the STM32L-Discovery LCD */

static uint32_t g_slcdgpio[BOARD_SLCD_NGPIOS] =
{
  BOARD_SLCD_COM0,  BOARD_SLCD_COM1, BOARD_SLCD_COM2, BOARD_SLCD_COM3,

  BOARD_SLCD_SEG0,  BOARD_SLCD_SEG1,  BOARD_SLCD_SEG2,  BOARD_SLCD_SEG3,
  BOARD_SLCD_SEG4,  BOARD_SLCD_SEG5,  BOARD_SLCD_SEG6,  BOARD_SLCD_SEG7,
  BOARD_SLCD_SEG8,  BOARD_SLCD_SEG9,  BOARD_SLCD_SEG10, BOARD_SLCD_SEG11,
  BOARD_SLCD_SEG12, BOARD_SLCD_SEG13, BOARD_SLCD_SEG14, BOARD_SLCD_SEG15,
  BOARD_SLCD_SEG16, BOARD_SLCD_SEG17, BOARD_SLCD_SEG18, BOARD_SLCD_SEG19,
  BOARD_SLCD_SEG20, BOARD_SLCD_SEG21, BOARD_SLCD_SEG22, BOARD_SLCD_SEG23
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: slcd_clear
 ****************************************************************************/

static void slcd_clear(void)
{
  uint32_t regaddr;

  /* Make sure that any previous transfer is complete.  The firmware sets
   * the UDR each it modifies the LCD_RAM. The UDR bit stays set until the
   * end of the update.  During this time the LCD_RAM is write protected.
   */

  while ((getreg32(STM32_LCD_SR) & LCD_SR_UDR) != 0);

  /* Write all zerios in to the LCD RAM */

  for (regaddr = STM32_LCD_RAML(0); regaddr <= STM32_LCD_RAMH(7); regaddr++)
    {
      putreg32(0, regaddr);
    }

  /* Set all buffered data to undecorated spaces and home the cursor */

  memset(g_slcdstate.buffer, ' ', SLCD_NCHARS);
  memset(g_slcdstate.options, 0, SLCD_NCHARS);
  g_slcdstate.curpos = 0;

  /* Set the UDR bit to transfer the updated data to the second level
   * buffer.
   */

  putreg32(1, SLCD_SR_UDR_BB);
}

/****************************************************************************
 * Name: slcd_getstream
 *
 * Description:
 *   Get one character from the keyboard.
 *
 ****************************************************************************/

static int slcd_getstream(FAR struct lib_instream_s *instream)
{
  FAR struct slcd_instream_s *slcdstream = (FAR struct slcd_instream_s *)instream;

  DEBUGASSERT(slcdstream && slcdstream->buffer);
  if (slcdstream->nbytes > 0)
    {
      slcdstream->nbytes--;
      slcdstream->stream.nget++;
      return (int)*slcdstream->buffer++;
    }

  return EOF;
}

/****************************************************************************
 * Name: slcd_getcontrast
 ****************************************************************************/

#if 0 /* Not used */
static uint8_t slcd_getcontrast(void)
{
  return (getreg32(STM32_LCD_FCR) & LCD_FCR_CC_MASK) >> LCD_FCR_CC_SHIFT;
}
#endif

/****************************************************************************
 * Name: slcd_setcontrast
 ****************************************************************************/

#if 0 /* Not used */
static int slcd_setcontrast(uint8_t contrast)
{
  uint2_t regval;
  int ret = OK;

  /* Make sure that the contrast setting is within range */

  if (contrast > 7)
    {
      contrast = 7;
      ret = -ERANGE;
    }

  regval = getreg32(STM32_LCD_FCR);
  regval &= !LCD_FCR_CC_MASK;
  regval |= contrast << LCD_FCR_CC_SHIFT;
  putreg32(regval, STM32_LCD_FCR);
}
#endif

/****************************************************************************
 * Name: slcd_writebar
 ****************************************************************************/

#if 0 /* Not used */
static void slcd_writebar(void)
{
  uint32_t regval;

  /* Make sure that any previous transfer is complete.  The firmware sets
   * the UDR each it modifies the LCD_RAM. The UDR bit stays set until the
   * end of the update.  During this time the LCD_RAM is write protected.
   */

  while ((getreg32(STM32_LCD_SR) & LCD_SR_UDR) != 0);

  /* Update the BAR */

  regval  = getreg32(STM32_LCD_RAM2L);
  regval &= 0xffff5fff;
  regval |= (uint32_t)(g_slcdstate.bar[0] << 12);
  putreg32(regval, STM32_LCD_RAM2L);

  regval  = getreg32(STM32_LCD_RAM3L);
  regval &= 0xffff5fff;
  regval |= (uint32_t)(g_slcdstate.bar[1] << 12);
  putreg32(regval, STM32_LCD_RAM3L);


  /* Set the UDR bit to transfer the updated data to the second level
   * buffer.
   */

  putreg32(1, SLCD_SR_UDR_BB);
}
#endif

/****************************************************************************
 * Name: slcd_mapch
 ****************************************************************************/

static inline uint16_t slcd_mapch(uint8_t ch)
{
  /* ASCII control characters, the forward delete character, period, colon,
   * and all 8-bit ASCII character have already been handled prior to this
   * function.
   */

  /* Return spaces all control characters (this should not happen) */

  if (ch < ASCII_SPACE)
    {
      return 0x0000;
    }

  /* Handle space and the first block of puncutation */

  if (ch < ASCII_0)
    {
      return g_slcdpunct1[(int)ch - ASCII_SPACE];
    }

  /* Handle numbers */

  else if (ch < ASCII_COLON)
    {
      return g_slcdnummap[(int)ch - ASCII_0];
    }

  /* Handle the next block of puncutation */

  else if (ch < ASCII_A)
    {
      return g_slcdpunct2[(int)ch - ASCII_COLON];
    }

  /* Handle upper case letters */

  else if (ch < ASCII_LBRACKET)
    {
      return g_slcdnummap[(int)ch - ASCII_A];
    }

  /* Handle the next block of puncutation */

  else if (ch < ASCII_a)
    {
      return g_slcdpunct3[(int)ch - ASCII_LBRACKET];
    }

  /* Handle lower case letters (by mapping them to upper case */

  else if (ch < ASCII_LBRACE)
    {
      return g_slcdnummap[(int)ch - ASCII_a];
    }

  /* Handle the final block of puncutation */

  else if (ch < ASCII_DEL)
    {
      return g_slcdpunct4[(int)ch - ASCII_LBRACE];
    }

  /* Ignore 8-bit ASCII and DEL (this should not happen) */

   return 0x0000;
}

/****************************************************************************
 * Name: slcd_writemem
 ****************************************************************************/

static inline void slcd_writemem(uint16_t bitset, int curpos)
{
  uint32_t regval;
  uint8_t segments[4];
  int i;
  int j;

  /* Isolate the least significant bits */

  for (i = 12, j = 0; j < 4; i -= 4, j++)
    {
      segments[j] = (bitset >> i) & 0x0f;
    }

  /* Make sure that any previous transfer is complete.  The firmware sets
   * the UDR each it modifies the LCD_RAM. The UDR bit stays set until the
   * end of the update.  During this time the LCD_RAM is write protected.
   */

  while ((getreg32(STM32_LCD_SR) & LCD_SR_UDR) != 0);

  /* Now update the SLCD memory for the character at this cursor position by
   * decoding the bit-mapped value
   */

  switch (curpos)
    {
    case 0:
      regval = getreg32(STM32_LCD_RAM0L);
      regval &= SLCD_CHAR1_MASK;
      regval |= SLCD_CHAR1_UPDATE(segments[0]);
      putreg32(regval, STM32_LCD_RAM0L);

      regval = getreg32(STM32_LCD_RAM1L);
      regval &= SLCD_CHAR1_MASK;
      regval |= SLCD_CHAR1_UPDATE(segments[1]);
      putreg32(regval, STM32_LCD_RAM1L);

      regval = getreg32(STM32_LCD_RAM2L);
      regval &= SLCD_CHAR1_MASK;
      regval |= SLCD_CHAR1_UPDATE(segments[2]);
      putreg32(regval, STM32_LCD_RAM2L);

      regval = getreg32(STM32_LCD_RAM3L);
      regval &= SLCD_CHAR1_MASK;
      regval |= SLCD_CHAR1_UPDATE(segments[3]);
      putreg32(regval, STM32_LCD_RAM3L);
      break;

    case 1:
      regval = getreg32(STM32_LCD_RAM0L);
      regval &= SLCD_CHAR2_MASK;
      regval |= SLCD_CHAR2_UPDATE(segments[0]);
      putreg32(regval, STM32_LCD_RAM0L);

      regval = getreg32(STM32_LCD_RAM1L);
      regval &= SLCD_CHAR2_MASK;
      regval |= SLCD_CHAR2_UPDATE(segments[1]);
      putreg32(regval, STM32_LCD_RAM1L);

      regval = getreg32(STM32_LCD_RAM2L);
      regval &= SLCD_CHAR2_MASK;
      regval |= SLCD_CHAR2_UPDATE(segments[2]);
      putreg32(regval, STM32_LCD_RAM2L);

      regval = getreg32(STM32_LCD_RAM3L);
      regval &= SLCD_CHAR2_MASK;
      regval |= SLCD_CHAR2_UPDATE(segments[3]);
      putreg32(regval, STM32_LCD_RAM3L);
      break;

    case 2:
      regval = getreg32(STM32_LCD_RAM0L);
      regval &= SLCD_CHAR3_MASK;
      regval |= SLCD_CHAR3_UPDATE(segments[0]);
      putreg32(regval, STM32_LCD_RAM0L);

      regval = getreg32(STM32_LCD_RAM1L);
      regval &= SLCD_CHAR3_MASK;
      regval |= SLCD_CHAR3_UPDATE(segments[1]);
      putreg32(regval, STM32_LCD_RAM1L);

      regval = getreg32(STM32_LCD_RAM2L);
      regval &= SLCD_CHAR3_MASK;
      regval |= SLCD_CHAR3_UPDATE(segments[2]);
      putreg32(regval, STM32_LCD_RAM2L);

      regval = getreg32(STM32_LCD_RAM3L);
      regval &= SLCD_CHAR3_MASK;
      regval |= SLCD_CHAR3_UPDATE(segments[3]);
      putreg32(regval, STM32_LCD_RAM3L);
      break;

    case 3:
      regval = getreg32(STM32_LCD_RAM0L);
      regval &= SLCD_CHAR4_MASK;
      regval |= SLCD_CHAR4_UPDATE(segments[0]);
      putreg32(regval, STM32_LCD_RAM0L);

      regval = getreg32(STM32_LCD_RAM1L);
      regval &= SLCD_CHAR4_MASK;
      regval |= SLCD_CHAR4_UPDATE(segments[1]);
      putreg32(regval, STM32_LCD_RAM1L);

      regval = getreg32(STM32_LCD_RAM2L);
      regval &= SLCD_CHAR4_MASK;
      regval |= SLCD_CHAR4_UPDATE(segments[2]);
      putreg32(regval, STM32_LCD_RAM2L);

      regval = getreg32(STM32_LCD_RAM3L);
      regval &= SLCD_CHAR4_MASK;
      regval |= SLCD_CHAR4_UPDATE(segments[3]);
      putreg32(regval, STM32_LCD_RAM3L);
      break;

    case 4:
      regval = getreg32(STM32_LCD_RAM0L);
      regval &= SLCD_CHAR5_MASKA;
      regval |= SLCD_CHAR5_UPDATEA(segments[0]);
      putreg32(regval, STM32_LCD_RAM0L);

      regval = getreg32(STM32_LCD_RAM1L);
      regval &= SLCD_CHAR5_MASKA;
      regval |= SLCD_CHAR5_UPDATEA(segments[1]);
      putreg32(regval, STM32_LCD_RAM1L);

      regval = getreg32(STM32_LCD_RAM2L);
      regval &= SLCD_CHAR5_MASKB;
      regval |= SLCD_CHAR5_UPDATEB(segments[2]);
      putreg32(regval, STM32_LCD_RAM2L);

      regval = getreg32(STM32_LCD_RAM3L);
      regval &= SLCD_CHAR5_MASKB;
      regval |= SLCD_CHAR5_UPDATEB(segments[3]);
      putreg32(regval, STM32_LCD_RAM3L);
      break;

    case 5:
      regval = getreg32(STM32_LCD_RAM0L);
      regval &= SLCD_CHAR6_MASK;
      regval |= SLCD_CHAR6_UPDATEA(segments[0]);
      putreg32(regval, STM32_LCD_RAM0L);

      regval = getreg32(STM32_LCD_RAM1L);
      regval &= SLCD_CHAR6_MASK;
      regval |= SLCD_CHAR6_UPDATEA(segments[1]);
      putreg32(regval, STM32_LCD_RAM1L);

      regval = getreg32(STM32_LCD_RAM2L);
      regval &= SLCD_CHAR6_MASK;
      regval |= SLCD_CHAR6_UPDATEB(segments[2]);
      putreg32(regval, STM32_LCD_RAM2L);

      regval = getreg32(STM32_LCD_RAM3L);
      regval &= SLCD_CHAR6_MASK;
      regval |= SLCD_CHAR6_UPDATEB(segments[3]);
      putreg32(regval, STM32_LCD_RAM3L);
      break;

    default:
      return;
  }

  /* Set the UDR bit to transfer the updated data to the second level
   * buffer.
   */

  putreg32(1, SLCD_SR_UDR_BB);
}

/****************************************************************************
 * Name: slcd_writech
 ****************************************************************************/

static void slcd_writech(uint8_t ch, uint8_t curpos, uint8_t options)
{
  uint16_t bitset;

  /* Map the character code to a 16-bit encoded value */

  bitset = slcd_mapch(ch);

  /* Check if the character should be decorated with a decimal point or colon */

  if ((options & SCLD_DP) != 0)
    {
      bitset |= 0x0002;
    }
  else if ((options & SCLD_DP) != 0)
    {
      bitset |= 0x0020;
    }

  /* Decode the value and write it to the SLCD segment memory */

  slcd_writemem(bitset, curpos);

  /* Save these values in the state structure */

  g_slcdstate.buffer[curpos]  = ch;
  g_slcdstate.options[curpos] = options;
}

/****************************************************************************
 * Name: slcd_appendch
 ****************************************************************************/

static void slcd_appendch(uint8_t ch, uint8_t options)
{
  /* Write the character at the current cursor position */

  slcd_writech(ch, g_slcdstate.curpos, options);
  if (g_slcdstate.curpos < (SLCD_NCHARS - 1))
    {
      g_slcdstate.curpos++;
    }
}

/****************************************************************************
 * Name: slcd_action
 ****************************************************************************/

static void slcd_action(enum slcdcode_e code, uint8_t count)
{
  switch (code)
    {
      /* Erasure */

      case SLCDCODE_BACKDEL:         /* Backspace (backward delete) N characters */
        {
          /* If we are at the home position, then ignore the action */

          if (g_slcdstate.curpos < 1)
            {
              break;
            }

          /* Otherwise, BACKDEL is like moving the cursor back one then doing a
           * forward deletion.  Decrement the cursor position and fall through.
           */

           g_slcdstate.curpos--;
         }

      case SLCDCODE_FWDDEL:          /* DELete (forward delete) N characters moving text */
        {
          int i;

          /* Move all characters after the current cursor position left by one */

          for (i = g_slcdstate.curpos + 1; i < SLCD_NCHARS - 1; i++)
            {
              slcd_writech(g_slcdstate.buffer[i-1], i, g_slcdstate.options[i-1]);
            }

          /* Erase the last character on the display */

          slcd_writech(' ', SLCD_NCHARS - 1, 0);
        }
        break;

      case SLCDCODE_ERASE:           /* Erase N characters from the cursor position */
        {
          int last;
          int i;

          /* Get the last position to clear and make sure that the last
           * position is on the SLCD.
           */

          last = g_slcdstate.curpos + count - 1;
          if (last >= SLCD_NCHARS)
            {
              last = SLCD_NCHARS - 1;
            }

          /* Erase N characters after the current cursor position left by one */

          for (i = g_slcdstate.curpos; i < last; i++)
            {
              slcd_writech(' ', i, 0);
            }
        }
        break;

      case SLCDCODE_CLEAR:           /* Home the cursor and erase the entire display */
        {
          /* This is like HOME followed by ERASEEOL.  Home the cursor and
           * fall through.
           */

          g_slcdstate.curpos = 0;
        }

      case SLCDCODE_ERASEEOL:        /* Erase from the cursor position to the end of line */
        {
          int i;

          /* Erasecharacters after the current cursor position to the end of the line */

          for (i = g_slcdstate.curpos; i < SLCD_NCHARS; i++)
            {
              slcd_writech(' ', i, 0);
            }
        }
        break;

      /* Cursor movement */

      case SLCDCODE_HOME:            /* Cursor home */
        {
          g_slcdstate.curpos = 0;
        }
        break;

      case SLCDCODE_END:             /* Cursor end */
        {
          g_slcdstate.curpos = SLCD_NCHARS - 1;
        }
        break;

      case SLCDCODE_LEFT:            /* Cursor left by N characters */
        {
          /* Don't permit movement past the beginning of the SLCD */

          if (g_slcdstate.curpos > 0)
            {
              g_slcdstate.curpos--;
            }
        }
        break;

      case SLCDCODE_RIGHT:           /* Cursor right by N characters */
        {
          /* Don't permit movement past the lcd of the SLCD */

          if (g_slcdstate.curpos < (SLCD_NCHARS - 1))
            {
              g_slcdstate.curpos++;
            }
        }
        break;

      case SLCDCODE_UP:              /* Cursor up by N lines */
      case SLCDCODE_DOWN:            /* Cursor down by N lines */
      case SLCDCODE_PAGEUP:          /* Cursor up by N pages */
      case SLCDCODE_PAGEDOWN:        /* Cursor down by N pages */
        break;                       /* Not supportable on this SLCD */

      /* Blinking */

      case SLCDCODE_BLINKSTART:      /* Start blinking with current cursor position */
      case SLCDCODE_BLINKEND:        /* End blinking after the current cursor position */
      case SLCDCODE_BLINKOFF:        /* Turn blinking off */
        break;                       /* Not implemented */

      /* These are actually unreportable errors */

      default:
      case SLCDCODE_NORMAL:          /* Not a special keycode */
        break;
    }
}

/****************************************************************************
 * Name: slcd_read
 ****************************************************************************/

static ssize_t slcd_read(FAR struct file *filp, FAR char *buffer, size_t len)
{
  int ret = 0;
  int i;

  for (i = 0; i < SLCD_NCHARS && ret < len; i++)
    {
      /* Return the character */

      *buffer++ = g_slcdstate.buffer[i];
      ret++;

      /* Check if the character is decorated with a folling period or colon */

      if (ret < len && g_slcdstate.buffer[i] != 0)
        {
          if ((g_slcdstate.buffer[i] & SCLD_DP) != 0)
            {
              *buffer++ = '.';
              ret++;
            }
          else if ((g_slcdstate.buffer[i] & SCLD_COLON) != 0)
            {
              *buffer++ = ':';
              ret++;
            }
        }
    }

  return ret;
}

/****************************************************************************
 * Name: slcd_write
 ****************************************************************************/

static ssize_t slcd_write(FAR struct file *filp,
                          FAR const char *buffer, size_t len)
{
  struct slcd_instream_s instream;
  struct slcdstate_s state;
  enum slcdret_e result;
  uint8_t ch;
  uint8_t count;
  uint8_t prev = ' ';
  bool valid = false;

  /* Initialize the stream for use with the SLCD CODEC */

  instream.stream.get  = slcd_getstream;
  instream.stream.nget = 0;
  instream.buffer      = buffer;
  instream.nbytes      = len;

  /* Prime the pump */

  memset(&state, 0, sizeof(struct slcdstate_s));
  result = slcd_decode(&instream.stream, &state, &prev, &count);
  switch (result)
    {
      case SLCDRET_CHAR:
        valid = true;
        break;

      case SLCDRET_SPEC:
        {
          slcd_action((enum slcdcode_e)prev, count);
          prev = ' ';
        }
        break;

      case SLCDRET_EOF:
        return 0;
    }

  /* Now decode and process every byte in the input buffer */

  while ((result = slcd_decode(&instream.stream, &state, &ch, &count)) != SLCDRET_EOF)
    {
      if (result == SLCDRET_CHAR)          /* A normal character was returned */
        {
          /* Check for ASCII control characters */

          if (ch < ASCII_SPACE)
            {
              /* All are ignored except for backspace and carriage return */

              if (ch == ASCII_BS)
                {
                  slcd_action(SLCDCODE_BACKDEL, 1);
                }
              else if (ch == ASCII_CR)
                {
                  slcd_action(SLCDCODE_HOME, 0);
                }
            }

          /* Handle characters decoreated with a period or a colon */

          else if (ch == '.')
            {
              /* Write the previous character with the decimal point appended */

              slcd_appendch(prev, SCLD_DP);
              prev = ' ';
              valid = false;
            }
          else if (ch == ':')
            {
              /* Write the previous character with the colon appended */

              slcd_appendch(prev, SCLD_COLON);
              prev = ' ';
              valid = false;
            }

          /* Handle ASCII_DEL */

          else if (ch == ASCII_DEL)
            {
              slcd_action(SLCDCODE_FWDDEL, 1);
            }

          /* The rest of the 7-bit ASCII characters are fair game */

          else if (ch < 128)
            {
              /* Write the previous character if it valid */

              if (valid)
                {
                  slcd_appendch(prev, 0);
                }

              /* There is now a valid output character */

              prev = ch;
              valid = true;
            }
        }
      else /* (result == SLCDRET_SPEC) */  /* A special SLCD action was returned */
        {
          slcd_action((enum slcdcode_e)ch, count);
        }
    }

  /* Handle any unfinished output */

  if (valid)
    {
      slcd_appendch(prev, 0);
    }

  /* Assume that the entire input buffer was processed */

  return (ssize_t)len;
}

/****************************************************************************
 * Name: slcd_poll
 ****************************************************************************/

#ifndef CONFIG_DISABLE_POLL
static int slcd_poll(FAR struct file *filp, FAR struct pollfd *fds,
                        bool setup)
{
  if (setup)
    {
      /* Data is always avaialble to be read */

      fds->revents |= (fds->events & (POLLIN|POLLOUT));
      if (fds->revents != 0)
        {
          sem_post(fds->sem);
        }
    }

  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  stm32_slcd_initialize
 *
 * Description:
 *   Initialize the STM32L-Discovery hardware and register the character
 *   driver.
 *
 ****************************************************************************/

int stm32_slcd_initialize(void)
{
  uint32_t regval;
  int ret = OK;
  int i;

  /* Only initialize the driver once. */

  if (!g_slcdstate.initialized)
    {
      lcdvdbg("Initializing\n");

      /* Configure LCD GPIO pins */

      for (i = 0; i < BOARD_SLCD_NGPIOS; i++)
        {
          stm32_configgpio(g_slcdgpio[i]);
        }

      /* Set the LCD prescaler and divider values */

      regval = getreg32(STM32_LCD_FCR);
      regval &= ~(LCD_FCR_DIV_MASK | LCD_FCR_PS_MASK);
      regval |= ( LCD_FCR_PS_DIV1 |  LCD_FCR_DIV(31));
      putreg32(regval, STM32_LCD_FCR);

      /* Wait for the FCRSF flag to be set */

      while ((getreg32(STM32_LCD_SR) & LCD_SR_FCRSF) == 0);

      /* Set the duty (1/4), bias (1/3), and the internal voltage source (VSEL=0) */

      regval = getreg32(STM32_LCD_CR);
      regval &= ~(LCD_CR_BIAS_MASK | LCD_CR_DUTY_MASK | LCD_CR_VSEL);
      regval |= (LCD_CR_DUTY_1TO4 | LCD_CR_BIAS_1TO3);
      putreg32(regval, STM32_LCD_CR);

      /* SEG[31:28] are multiplexed with SEG[43:40] */

      regval |= LCD_CR_MUX_SEG;
      putreg32(regval, STM32_LCD_CR);

      /* Set the contrast to the mean value */

      regval  = getreg32(STM32_LCD_FCR);
      regval &= ~LCD_FCR_CC_MASK;
      regval |=  LCD_FCR_CC_VLCD(4);
      putreg32(regval, STM32_LCD_FCR);

      /* No dead time */

      regval &= ~LCD_FCR_DEAD_MASK;
      putreg32(regval, STM32_LCD_FCR);

      /* Set the pulse-on duration to 4/ck_ps */

      regval &= ~LCD_FCR_PON_MASK;
      regval |= LCD_FCR_PON(4);
      putreg32(regval, STM32_LCD_FCR);

      /* Wait Until the LCD FCR register is synchronized */

      while ((getreg32(STM32_LCD_SR) & LCD_SR_FCRSF) == 0);

      /* Enable LCD peripheral */

      putreg32(1, SLCD_CR_LCDEN_BB);

      /* Wait Until the LCD is enabled and the LCD booster is ready */

      while ((getreg32(STM32_LCD_SR) & (LCD_SR_ENS | LCD_SR_RDY)) != (LCD_SR_ENS | LCD_SR_RDY));

      /* Disable blinking */

      regval  = getreg32(STM32_LCD_FCR);
      regval &= ~(LCD_FCR_BLINKF_MASK | LCD_FCR_BLINK_MASK);
      regval |=  (LCD_FCR_BLINK_DISABLE | LCD_FCR_BLINKF_DIV32);
      putreg32(regval, STM32_LCD_FCR);

      /* Register the LCD device driver */

      ret = register_driver("/dev/slcd", &g_slcdops, 0644, (FAR struct file_operations *)&g_slcdops);
      g_slcdstate.initialized = true;

      /* Then clear the display */

      slcd_clear();
    }

  return ret;
}

#endif /* CONFIG_STM32_LCD */