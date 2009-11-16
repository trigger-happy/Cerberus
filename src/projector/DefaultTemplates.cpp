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
"				<th>Rank</th><th>Name</th><th>Group</th><th>Score</th>"
"			</tr>"
"			{{#CONTESTANT_SECTION}}"
"			<tr>"
"				<td>{{RANK}}</td>"
"				<td>{{NAME}}</td><td>{{GROUP}}</td>"
"				<td>{{SCORE}}</td>"
"				<td>{{TIME}}</td>"
"			</tr>"
"			{{/CONTESTANT_SECTION}}"
"		<table>"
"	</body>"
"</html>";

const char * const DEFAULT_TEMPLATE_TIMEBOARD =
"<html>"
"	<body>"
"		<h1>{{CONTEST_NAME}} - Round {{STAGE_NUMBER}}</h1>"
"		<h2>Time: {{TIME_LEFT}}</h2>"
"	<body>"
"</html>";

const char * const DEFAULT_TEMPLATE_QDISPLAY =
"<html>"
"	<body>"
"		<h1>{{CONTEST_NAME}} - Round {{STAGE_NUMBER}}</h1>"
"		<h1>Question Number: {{QUESTION_NUMBER}} out of {{QUESTION_TOTAL}}</h1>"
"		<h2>Time: {{TIME_LEFT}}</h2>"
"		<h2>Question: {{QUESTION}}</h2>"
"		{{#ANSWER_SECTION}}<h2>Answer: {{ANSWER}}</h2>{{/ANSWER_SECTION}}"
"		<ol>"
"			{{#CHOICE_SECTION}}<li>{{CHOICE}}</li>{{/CHOICE_SECTION}}"
"		</ol>"
"	</body>"
"</html>";
