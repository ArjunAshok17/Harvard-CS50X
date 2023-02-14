SELECT avg(ratings.rating) FROM ratings
    JOIN movies
        ON ratings.movie_id = movies.id
        WHERE movies.year = 2012;
    -- WHERE (SELECT year FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE year = 2012)