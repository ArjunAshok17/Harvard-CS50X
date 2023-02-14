SELECT DISTINCT people.name FROM movies
    JOIN stars
        ON stars.movie_id IN (SELECT movies.id FROM movies
                                JOIN stars
                                    ON movies.id = stars.movie_id
                                        AND stars.person_id = (SELECT id FROM people where name = "Kevin Bacon" AND birth = 1958))
        -- AND NOT stars.person_id = (SELECT id FROM people where name = "Kevin Bacon" AND birth = 1958)
    JOIN people
        ON stars.person_id = people.id
            AND NOT (people.name = "Kevin Bacon" AND birth = 1958)
    ORDER BY people.name;