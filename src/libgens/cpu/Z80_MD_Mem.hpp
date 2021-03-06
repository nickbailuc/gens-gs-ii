/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * Z80_MD_Mem.hpp: Z80 memory handler. (Mega Drive mode)                   *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2010 by David Korth.                                 *
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

#ifndef __LIBGENS_CPU_Z80_MEM_HPP__
#define __LIBGENS_CPU_Z80_MEM_HPP__

#include <stdint.h>

// NOTE: mdZ80 uses the FASTCALL calling convention.
#include "macros/fastcall.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO: mdZ80 accesses Ram_Z80 directly.
// Move Ram_Z80 back to Z80_MD_Mem once mdZ80 is updated.
extern uint8_t Ram_Z80[8 * 1024];

#ifdef __cplusplus
}
#endif

namespace LibGens
{

class Z80_MD_Mem
{
	public:
		static void Init(void);
		static void End(void);

#if 0
		// TODO: mdZ80 accesses Ram_Z80 directly.
		// Move Ram_Z80 back to Z80_MD_Mem once mdZ80 is updated.
		static uint8_t Ram_Z80[8 * 1024];
#endif
		
		// M68K ROM banking address.
		static int Bank_Z80;

		/** Public read/write functions. **/
		// TODO: Make these inline!
		static uint8_t FASTCALL Z80_ReadB(uint32_t address);
		static void FASTCALL Z80_WriteB(uint32_t address, uint8_t data);

	private:
		/** Z80 read/write functions. **/
		typedef uint8_t (FASTCALL *Z80_ReadB_fn) (uint32_t address);
		typedef void    (FASTCALL *Z80_WriteB_fn)(uint32_t address, uint8_t data);

		/** Read Byte functions. **/
		static uint8_t Z80_ReadB_YM2612(uint32_t address);
		static uint8_t Z80_ReadB_VDP(uint32_t address);
		static uint8_t Z80_ReadB_68K_Rom(uint32_t address);

		/** Write Byte functions. **/
		static void Z80_WriteB_Bank(uint32_t address, uint8_t data);
		static void Z80_WriteB_YM2612(uint32_t address, uint8_t data);
		static void Z80_WriteB_VDP(uint32_t address, uint8_t data);
		static void Z80_WriteB_68K_Rom(uint32_t address, uint8_t data);
};

}

#endif /* __LIBGENS_CPU_Z80_MEM_HPP__ */
