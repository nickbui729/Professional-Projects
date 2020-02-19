
-- Select Pokemon and display information

SELECT name FROM Pokemon

SELECT pokemon_id FROM Pokemon WHERE name= [inputPokemonName]

SELECT species FROM Pokemon WHERE name=[inputPokemonName]

SELECT ability_name FROM ability
    INNER JOIN pokemon_ability
    ON ability.ability_id = pokemon_ability.mid
    INNER JOIN pokemon
    ON pokemon_moves.pid = pokemon.pokemon_id
    WHERE pokemon.name =[inputPokemonName]

SELECT type_name FROM type
    INNER JOIN pokemon_type
    ON type.type_id = pokemon_type.tid
    INNER JOIN pokemon
    ON pokemon_type.pid = pokemon.pokemon_id
    WHERE pokemon.name =[inputPokemonName]

-- Add a type to a Pokemon 

SELECT pokemon_id FROM pokemon
SELECT type_id FROM type
INSERT INTO pokemon_types (pid, tid) VALUES [inputPID], [inputTID]

-- Delete a Pokemon

DELETE FROM pokemon WHERE name =[inputPokemonName]

-- Add a type

INSERT INTO type (type_name) VALUES [inputPOkemonType]

-- Select an ability and learn more about it Displays ABIILTY NAME TYPE DAMAGE

SELECT ability_name FROM ability
SELECT type_name 
FROM type
    INNER JOIN ability
    ON ability.type = ability.ability_id
    WHERE ability.ability_name =[inputPokemonAbility]
SELECT damage FROM ability WHERE ability_name =[inputPokemonAbility]

-- Update an ability

SELECT ability_name FROM ability
UPDATE ability SET damage =[inputDamage]  WHERE ability_name =[inputPokemonAbility]

-- Delete an ability

DELETE FROM ability WHERE ability_name =[inputPokemonAbility]

-- Add a town

INSERT INTO town (town_name, description) VALUES [inputTownName], [inputTownDescription]

-- Add a type to a Gym Leader

SELECT gym_id FROM Gym WHERE leader_name =[inputGymleader]
SELECT type_id FROM type WHERE type_name =[inputTypeName] 
INSERT INTO Gym_Type (gym_id, type_id) VALUES [input]


