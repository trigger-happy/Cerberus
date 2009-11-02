/*
Copyright (C) 2009 Wilhansen Li

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

#include "AuthorController.h"
#include "ProjectorWindow.h"
#include <QKeyEvent>

AuthorController::AuthorController(ProjectorWindow &target) :
		ProjectorController(target) {
}

static int properMod(int a, int m) {
	return (a % m + m) % m;
}

bool AuthorController::keyReleaseEvent(QKeyEvent *event) {
	switch ( event->key() ) {
		case Qt::Key_F5:
			m_target.refresh();
			break;
		case Qt::Key_Left:
			m_target.setView(
					TemplateManager::TKey(
							((int)m_target.getView() + 1) % TemplateManager::N_TEMPLATES));
			break;
		case Qt::Key_Right:
				m_target.setView(
				TemplateManager::TKey(
						properMod((int)m_target.getView() - 1,
								  (int)TemplateManager::N_TEMPLATES)));
			break;
		default:
			return false;
	}
	return true;
}
