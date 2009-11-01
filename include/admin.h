/*
Copyright (C) 2009 James Choa

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#ifndef ADMIN_H
#define ADMIN_H
#include <QtGui>
#include "server.h"

namespace Ui {

	class server_dlg;
}

class Admin : public QDialog {
	Q_OBJECT

public:
	Admin( QWidget* parent = 0 );
	~Admin();

public slots:
	// TODO: add a bunch of slots here
	void onStopBtn();

	void onStartBtn();

	void onPauseBtn();

private:
	Ui::server_dlg* m_dlg;
	Server m_server;
};

#endif //ADMIN_H
