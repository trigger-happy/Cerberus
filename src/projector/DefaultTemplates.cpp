#include "DefaultTemplates.h"


const char * const DEFAULT_TEMPLATE_ERROR =
"<html>"
"	<head><title>Cerberus - Error</title></head>"
"	<body>"
"		<h1>An error has occured: {{ERROR_BRIEF}}</h1>"
"		{{ERROR_DETAIL}}"
"	</body>"
"</html>";

const char * const DEFAULT_TEMPLATE_INDEX =
"<html>"
"	<head><title>Cerberus - Index</title></head>"
"	<body>"
"		<h1>Cerberus Contest System</h1>"
"		<h2>&copy; <a href=\'http://www.compsat.org\'>CompSAt</a></h2>"
"	</body>"
"</html>";

const char * const DEFAULT_TEMPLATE_SCOREBOARD =
"";

const char * const DEFAULT_TEMPLATE_TIMEBOARD =
"";

const char * const DEFAULT_TEMPLATE_QDISPLAY =
"";
