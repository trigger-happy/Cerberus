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

#ifndef PROJECTOR_TEMPLATEMANAGER_H_
#define PROJECTOR_TEMPLATEMANAGER_H_

#include <QString>
#include <QDir>

namespace ctemplate {
	class Template;
}

#ifndef ENTITY_INFINITY
#define ENTITY_INFINITY "&infin;"
#endif

class TemplateManager
{
public:
	enum TKey { INDEX, ERROR, SCOREBOARD, QDISPLAY, TIMEBOARD, N_TEMPLATES };
	TemplateManager();

	void setTemplatePath(const QString &path);

	QString getTemplatePath() const { return m_dir.canonicalPath(); }

	ctemplate::Template* getTemplate(TKey template_key);
private:
	static void initialize();
	QDir m_dir;
};

#endif // PROJECTOR_TEMPLATEMANAGER_H_
