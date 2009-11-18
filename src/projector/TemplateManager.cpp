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

#include "TemplateManager.h"
#include <ctemplate/template.h>
#include "DefaultTemplates.h"
#include <cstring>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

static const QString TEMPLATE_FILENAMES[] =
{ "index.html", "error.html", "scoreboard.html",
  "qdisplay.html", "timeboard.html" };

static const char * const DEFAULT_KEYS[] =
{ "default@index", "default@error", "default@scoreboard",
  "default@qdisplay", "default@timeboard"};

void TemplateManager::initialize() {
	static bool initialized = false;
	if ( initialized ) return;
	using ctemplate::Template;

	Template::StringToTemplateCache(
			DEFAULT_KEYS[INDEX],
			DEFAULT_TEMPLATE_INDEX, strlen(DEFAULT_TEMPLATE_INDEX));
	Template::StringToTemplateCache(
			DEFAULT_KEYS[ERROR],
			DEFAULT_TEMPLATE_ERROR, strlen(DEFAULT_TEMPLATE_ERROR));
	Template::StringToTemplateCache(
			DEFAULT_KEYS[SCOREBOARD],
			DEFAULT_TEMPLATE_SCOREBOARD, strlen(DEFAULT_TEMPLATE_SCOREBOARD));
	Template::StringToTemplateCache(
			DEFAULT_KEYS[QDISPLAY],
			DEFAULT_TEMPLATE_QDISPLAY, strlen(DEFAULT_TEMPLATE_QDISPLAY));
	Template::StringToTemplateCache(
			DEFAULT_KEYS[TIMEBOARD],
			DEFAULT_TEMPLATE_TIMEBOARD, strlen(DEFAULT_TEMPLATE_TIMEBOARD));

	Template::ReloadAllIfChanged();
	initialized = true;
}

TemplateManager::TemplateManager()
{
	initialize();
}

//#define DEBUG_TEMPLATE_RESOLUTION

ctemplate::Template* TemplateManager::getTemplate(TKey template_key) {
	using ctemplate::Template;

	QFileInfo fullPath(QDir(m_path), TEMPLATE_FILENAMES[template_key]);
#ifdef DEBUG_TEMPLATE_RESOLUTION
	qDebug() << "Requesting for template: " << fullPath.absoluteFilePath();
#endif
	if ( fullPath.exists() ) {
#ifdef DEBUG_TEMPLATE_RESOLUTION
		qDebug() << "\tSuccess";
#endif
		Template *ret = Template::GetTemplate(
				fullPath.canonicalFilePath().toStdString(), ctemplate::DO_NOT_STRIP);
		if ( ret )
			return ret;
	}
#ifdef DEBUG_TEMPLATE_RESOLUTION
	qDebug() << "\tNot found, resorting to default.";
#endif
	return Template::GetTemplate(DEFAULT_KEYS[template_key], ctemplate::DO_NOT_STRIP);
}
