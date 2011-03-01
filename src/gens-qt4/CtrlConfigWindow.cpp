/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * CtrlConfigWindow.cpp: Controller Configuration Window.                  *
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

#include "CtrlConfigWindow.hpp"
#include "gqt4_main.hpp"

// EmuMD has the I/O devices.
#include "libgens/MD/EmuMD.hpp"

// Qt includes.
#include <QtGui/QActionGroup>

namespace GensQt4
{

// Static member initialization.
CtrlConfigWindow *CtrlConfigWindow::m_CtrlConfigWindow = NULL;

// Controller icon filenames.
const char *CtrlConfigWindow::ms_CtrlIconFilenames[LibGens::IoBase::IOT_MAX] =
{
	":/gens/controller-none.png",		// IOT_NONE
	":/gens/controller-3btn.png",		// IOT_3BTN
	":/gens/controller-6btn.png",		// IOT_6BTN
	":/gens/controller-2btn.png",		// IOT_2BTN
	":/gens/controller-mega-mouse.png"	// IOT_MEGA_MOUSE (TODO)
	":/gens/controller-teamplayer.png"	// IOT_TEAMPLAYER (TODO)
	":/gens/controller-4wp.png"		// IOT_4WP_MASTER (TODO)
	":/gens/controller-4wp.png"		// IOT_4WP_SLAVE (TODO)
};


/**
 * CtrlConfigWindow(): Initialize the Controller Configuration window.
 */
CtrlConfigWindow::CtrlConfigWindow(QWidget *parent)
	: QMainWindow(parent,
		Qt::Dialog |
		Qt::WindowTitleHint |
		Qt::WindowSystemMenuHint |
		Qt::WindowMinimizeButtonHint |
		Qt::WindowCloseButtonHint)
{
	// Initialize the Qt4 UI.
	setupUi(this);
	
	// Make sure the window is deleted on close.
	this->setAttribute(Qt::WA_DeleteOnClose, true);
	
#ifdef Q_WS_MAC
	// Remove the window icon. (Mac "proxy icon")
	this->setWindowIcon(QIcon());
#endif
	
	// Initialize the toolbar.
	m_actgrpSelPort = new QActionGroup(this);
	m_actgrpSelPort->setExclusive(true);
	m_actgrpSelPort->addAction(actionPort1);
	m_actgrpSelPort->addAction(actionPort2);
	
	// Copy the current controller settings.
	// TODO: Button mapping.
	// TODO: Load from the configuration cache instead of the emulation context.
	if (gqt4_emuContext)
	{
		// Emulation is running.
		m_devType[0] = gqt4_emuContext->m_port1->devType();
		m_devType[1] = gqt4_emuContext->m_port2->devType();
	}
	else
	{
		// Emulation is not running.
		m_devType[0] = LibGens::IoBase::IOT_NONE;
		m_devType[1] = LibGens::IoBase::IOT_NONE;
	}
	
	// Initialize all the settings.
	updatePortButton(0);
	updatePortButton(1);
	
	// Initialize the "Device" dropdown.
	// TODO: Handle Master/Slave devices.
	for (int i = LibGens::IoBase::IOT_NONE; i <= LibGens::IoBase::IOT_4WP_MASTER; i++)
	{
		cboDevice->addItem(QIcon(QString::fromLatin1(ms_CtrlIconFilenames[i])),
				GetShortDeviceName((LibGens::IoBase::IoType)i));
	}
	
	// Set Port 1 as active.
	actionPort1->setChecked(true);
}


/**
 * ~CtrlConfigWindow(): Shut down the Controller Configuration window.
 */
CtrlConfigWindow::~CtrlConfigWindow()
{
	// Clear the m_CtrlConfigWindow pointer.
	m_CtrlConfigWindow = NULL;
}


/**
 * ShowSingle(): Show a single instance of the Controller Configuration window.
 * @param parent Parent window.
 */
void CtrlConfigWindow::ShowSingle(QWidget *parent)
{
	if (m_CtrlConfigWindow != NULL)
	{
		// Controller Configuration Window is already displayed.
		// NOTE: This doesn't seem to work on KDE 4.4.2...
		QApplication::setActiveWindow(m_CtrlConfigWindow);
	}
	else
	{
		// Controller Configuration Window is not displayed.
		m_CtrlConfigWindow = new CtrlConfigWindow(parent);
		m_CtrlConfigWindow->show();
	}
}


/**
 * GetShortDeviceName(): Get the short name of an I/O device.
 * TODO: Use IoBase::devName()?
 * @param devType Device type.
 * @return Short device name.
 */
const QString CtrlConfigWindow::GetShortDeviceName(LibGens::IoBase::IoType devType)
{
	using LibGens::IoBase;
	
	switch (devType)
	{
		case IoBase::IOT_NONE:
		default:
			return tr("None");
		
		case IoBase::IOT_3BTN:		return tr("3-button");
		case IoBase::IOT_6BTN:		return tr("6-button");
		case IoBase::IOT_2BTN:		return tr("2-button");
		case IoBase::IOT_MEGA_MOUSE:	return tr("Mega Mouse");
		case IoBase::IOT_TEAMPLAYER:	return tr("Teamplayer");
		case IoBase::IOT_4WP_MASTER:	/* see below */
		case IoBase::IOT_4WP_SLAVE:	return tr("4-Way Play");
	}
}


/**
 * GetLongDeviceName(): Get the long name of an I/O device.
 * TODO: Use IoBase::devName()?
 * @param devType Device type.
 * @return Long device name.
 */
const QString CtrlConfigWindow::GetLongDeviceName(LibGens::IoBase::IoType devType)
{
	using LibGens::IoBase;
	
	switch (devType)
	{
		case IoBase::IOT_NONE:
		default:
			return tr("No device connected.");
		
		case IoBase::IOT_3BTN:		return tr("3-button gamepad");
		case IoBase::IOT_6BTN:		return tr("6-button gamepad");
		case IoBase::IOT_2BTN:		return tr("2-button gamepad (SMS)");
		case IoBase::IOT_MEGA_MOUSE:	return tr("Mega Mouse");
		case IoBase::IOT_TEAMPLAYER:	return tr("Sega Teamplayer");
		case IoBase::IOT_4WP_MASTER:	/* see below */
		case IoBase::IOT_4WP_SLAVE:	return tr("EA 4-Way Play");
	}
}


/**
 * updatePortButton(): Update a port button.
 * @param port Port ID.
 */
void CtrlConfigWindow::updatePortButton(int port)
{
	QAction *actionPort;
	switch (port)
	{
		case 0:
			actionPort = actionPort1;
			break;
		case 1:
			actionPort = actionPort2;
			break;
		default:
			return;
	}
	
	// Make sure the device type is in bounds.
	if (m_devType[port] < LibGens::IoBase::IOT_NONE ||
	    m_devType[port] >= LibGens::IoBase::IOT_MAX)
	{
		// Invalid device type.
		return;
	}
	
	// Update the port icon and tooltip.
	actionPort->setIcon(QIcon(QString::fromLatin1(ms_CtrlIconFilenames[m_devType[port]])));
	actionPort->setToolTip(GetLongDeviceName(m_devType[port]));
}


/**
 * updatePortSettings(): Update port settings.
 * @param port Port to display.
 */
void CtrlConfigWindow::updatePortSettings(int port)
{
	// TODO: Port 3 support?
	if (port < 0 || port > 1)
		return;
	
	// Update the port settings.
	// TODO: Controller keymap.
	
	// Make sure the device type is in bounds.
	if (m_devType[port] < LibGens::IoBase::IOT_NONE ||
	    m_devType[port] >= LibGens::IoBase::IOT_MAX)
	{
		// Invalid device type.
		return;
	}
	
	// Set the "Port Settings" text.
	// TODO: Port names for when e.g. EXT, J-Cart, etc. are added.
	grpPortSettings->setTitle(tr("Controller Settings: Port %1").arg(port + 1));
	
	// Set the device type in the dropdown.
	int devIndex = m_devType[port];
	if (devIndex >= LibGens::IoBase::IOT_4WP_SLAVE)
		devIndex--;	// avoid having two 4WP devices in the dropdown
	cboDevice->setCurrentIndex(devIndex);
}


/** TODO **/

void CtrlConfigWindow::accept(void)
	{ close(); }
void CtrlConfigWindow::reject(void)
	{ close(); }

void CtrlConfigWindow::reload(void) { }
void CtrlConfigWindow::apply(void) { }


/** Widget slots. **/

void CtrlConfigWindow::on_actionPort1_toggled(bool checked)
{
	if (checked)
		updatePortSettings(0);
}

void CtrlConfigWindow::on_actionPort2_toggled(bool checked)
{
	if (checked)
		updatePortSettings(1);
}

}
