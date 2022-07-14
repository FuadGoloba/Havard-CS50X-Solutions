import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
#import re
#import string

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)


# Note- Response Caching reduces the number of requests a client or proxy makes to a web server
# Also reduces the amount of work the web server performs to generate a response
# Ensure templates are auto-reloaded - Disabling caching of responses;
# The idea here is to allow my browser notice changes I make to the code space without manually reloading the webpage
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configuring jinja with a Custom filter, usd, a function (defined in helpers.py) that makes it easier to format values as USDollars
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
# Configuring Flask to store sessions on local filesystem as opposed to storing them inside of (digitally signed) cookies which is Flask's default.
app.config["SESSION_PERMANENT"] = False
#app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required  # A decorated function defined in helpers.py ensuring that if a user tries to visit the above route,
#                # They'll first be redirected to login so as to login.
def index():
    """Show portfolio of stocks"""

    # Grouping the user's purchase record by company and price to have the total cost and total shares
    portfolio = db.execute(
        "SELECT *, SUM(shares) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])

    # Getting the user's cash balance
    cash = (db.execute
    ("SELECT Cash from users where id = ?", session["user_id"]))[0]["cash"] # Getting the user's cash balance

    #Calculating cost per stock(price * share), Grand Total, and getting current price to display to user
    total_cost = 0
    price = {}
    cost = {}
    for stock in portfolio:
        price[stock["symbol"]] = lookup(stock["symbol"])["price"]
        cost[stock["symbol"]] = price[stock["symbol"]] * float(stock["total_shares"])

        total_cost += cost[stock["symbol"]]
    grand_total = cash + total_cost

    return render_template("index.html", portfolio=portfolio, cost=cost, price=price, balance=cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        cash = (db.execute("SELECT Cash from users where id = ?", session["user_id"]))[0]["cash"] # Getting the actual amount of the user's cash
        symbol = request.form.get("symbol").upper() # we get the symbol

        if stock == None:
            return apology("Invalid Symbol", 400)

        # Handling fractional, negative and non-numeric shares
        try:
            shares = int(request.form.get("shares")) # we get the user's shares input
        except ValueError:
            return apology("Shares must be a positive Integer",400)

        tran_type = "Purchase"
        stock = lookup(symbol)
        company = lookup(symbol)["name"] # we get the company's name
        price = lookup(symbol)["price"] # we get the company's stock price for that day
        cost = int(shares) * price # We calculate the cost of buying the stock
        balance = cash - cost # we calculte the user's balance after buying the stock

        # Check if the user inputs a symbol
        if not symbol:
            return apology("Missing Symbol", 400)

        # ENsure amount of shares is valid
        elif int(shares) <= 0:
            return apology("Invalid shares amount", 400)

        # check if the symbol exists
        elif lookup(symbol) == None:
            return apology("Invalid Symbol", 400)

        # check if the user inputs shares amount
        elif not request.form.get("shares"):
            return apology("Missing Shares", 400)

        # check if the user has enough cash to purchase
        elif cash < cost:
            return apology("Not enough to purchase", 403)

        # Create table to store info on user's purchase
        # CREATE TABLE transactions (user_id INTEGER, symbol TEXT NOT NULL, company TEXT NOT NULL, shares INTEGER NOT NULL, price DECIMAL(10,2) NOT NULL, tran_type TEXT, date DATETIME DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY(user_id) REFERENCES users(id));
        # Track user's transaction into the the new table in the db
        db.execute(
            "INSERT INTO transactions (user_id, symbol, company, shares, price, tran_type) VALUES(?,?,?,?,?,?)", session["user_id"],symbol,company,shares,price,tran_type)

        # Update user's cash amount in the users table of the db
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])

        # Redirect to the home page
        flash("Stock Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")

@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    """ Change Password """

    if request.method == "POST":

        # Query database for username
        rows = db.execute("SELECT * FROM users where id = ?", session['user_id'])

        # Ensure username exists and old password is correct (i.e if the length of rows is not equal to 1, then username does not exist)
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            return apology("invalid password", 403)

        # Ensures that the new password and the old password are not same
        elif request.form.get("new_password") != request.form.get("confirmation"):
            return apology("New passwords do not match", 403)

        # Ensure new password entered 2xce matches correctly
        elif request.form.get("old_password") == request.form.get("new_password"):
            return apology("New password must be different", 403)

        # Update database with password chane
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(request.form.get("new_password")), session['user_id'])

        flash("Password changed successfully!")
        return redirect("/")

    else:
        return render_template("profile.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query all transactions and display as history
    transactions = db.execute(
        "SELECT * FROM transactions where user_id = ?", session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # When redirected to the login page, first forgets the previous user_id
    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username; rows is a list of dict;
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct (i.e if the length of rows is not equal to 1, then username does not exist)
        # Also uses check_password_hash to check the password entered with the hashed password stored in the db
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in by storing their user_id in session
        # When the user's deets are correct, we store the user_id in session
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged in") # flashes a message
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    # User's session is still on and gets to this route by redirect
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id when logged out.
    session.clear()

    # Redirect user to home/index page
    return redirect("/")


# Allowing user to look up a stock's current price
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If the user enters a symbol in the input text field
    if request.method == "POST":

        # We get the value that the user inputted
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol) # Use the lookup function(from helpers.py) to get the stock quote for the company
        # and returns a dictionary of {'name':'company name', 'symbol':symbol, 'price':'stock price'}

        if not symbol:
            return apology("Blank Symbol", 400)

        elif stock == None:
            return apology("Invalid Symbol", 400)

        return render_template("quoted.html", stock=stock)

    else: # if the request is not a post(i.e get), show the page where the user enters the company symbol to get the quote of stock price
        return render_template("quote.html")


# Create a Register route to allow user to register for an account via a form
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # Check if the user's input is blank or the username already exists
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        #usernames = [row['username'] for row in rows]

        # Ensures the user provides a username
        if not request.form.get("username"):
            return apology("must provide a username", 400)

        # Ensures the username does not exist in the database already
        elif request.form.get("username") in [row['username'] for row in rows]:
            return apology("username already exists", 400)

        # Ensures that a password is provided
        elif not request.form.get("password") or not request.form.get("confirmation"):
            return apology("must provide a password", 400)

        # Ensures that 2 passwords entry matches correctly
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # Register the new user into the DB, storing the username and hash of the user's password
        db.execute("INSERT INTO users (username, hash) VALUES(?,?)", request.form.get("username"), generate_password_hash(request.form.get("password")))
        flash("Registered Successfully!")
        return redirect("/") # redirect the user to the homepage after registering

    else: #if the request method is not a post, show the register page
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    portfolio = db.execute("SELECT *, SUM(shares) AS total_shares FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])

    if request.method == "POST":

        cash = (db.execute("SELECT Cash from users where id = ?", session["user_id"]))[0]["cash"]
        symbol = request.form.get("symbol")
        # Getting a user's particular stock
        stock = db.execute("SELECT *, SUM(shares) AS total_shares FROM transactions WHERE user_id = ? and symbol = ? GROUP BY symbol", session["user_id"], symbol)
        company = stock[0]["company"]
        shares = (-1) * float((request.form.get("shares")))
        price = lookup(symbol)["price"] # Selling at current market price (not price bought at)
        cost = abs(shares) * price
        balance = cash + cost
        tran_type = "Sale"

        # Check that the user selects a symbol in the dropdown
        if not symbol:
            return apology("Invalid stock", 403)

        # Check that the user has enough shares of the selected stock to sell
        if stock[0]["total_shares"] <= 0 or abs(shares) > stock[0]["total_shares"]:
            return apology("Not enough shares", 400)

        # Update the database based on the sale of stock
        db.execute("INSERT INTO transactions (user_id, symbol, company, shares, price, tran_type) VALUES(?,?,?,?,?,?)", session["user_id"],symbol,company,shares,price,tran_type)

        # Update user's cash amount in the users table of the db based on the sale
        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, session["user_id"])

        flash("Stock sold!")
        return redirect("/")

    else:
        return render_template("sell.html", portfolio=portfolio)







