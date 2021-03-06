/***************************************************************************
 * libgens: Gens Emulation Library.                                        *
 * IoPico.cpp: Sega Pico controller.                                       *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2015 by David Korth.                                 *
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

#include "IoPico.hpp"
#include "lg_osd.h"

// References:
// - http://notaz.gp2x.de/docs/picodoc.txt

// TODO: Add support for:
// - Pen position. (in progress)

namespace LibGens { namespace IO {

IoPico::IoPico()
	: Device()
	, m_page_num(0)
	, m_adj_x(0)
	, m_adj_y(0)
{
	m_type = IoManager::IOT_PICO;
	m_hasDPad = false;
}

/**
 * Reset Device data that only affects the device
 * and not the emulation-side registers.
 *
 * Should be overridden by subclasses that have
 * device-specific data.
 */
void IoPico::resetDev(void)
{
	Device::resetDev();	// TODO: typedef super?
	m_page_num = 0;		// Reset to title page.
}

/**
 * Update the I/O device.
 * Runs the internal device update.
 */
void IoPico::update(void)
{
	// NOTE: The Pico controller is input-only.
	// Update tristate settings to reflect this.
	this->ctrl = 0x00;	// input only

	// NOTE: The Pico controller is hard-wired into the system.
	// There's no DE-9 interface, so m_pin58 is ignored.

	/**
	 * Data format:
	 * - PudBRLDU
	 * B = red button
	 * d = Page Down
	 * u = Page Up
	 * P = pen button
	 *
	 * NOTE: Page control buttons are NOT visible to the
	 * system here. They're mapped as buttons for
	 * convenience purposes.
	 */
	this->deviceData = (0xFF & (this->buttons | 0x60));

	// Due to the design of the MD input subsystem,
	// the high bit is normally a latched bit.
	// Pico is input-only, so all 8 bits are input.
	// Update the latched data to match the device data.
	this->mdData = this->deviceData;

	// Check for page control buttons.
	// TODO: If both buttons are pressed, do nothing?
	if ((this->buttons_prev & 0x20) &&
	    !(this->buttons & 0x20))
	{
		// Page Down was pressed.
		if (m_page_num < (PICO_MAX_PAGES - 1)) {
			m_page_num++;
			lg_osd(OSD_PICO_PAGEDOWN, m_page_num);
		}
	}
	if ((this->buttons_prev & 0x40) &&
	    !(this->buttons & 0x40))
	{
		// Page Up was pressed.
		if (m_page_num > 0) {
			m_page_num--;
			lg_osd(OSD_PICO_PAGEUP, m_page_num);
		}
	}

	// Adjust absolute coordinates.
	if (m_abs_x < 0 || m_abs_y < 0) {
		// Offscreen.
		// TODO: What is the correct "offscreen" value?
		m_adj_x = 0;
		m_adj_y = 0;
	} else {
		// Adjust the X coordinate.
		// m_abs_x = [0, 1279]
		// m_adj_x = [0x3C, 0x17C)?
		m_adj_x = (m_abs_x / 5) + 0x3C;

		// Adjust the Y coordinate.
		// m_abs_y = [0, 479]
		// m_adj_y = [0x1FC, 0x2F7], then [0x2F8, 0x3F3]
		// Note that the vertical ranges are each 251 lines,
		// whereas the visible image is 240 lines.
		// TODO: Figure out the correct calibration.
		if (m_abs_y < 240) {
			m_adj_y = 0x1FC + m_abs_y;
		} else {
			m_adj_y = 0x2F8 + (m_abs_y - 240);
		}
	}
}

/** Pico-specific functions. **/

/**
 * Get the current page number.
 * @return Page number. (0 == title; 1-7 == regular page)
 */
uint8_t IoPico::picoCurPageNum(void) const
{
	return m_page_num & 7;
}

/**
 * Set the current page number.
 * @param pg Page number. (0 == title; 1-7 == regular page)
 */
void IoPico::setPicoCurPageNum(uint8_t pg)
{
	if (m_page_num == pg)
		return;

	if (/*pg >= 0 &&*/ pg <= PICO_MAX_PAGES) {
		m_page_num = pg;
		lg_osd(OSD_PICO_PAGESET, m_page_num);
	}
}

/**
 * Get the current page register value.
 * @return Page register value.
 */
uint8_t IoPico::picoCurPageReg(void) const
{
	static const uint8_t pg_reg[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F};
	return pg_reg[m_page_num & 7];
}

/**
 * Read a Pico I/O port related to controller input.
 * This maps to odd addresses in the range:
 * - [800003, 80000D]
 * @param address Address.
 * @param d_out Data output.
 * @return 0 on success; non-zero if address is invalid.
 */
int IoPico::picoReadIO(uint32_t address, uint8_t *d_out) const
{
	if ((address & 0xFFFFF1) != 0x800001) {
		// Invalid address.
		return -1;
	}

	int ret = 0;
	switch (address & 0xF) {
		case 0x3:
			// Buttons.
			*d_out = this->deviceData;
			break;
		case 0x5:
			// Pen X coordinate, MSB.
			*d_out = (m_adj_x >> 8);
			break;
		case 0x7:
			// Pen X coordinate, LSB.
			*d_out = (m_adj_x & 0xFF);
			break;
		case 0x9:
			// Pen Y coordinate, MSB.
			*d_out = (m_adj_y >> 8);
			break;
		case 0xB:
			// Pen Y coordinate, LSB.
			*d_out = (m_adj_y & 0xFF);
			break;
		case 0xD:
			// Page register.
			*d_out = picoCurPageReg();
			break;
		default:
			ret = -1;
			break;
	}

	return ret;
}

} }
