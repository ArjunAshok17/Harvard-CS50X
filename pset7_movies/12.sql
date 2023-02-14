SELECT movies.title FROM movies
    WHERE movies.id IN (SELECT DISTINCT movies.id FROM movies
                            JOIN stars
                                ON movies.id = stars.movie_id
                                AND stars.person_id = (SELECT id FROM people WHERE name = "Johnny Depp"))
        AND movies.id IN (SELECT DISTINCT movies.id FROM movies
                            JOIN stars
                                ON movies.id = stars.movie_id
                                AND stars.person_id = (SELECT id FROM people WHERE name = "Helena Bonham Carter"))
    ORDER BY title;