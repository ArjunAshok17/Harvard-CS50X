-- SELECT DISTINCT people.name FROM people
--     JOIN stars
--     ON people.id = stars.person_id
--     JOIN movies
--     ON stars.movie_id = (SELECT id FROM movies where title = "Toy Story")

SELECT DISTINCT people.name FROM stars
    JOIN movies
    ON stars.movie_id = (SELECT id FROM movies where title = "Toy Story")
    JOIN people
    ON stars.person_id = people.id