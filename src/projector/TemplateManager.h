#ifndef PROJECTOR_TEMPLATEMANAGER_H_
#define PROJECTOR_TEMPLATEMANAGER_H_

#include <QString>
#include <ctemplate/template.h>

class TemplateManager
{
public:
	enum TKey { INDEX, ERROR, SCOREBOARD, QDISPLAY, TIMEBOARD };
	TemplateManager();

	void setTemplatePath(const QString &path) {
		m_path = path;
	}

	const QString& getTemplatePath() { return m_path; }

	ctemplate::Template* getTemplate(TKey template_key);
private:
	static void initialize();
	QString m_path;
};

#endif // PROJECTOR_TEMPLATEMANAGER_H_
