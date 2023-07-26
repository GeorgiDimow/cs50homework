SELECT title FROM movies WHERE id IN (SELECT ratings.movie_id FROM ratings INNER JOIN stars ON ratings.movie_id = stars.movie_id WHERE stars.person_id IN (SELECT id FROM people WHERE name = "Chadwick Boseman") ORDER BY rating) LIMIT 5;