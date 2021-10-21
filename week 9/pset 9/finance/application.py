import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
import requests

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

app.jinja_env.globals.update(usd=usd)


@app.route("/")
@login_required
def index():
    userCash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
    stocks = db.execute("SELECT symbol, SUM(shares) as shares, operation FROM stocks WHERE userID = ? GROUP BY symbol HAVING (SUM(shares)) > 0;",
                        session["user_id"])

    total_cash_stocks = 0
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["total"] = stock["price"] * stock["shares"]
        total_cash_stocks = total_cash_stocks + stock["total"]

    total_cash_stocks += userCash[0]["cash"]
    return render_template("index.html", cash=float(userCash[0]['cash']), shares=stocks, total=total_cash_stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        data = lookup(symbol)
        shares = request.form.get("shares")
        userCash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]

        if not symbol or not data:
            return apology("Invalid Symbol")

        try:
            int(shares)
            if not shares or int(shares) <= 0:
                return apology("Invalid number of shares")
        except ValueError:
            return apology("Invalid number of shares")

        if int(shares) * data['price'] > userCash:
            return apology("Not enough cash")

        db.execute("UPDATE users SET cash= cash - ? WHERE id=?",
                   float(int(shares) * data['price']),
                   session["user_id"])

        db.execute("INSERT INTO stocks (userID, symbol, shares, price, operation) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"],
                   symbol.upper(),
                   shares,
                   data["price"],
                   "buy")

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"],
                   data['symbol'].upper(),
                   "+" + str(shares),
                   data['price'],
                   datetime.datetime.now())

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userId = session["user_id"]
    userTransactions = db.execute("SELECT * FROM transactions WHERE user_id=?", userId)
    return render_template("history.html", shares=userTransactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

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

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        data = lookup(request.form.get("symbol"))
        if not data:
            return apology("Invalid quote")
        return render_template("quoted.html", name=data['name'], symbol=data['symbol'], price=usd(data['price']))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")

    return apology("Invalid quote")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # validate username
        username = request.form.get("username")
        rows = db.execute("SELECT username FROM users")
        for row in rows:
            if username == row['username']:
                return apology("Username already exists")
        if not username:
            return apology("Invalid username")

        # validate password
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not password or not confirmation or password != confirmation:
            return apology("Invalid Password")

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

    return render_template("login.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "POST":
        selection = request.form.get("symbol")
        shares = request.form.get("shares")

        try:
            int(shares)
            if not shares or int(shares) <= 0:
                return apology("Invalid number of shares")
        except ValueError:
            return apology("Invalid number of shares")

        if not selection:
            return apology("Invalid Selection")

        stocks = db.execute(
            "SELECT SUM(shares) as shares FROM stocks WHERE userID = ? AND symbol = ?;",
            session["user_id"],
            selection)[0]

        if float(shares) > stocks["shares"]:
            return apology("You don't have enough shares")

        price = lookup(selection)["price"]
        total = price * int(shares)

        db.execute("INSERT INTO stocks (userID, symbol, shares, price, operation) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"],
                   selection.upper(),
                   -int(shares),
                   price,
                   "sell")

        db.execute("UPDATE users SET cash= cash + ? WHERE id=?", total, session["user_id"])

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, date) VALUES (?, ?, ?, ?, ?)",
                   session["user_id"], selection.upper(), "-" + str(shares), price, datetime.datetime.now())

        return redirect("/")

    else:
        stocks = db.execute("SELECT symbol FROM stocks WHERE userID = ? GROUP BY symbol",
                            session["user_id"])

        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
