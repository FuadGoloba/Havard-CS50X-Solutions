from cs50 import SQL

# Searches for popularity of a show title from a user input

db = SQL("sqlite:///favourites2.db")

title = input('Title of show: ').strip()

#searching for titles; rows will return a list of dictionary
# mapping each row to it;s col
rows = db.execute('SELECT COUNT(*) AS counter FROM favourites where title LIKE ?', title)

# Get first and only row; note it returns a list
row = rows[0]
print(rows)

print(row["counter"])


#
#for row in rows:
#    print(row['title'])
