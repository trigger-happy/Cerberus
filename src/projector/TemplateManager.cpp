#include "TemplateManager.h"
#include <ctemplate/template.h>
#include "DefaultTemplates.h"
#include <cstring>
#include <QFileInfo>
#include <QDir>

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

ctemplate::Template* TemplateManager::getTemplate(TKey template_key) {
	using ctemplate::Template;

	QFileInfo fullPath(QDir(m_path), TEMPLATE_FILENAMES[template_key]);
	if ( fullPath.exists() ) {
		Template *ret = Template::GetTemplate(
				fullPath.canonicalFilePath().toStdString(), ctemplate::DO_NOT_STRIP);
		if ( ret )
			return ret;
	}
	return Template::GetTemplate(DEFAULT_KEYS[template_key], ctemplate::DO_NOT_STRIP);
}
