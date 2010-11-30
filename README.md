Cerberus
========

Cerberus is a networked quiz-bee system in [Qt][qt]/C++.

The project is under the supervision of the [Computer Society of Ateneo][compsat].

[compsat]: http://compsat.org
[qt]: http://qt.nokia.com

Build Requirements
------------------
The following is needed to build Cerberus:

 * Qt Framework 4.6 - For some reason the system is unstable in Qt 4.7 due to some networking issues, we're currently investigating this. 
 * C++ Compiler - Qt SDK for Windows already provides this.

Components
----------
The system is split into multiple specialized programs in spirit of the UNIX development philosophy.

Main Components:

 * server: The main daemon of the system.
 * client: Client program to be installed in each computer of the contestant.
 * projector: Displays contest status, and other information. Used for projectors in the contest.
 * admin: Administrator interaface to the server.

Utilities:

 * question\_editor
 * config\_editor
 * registration\_app
 * user\_manager
