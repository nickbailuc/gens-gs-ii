/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * gqt4_main.hpp: Main UI code.                                            *
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

#ifndef __GENS_QT4_GQT4_MAIN_HPP__
#define __GENS_QT4_GQT4_MAIN_HPP__

// LibGens includes.
#include "libgens/macros/common.h"
#include "libgens/lg_osd.h"

#ifndef _WIN32
#define gens_main main
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * LOG_MSG() critical error handler.
 * @param channel Debug channel. (ASCII)
 * @param msg Message. (Preformatted UTF-8)
 */
void gqt4_log_msg_critical(const char *channel, const char *msg);

/**
 * LibGens OSD handler.
 * @param osd_type: OSD type.
 * @param param: Integer parameter.
 */
void gqt4_osd(OsdType osd_type, int param);

/**
 * Main entry point.
 * @param argc Number of arguments.
 * @param argv Array of arguments.
 * @return Return value.
 */
int gens_main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#include "Config/ConfigStore.hpp"

#include "EmuThread.hpp"
#include "libgens/EmuContext/EmuContext.hpp"

class QTranslator;

namespace GensQt4
{
	// Class forward declarations.
	class GensQApplication;

	// GensQApplication.
	extern GensQApplication *gqt4_app;

	// Configuration store.
	extern ConfigStore *gqt4_cfg;

	// Emulation objects.
	// TODO: Move the EmuContext to the EmuThread later.
	extern EmuThread *gqt4_emuThread;		// Thread.
	extern LibGens::EmuContext *gqt4_emuContext;	// Context.

	// Qt translators.
	extern QTranslator *gqt4_qtTranslator;		// Qt base translator.
	extern QTranslator *gqt4_gensTranslator;	// Gens translator.

	void QuitGens(void);
}

#endif /* __GENS_QT4_GQT4_MAIN_HPP__ */
