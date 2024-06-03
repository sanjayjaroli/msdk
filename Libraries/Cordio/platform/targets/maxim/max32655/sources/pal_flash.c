/*************************************************************************************************/
/*!
 * \file
 *
 * \brief      Flash driver implementation.
 *
 * Uses pages of internal flash for the databse.
 *
 * Copyright (c) 2019-2020 Packetcraft, Inc.  All rights reserved.
 * Packetcraft, Inc. confidential and proprietary.
 *
 * IMPORTANT.  Your use of this file is governed by a Software License Agreement
 * ("Agreement") that must be accepted in order to download or otherwise receive a
 * copy of this file.  You may not use or copy this file for any purpose other than
 * as described in the Agreement.  If you do not agree to all of the terms of the
 * Agreement do not use this file and delete all copies in your possession or control;
 * if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 * to any use, copying or further distribution of this software.
 *
 * Copyright (c) 2022-2023 Analog Devices, Inc.
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
 */
/*************************************************************************************************/

#include <string.h>
#include "pal_flash.h"
#include "pal_sys.h"
#include "wsf_assert.h"
#include "wsf_cs.h"
#include "mxc_device.h"
#include "flc.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Minimux flash write size if 4 bytes */
#define PAL_NVM_WORD_SIZE                       4

/*! Aligns a value to word size. */
#define PAL_NVM_WORD_ALIGN(value)               (((value) + (PAL_NVM_WORD_SIZE - 1)) & \
                                                        ~(PAL_NVM_WORD_SIZE - 1))
/*! Validates if a value is aligned to word. */
#define PAL_NVM_IS_WORD_ALIGNED(value)          (((uint32_t)(value) & \
                                                        (PAL_NVM_WORD_SIZE - 1)) == 0)

/*! Validates if a value is aligned to sector. */
#define PAL_NVM_IS_SECTOR_ALIGNED(value)        (((uint32_t)(value) & \
                                                        (MXC_FLASH_PAGE_SIZE - 1)) == 0)

extern uint32_t __pal_nvm_db_start__, __pal_nvm_db_end__;


/**************************************************************************************************
  Functions: Initialization
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the platform flash.
 *
 *  \param[in] actCback     Callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalFlashInit(PalFlashCback_t actCback)
{
  (void)actCback;
}

/*************************************************************************************************/
/*!
 *  \brief  De-initialize the platform flash.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalFlashDeInit(void)
{

}

/**************************************************************************************************
  Functions: Control and Status
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief     Get NVM state.
 *
 *  \return    NVM state.
 */
/*************************************************************************************************/
PalFlashState_t PalNvmGetState(void)
{
  if(&__pal_nvm_db_start__ == &__pal_nvm_db_end__) {
    return PAL_FLASH_STATE_UNINIT;
  }

  return PAL_FLASH_STATE_READY;
}

/*************************************************************************************************/
/*!
 *  \brief     Get NVM size.
 *
 *  \return    NVM size in bytes.
 */
/*************************************************************************************************/
uint32_t PalNvmGetTotalSize(void)
{
  return (uint32_t)&__pal_nvm_db_end__ - (uint32_t)&__pal_nvm_db_start__;
}

/*************************************************************************************************/
/*!
 *  \brief     Get NVM sector size.
 *
 *  \return    NVM sector size in bytes.
 */
/*************************************************************************************************/
uint32_t PalNvmGetSectorSize(void)
{
  return MXC_FLASH_PAGE_SIZE;
}

/**************************************************************************************************
  Functions: Data Transfer
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief     Reads data from NVM storage.
 *
 *  \param[in] pBuf     Pointer to memory buffer where data will be stored.
 *  \param[in] size     Data size in bytes to be read.
 *  \param[in] srcAddr  Word aligned address from where data is read.
 *
 *  \return    None.
 */
/*************************************************************************************************/
void PalFlashRead(void *pBuf, uint32_t size, uint32_t srcAddr)
{
  if(PalNvmGetState() != PAL_FLASH_STATE_READY) {
    /* Fill the buffer with erased flash data */
    memset(pBuf, 0xFF, size);
    return;
  }

  /* Offset the address into flash */
  srcAddr += (uint32_t)&__pal_nvm_db_start__;

  uint32_t *src = (uint32_t*)srcAddr;
  memcpy(pBuf, src, size);

}

/*************************************************************************************************/
/*!
 *  \brief     Writes data to NVM storage.
 *
 *  \param[in] pBuf     Pointer to memory buffer from where data will be written.
 *  \param[in] size     Data size in bytes to be written.
 *  \param[in] dstAddr  Word aligned address to write data.
 *
 *  \return    None.
 */
/*************************************************************************************************/
void PalFlashWrite(void *pBuf, uint32_t size, uint32_t dstAddr)
{
  if(PalNvmGetState() != PAL_FLASH_STATE_READY) {
    return;
  }

  if(!PAL_NVM_IS_WORD_ALIGNED(dstAddr)) {
    WSF_ASSERT(FALSE);
    return;
  }

  /* Offset the address into flash */
  dstAddr += (uint32_t)&__pal_nvm_db_start__;
  WsfCsEnter();
  MXC_FLC_Write(dstAddr, size, pBuf);
  WsfCsExit();

}

/*************************************************************************************************/
/*!
 *  \brief  Erase sector.
 *
 *  \param[in] size       Data size in sectors to be erased.
 *  \param[in] startAddr  Word aligned address.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalFlashEraseSector(uint32_t size, uint32_t startAddr)
{
  if(!PAL_NVM_IS_SECTOR_ALIGNED(startAddr)) {
    PalSysAssertTrap();
  }

  /* Offset the address into flash */
#if defined (__GNUC__)	
  startAddr += (uint32_t)&__pal_nvm_db_start__;
#elif defined (__CC_ARM)
  startAddr += (uint32_t)__pal_nvm_db_start__;	
#elif defined (__ICCARM__)
  startAddr += (uint32_t)__pal_nvm_db_start__;	
#endif 

  while(size) {
    WsfCsEnter();
    MXC_FLC_PageErase(startAddr);
    WsfCsExit();

    startAddr += MXC_FLASH_PAGE_SIZE;
    size --;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Erase all of the NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalFlashEraseChip(void)
{
  uint32_t startAddr, size;

#if defined (__GNUC__)
  /* Offset the address into flash */
  startAddr = (uint32_t)&__pal_nvm_db_start__;
  size = (uint32_t)&__pal_nvm_db_end__ - (uint32_t)&__pal_nvm_db_start__;
#elif defined (__CC_ARM)
  /* Offset the address into flash */
  startAddr = (uint32_t)__pal_nvm_db_start__;
  size = (uint32_t)__pal_nvm_db_end__ - (uint32_t)__pal_nvm_db_start__;
#elif defined (__ICCARM__)
  /* Offset the address into flash */
  startAddr = (uint32_t)__pal_nvm_db_start__;
  size = (uint32_t)__pal_nvm_db_end__ - (uint32_t)__pal_nvm_db_start__ -1;
#endif 

  while(size) {
    WsfCsEnter();
    MXC_FLC_PageErase(startAddr);
    WsfCsExit();

    startAddr += MXC_FLASH_PAGE_SIZE;
    size -= MXC_FLASH_PAGE_SIZE;
  }
}
