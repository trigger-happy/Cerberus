#include "DefaultTemplates.h"

const char * const DEFAULT_TEMPLATE_ERROR =
"<html>"
"	<body>"
"		<h1>An error has occured: {{ERROR_BRIEF}}</h1>"
"		{{ERROR_DETAIL}}"
"	</body>"
"</html>";

const char * const DEFAULT_TEMPLATE_INDEX =
"<html>"
"	<body>"
"		<h1>{{CONTEST_NAME}} by Cerberus Contest System</h1>"
"		<h2>&copy; <a href=\'http://www.compsat.org\'>CompSAt</a></h2>"
"	</body>"
"</html>";

const char * const DEFAULT_TEMPLATE_SCOREBOARD =
"<html>"
"	<body>"
"		<h1>{{CONTEST_NAME}} - Rankings</h1>"
"		<table>"
"			<tr>"
"				<th>Name</th><th>Group</th>"
"				{{#TABLE_HEADER}}<th>{{ROUND_LABEL}}</th>{{/TABLE_HEADER}}"
"			</tr>"
"			{{#CONTESTANT_SECTION}}"
"			<tr>"
"				<th>{{CONTESTANT_NAME}}</th><th>{{CONTESTANT_GROUP}}</th>"
"				{{#ROUND_SECTION}}<td>{{ROUND_SCORE}}</td>{{/ROUND_SECTION}}"
"			</tr>"
"			{{/CONTESTANT_SECTION}}"
"		<table>"
"	</body>"
"</html>";

const char * const DEFAULT_TEMPLATE_TIMEBOARD =
"<html>"
"	<body>"
"		<h1>{{CONTEST_NAME}} - Round {{ROUND_NUMBER}}</h1>"
"		<h2>Time: {{TIME_LEFT}}</h2>"
"	<body>"
"</html>";

const char * const DEFAULT_TEMPLATE_QDISPLAY =
"<html>"
"	<body>"
"		<h1>{{CONTEST_NAME}} - Round {{ROUND_NUMBER}}</h1>"
"		<h1>Question Number: {{QUESTION_NUMBER}} out of {{QUESTION_TOTAL}}</h1>"
"		<h2>Time: {{TIME_LEFT}}</h2>"
"		<h2>Question: {{QUESTION}}</h2>"
"		{{#ANSWER_SECTION}}<h2>Answer: {{ANSWER}}</h2>{{/ANSWER_SECTION}}"
"		<ol>"
"			{{#CHOICE_SECTION}}<li>{{CHOICE}}</li>{{/CHOICE_SECTION}}"
"		</ol>"
"	</body>"
"</html>";
