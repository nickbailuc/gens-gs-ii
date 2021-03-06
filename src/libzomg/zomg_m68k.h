/***************************************************************************
 * libzomg: Zipped Original Memory from Genesis.                           *
 * zomg_m68k.h: ZOMG save definitions for the Motorola 68000 emulator.     *
 *                                                                         *
 * Copyright (c) 2008-2010 by David Korth                                  *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

#ifndef __LIBGENS_SAVE_ZOMG_ZOMG_M68K_H__
#define __LIBGENS_SAVE_ZOMG_ZOMG_M68K_H__

#include "zomg_common.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// MC68000 memory save struct.
// NOTE: Byteswapping is done in Zomg.cpp when saving/loading.
#pragma pack(1)
typedef struct PACKED _Zomg_M68KMemSave_t
{
	uint16_t mem[32768];	// 16-bit BE: M68K memory.
} Zomg_M68KMemSave_t;

// MC68000 register save struct.
// NOTE: Byteswapping is done in Zomg.cpp when saving/loading.
#pragma pack(1)
typedef struct PACKED _Zomg_M68KRegSave_t
{
	uint32_t dreg[8];	// 32-bit BE: Data registers.
	uint32_t areg[7];	// 32-bit BE: Address registers. (a0-a6)
	uint32_t ssp;		// 32-bit BE: Supervisor stack pointer. (a7/super)
	uint32_t usp;		// 32-bit BE: User stack pointer. (a7/user)
	uint32_t pc;		// 32-bit BE: Program counter.
	uint16_t sr;		// 16-bit BE: Status register.
	uint16_t reserved1;	// 16-bit BE: Reserved.
	uint32_t reserved2;	// 32-bit BE: Reserved.
} Zomg_M68KRegSave_t;
#pragma pack()

// MC68000 register save struct. (OLD VERSION; pre-c43510fe)
// NOTE: Byteswapping is done in Zomg.cpp when saving/loading.
#pragma pack(1)
typedef struct PACKED _Zomg_M68KRegSave_Old_t
{
	uint32_t areg[8];	// 32-bit BE: Address registers.
	uint32_t dreg[8];	// 32-bit BE: Data registers.
	uint32_t pc;		// 32-bit BE: Program counter.
	uint32_t asp;		// 32-bit BE: Alternate stack pointer.
	uint16_t sr;		// 16-bit BE: Status register.
} Zomg_M68KRegSave_Old_t;
#pragma pack()

#ifdef __cplusplus
}
#endif

#endif /* __LIBGENS_SAVE_ZOMG_ZOMG_M68K_H__ */
