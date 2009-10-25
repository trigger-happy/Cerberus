BEGIN TRANSACTION;
CREATE TABLE team(team_name varchar(30) NOT NULL, school varchar(30), PRIMARY KEY(team_name));
/*INSERT INTO "team" VALUES('team pacquiao','notre dame');*/

CREATE TABLE user(username varchar(30) NOT NULL, team_name varchar(30) NOT NULL, firstname varchar(30) NOT NULL, lastname varchar(30) NOT NULL, password varchar(30) NOT NULL, PRIMARY KEY(username), FOREIGN KEY(team_name) references team(team_name) );
/*INSERT INTO "user" VALUES('mynameismanny','team pacquiao','manuel','pacquiao','money');*/

CREATE TABLE scores(username varchar(30) NOT NULL, score double DEFAULT 0.0, FOREIGN KEY(username) references user(username));
/*INSERT INTO "scores" VALUES('mynameismanny',0.0);*/

CREATE TABLE admin(username varchar(30) NOT NULL, password varchar(30) NOT NULL, PRIMARY KEY(username));
/*INSERT INTO "admin" VALUES('admin1', 'admin1');*/

COMMIT;
