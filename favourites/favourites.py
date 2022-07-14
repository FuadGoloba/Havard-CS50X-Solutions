import cs50
import csv

# Creating a database and writing CSV data into it using Python

# We create a database

open("favourites.db", "w").close()

# cs50.SQL - Create instance of sqlalchemy.engine.Engine.
db = cs50.SQL("sqlite:///favourites.db")

# Creating 2 tables
# CReating a shows table with cols; id and title
db.execute("CREATE TABLE shows (id INTEGER, title TEXT NOT NULL, PRIMARY KEY(id))")

# Creating a genres table with cols; show_id, genre
db.execute("CREATE TABLE genres (show_id INTEGER, genre TEXT NOT NULL, FOREIGN KEY(show_id) REFERENCES shows(id))")

# Opening CSV file
with open("favourites.csv", 'r') as file:

    reader = csv.DictReader(file)

    # iterating over CSV file, row by row
    for row in reader:

        title = row['title'].strip().upper()

        # inserting title into the shows table in the db
        show_id = db.execute("INSERT INTO shows (title) VALUES(?)", title)

        # inserting genres
        for genre in row['genres'].split(", "):
            db.execute("INSERT INTO genres (show_id, genre) VALUES(?, ?)", show_id, genre)
