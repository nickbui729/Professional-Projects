/* DROP TABLE IF EXISTS */

DROP TABLE IF EXISTS `Pokemon`;
DROP TABLE IF EXISTS `Ability`;
DROP TABLE IF EXISTS `Type`;
DROP TABLE IF EXISTS `Gym`;
DROP TABLE IF EXISTS `Town`;
DROP TABLE IF EXISTS `Pokemon_Ability`;
DROP TABLE IF EXISTS `Ability_Type`;
DROP TABLE IF EXISTS `Gym_Type`;
DROP TABLE IF EXISTS `Pokemon_Type`;
DROP TABLE IF EXISTS `Gym_Town`;

/* CREATE TABLES */

CREATE TABLE `Pokemon` (
	`pokemon_id` int(11) NOT NULL,
	`pokemon_name` varchar(255) NOT NULL,
	`species` varchar(255),
	PRIMARY KEY (`pokemon_id`),
	UNIQUE KEY (`pokemon_name`)
) ENGINE=InnoDB;


CREATE TABLE `Ability` (
	`ability_id` int(11) NOT NULL,
	`ability_name` varchar(255) NOT NULL,
	`damage` varchar(255),
	PRIMARY KEY (`ability_id`)
) ENGINE=InnoDB;


CREATE TABLE `Type` (
	`type_id` int(11) NOT NULL,
	`type_name` varchar(255) NOT NULL,
	PRIMARY KEY (`type_id`)
) ENGINE=InnoDB;


CREATE TABLE `Gym` (
	`gym_id` int(11) NOT NULL,
	`leader_name` varchar(255) NOT NULL,
	PRIMARY KEY (`gym_id`)
) ENGINE=InnoDB;


CREATE TABLE `Town` (
	`town_id` int(11) NOT NULL,
	`town_name` varchar(255) NOT NULL,
	`description` varchar(255),
	PRIMARY KEY (`town_id`)
) ENGINE=InnoDB;


CREATE TABLE `Pokemon_Ability` (
	`pid` int(11) NOT NULL, 
	`aid` int(11) NOT NULL, 
	PRIMARY KEY (`pid`, `aid`),
	FOREIGN KEY (`pid`) REFERENCES `Pokemon` (`pokemon_id`),
	FOREIGN KEY (`aid`) REFERENCES `Ability` (`ability_id`)
) ENGINE=InnoDB;


CREATE TABLE `Ability_Type` (
	`aid` int(11) NOT NULL,
	`tid` int(11) NOT NULL, 
	PRIMARY KEY (`aid`, `tid`),
	FOREIGN KEY (`aid`) REFERENCES `Ability` (`ability_id`),
	FOREIGN KEY (`tid`) REFERENCES `Type` (`type_id`)
) ENGINE=InnoDB;


CREATE TABLE `Gym_Type` (
	`gid` int(11) NOT NULL, 
	`tid` int(11) NOT NULL,
	PRIMARY KEY (`gid`, `tid`),
	FOREIGN KEY (`gid`) REFERENCES `Gym` (`gym_id`),
	FOREIGN KEY (`tid`) REFERENCES `Type` (`type_id`)
) ENGINE=InnoDB;


CREATE TABLE `Pokemon_Type` (
	`pid` int(11) NOT NULL,
	`tid` int(11) NOT NULL,
	PRIMARY KEY (`pid`, `tid`),
	FOREIGN KEY (`pid`) REFERENCES `Pokemon` (`pokemon_id`),
	FOREIGN KEY (`tid`) REFERENCES `Type` (`type_id`)
) ENGINE=InnoDB;


CREATE TABLE `Gym_Town` (
	`gid` int(11) NOT NULL,
	`townid` int(11) NOT NULL,
	PRIMARY KEY (`gid`, `townid`),
	FOREIGN KEY (`gid`) REFERENCES `Gym` (`gym_id`),
	FOREIGN KEY (`townid`) REFERENCES `Town` (`town_id`)
) ENGINE=InnoDB;




/* SAMPLE INSERTS */

/*****POKEMON TABLE*****/

INSERT INTO Pokemon (pokemon_id, pokemon_name, species) VALUES (25, "Pikachu", "Mouse");
INSERT INTO Pokemon (pokemon_id, pokemon_name, species) VALUES (6, "Charizard", "Flame");
INSERT INTO Pokemon (pokemon_id, pokemon_name, species) VALUES (11, "Metapod", "Cocoon");

/******ABILITY TABLE*****/

INSERT INTO Ability (ability_id, ability_name, damage) VALUES (21, "Flamethrower", 100);
INSERT INTO Ability (ability_id, ability_name, damage) VALUES (2, "Growl", 0);

/******TYPE TABLE*****/

INSERT INTO Type (type_id, type_name) VALUES (1, "BUG");
INSERT INTO Type (type_id, type_name) VALUES (2, "DRAGON");
INSERT INTO Type (type_id, type_name) VALUES (3, "ICE");
INSERT INTO Type (type_id, type_name) VALUES (4, "FIGHTING");
INSERT INTO Type (type_id, type_name) VALUES (5, "FIRE");
INSERT INTO Type (type_id, type_name) VALUES (6, "FLYING");
INSERT INTO Type (type_id, type_name) VALUES (7, "GRASS");
INSERT INTO Type (type_id, type_name) VALUES (8, "GHOST");
INSERT INTO Type (type_id, type_name) VALUES (9, "GROUND");
INSERT INTO Type (type_id, type_name) VALUES (10, "ELECTRIC");
INSERT INTO Type (type_id, type_name) VALUES (11, "NORMAL");
INSERT INTO Type (type_id, type_name) VALUES (12, "POISON");
INSERT INTO Type (type_id, type_name) VALUES (13, "PSYCHIC");
INSERT INTO Type (type_id, type_name) VALUES (14, "ROCK");
INSERT INTO Type (type_id, type_name) VALUES (15, "WATER");

/******GYM TABLE*****/

INSERT INTO Gym (gym_id, leader_name) VALUES (1, "Brock");
INSERT INTO Gym (gym_id, leader_name) VALUES (2, "Misty");
INSERT INTO Gym (gym_id, leader_name) VALUES (3, "Lt. Surge");
INSERT INTO Gym (gym_id, leader_name) VALUES (4, "Erika");
INSERT INTO Gym (gym_id, leader_name) VALUES (5, "Sabrina");
INSERT INTO Gym (gym_id, leader_name) VALUES (6, "Koga");
INSERT INTO Gym (gym_id, leader_name) VALUES (7, "Blaine");
INSERT INTO Gym (gym_id, leader_name) VALUES (8, "Giovanni");

/******TOWN TABLE*****/

INSERT INTO Town (town_id, town_name, description) VALUES (1, "Pewter City", "Lies between Viridian forest and Mt. Moon.");
INSERT INTO Town (town_id, town_name, description) VALUES (2, "Cerulean City", "A seaside cty located in Northern Kanto.");
INSERT INTO Town (town_id, town_name, description) VALUES (3, "Vermillion City", "Serves as a populat port for many ships.");
INSERT INTO Town (town_id, town_name, description) VALUES (4, "Celadon City", "The most populast city in Kanta and known for its mall.");
INSERT INTO Town (town_id, town_name, description) VALUES (5, "Saffron City", "A sprawling metropolis known for its fighting and violence.");
INSERT INTO Town (town_id, town_name, description) VALUES (6, "Fuchsia City", "Home of the famous Safari Zone.");
INSERT INTO Town (town_id, town_name, description) VALUES (7, "Cinnibar", "A large island south of Pallet Town.");
INSERT INTO Town (town_id, town_name, description) VALUES (8, "Viridian City", "A small city where the Team Rocket leader resides.");

/******POKEMON_ABILITY TABLE*****/

INSERT INTO Pokemon_Ability (pid, aid) VALUES ((SELECT pokemon_id FROM Pokemon WHERE pokemon_name = "Charizard"), (SELECT ability_id FROM Ability WHERE ability_name = "Flamethrower")); 

/******ABILITY_TYPE TABLE*****/

INSERT INTO Ability_Type (aid, tid) VALUES ((SELECT ability_id FROM Ability WHERE ability_name = "Growl"), (SELECT type_id FROM Type WHERE type_name = "Normal")); 

/******GYM_TYPE TABLE*****/

INSERT INTO Gym_Type (gid, tid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Brock"), (SELECT type_id FROM Type WHERE type_name = "Rock"));


/******POKEMON_TYPE TABLE*****/

INSERT INTO Pokemon_Type (pid, tid) VALUES ((SELECT pokemon_id FROM Pokemon WHERE pokemon_name = "Metapod"), (SELECT type_id FROM Type WHERE type_name = "bug"));

/******GYM_TOWN TABLE*****/

INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Brock"), (SELECT town_id FROM Town WHERE town_name = "Pewter City"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Misty"), (SELECT town_id FROM Town WHERE town_name = "Cerulean City"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Lt. Surge"), (SELECT town_id FROM Town WHERE town_name = "Vermillion City"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Erika"), (SELECT town_id FROM Town WHERE town_name = "Celadon City"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Sabrina"), (SELECT town_id FROM Town WHERE town_name = "Saffron City"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Koga"), (SELECT town_id FROM Town WHERE town_name = "Fuchsia City"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Blaine"), (SELECT town_id FROM Town WHERE town_name = "Cinnibar"));
INSERT INTO Gym_Town (gid, townid) VALUES ((SELECT gym_id FROM Gym WHERE leader_name = "Giovanni"), (SELECT town_id FROM Town WHERE town_name = "Viridian City"));
