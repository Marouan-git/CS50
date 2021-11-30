import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime

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
app.config["SESSION_PERMANENT"] = True
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    """Get list of symbols owned"""
    symbols_dict = db.execute("SELECT DISTINCT symbol FROM purchases WHERE user_id = ?", user_id)
    symbols = []
    for sym in symbols_dict:
        symbols.append(sym["symbol"])

    """Get dict of shares for each symbol"""
    shares_dict = {}
    total = 0
    for symbol in symbols:
        shares = int(db.execute("SELECT SUM(shares) AS sum FROM purchases WHERE symbol = ? and user_id = ?", symbol, user_id)[0]["sum"])
        shares_dict[symbol] = shares
        total = total + shares * lookup(symbol)["price"]

    """Get the cash of the current user"""
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    total = total + cash


    return render_template("index.html", symbols=symbols, shares_dict=shares_dict, lookup=lookup, int=int, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        if not symbol or lookup(symbol) is None:
            apology("Invalid symbol", 400)
        if not isinstance(shares, int):
            apology("Shares must be int!", 400)
        if shares < 0:
            apology("Shares must be positive!", 400)
        price = lookup(symbol)["price"]
        cash = db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])
        """Check if can afford"""
        if shares * price > int(cash[0]["cash"]):
            return apology("Can't afford!")
        new_cash = cash[0]["cash"] - price * shares
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])
        username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])[0]["username"]
        now = datetime.now()
        date_time = now.strftime("%d/%m/%Y, %H:%M:%S")
        db.execute("INSERT INTO purchases (user_id, username, symbol, shares, date, price) VALUES(?,?,?,?,?,?)", session["user_id"], username, symbol, shares, date_time, price)

        """Update stocks table
        symbols = db.execute("SELECT symbol FROM stocks WHERE user_id = ?", session["user_id"])
        boolean = False
        for sym in symbols:
            if symbol == sym["symbol"]:
                boolean = True
        if boolean == True:
            db.execute("UPDATE stocks SET shares = ? WHERE user_id = ? AND symbol = ?", shares, session["user_id"], symbol)
        else:
            db.execute("INSERT INTO stocks (")
        """
        return redirect("/")
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    purchases = db.execute("SELECT * FROM purchases WHERE user_id = ?", user_id)
    return render_template("history.html", purchases=purchases)


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

    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        if lookup(symbol) is None or not symbol:
            return apology("Invalid symbol", 400)
        symbol_name = lookup(symbol)["name"]
        symbol_price = lookup(symbol)["price"]
        return render_template("quoted.html", symbol=symbol, symbol_name=symbol_name, symbol_price=symbol_price)



@app.route("/register", methods=["GET", "POST"])
def register():

    if request.method == "POST":
        """Register user"""
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        """Check if username already exists or is blank"""
        if len(rows) == 1 or not username:
            return apology("Invalid username", 400)

        """Check if confirmation pass matches the password or password is blank"""
        if password != confirmation or not password or not confirmation:
            return apology("Passwords don't match or no password entered", 400)

        hashedPass = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
        """Insert username and hashed password in users"""
        db.execute("INSERT INTO users (username, hash) VALUES(?,?)", username, hashedPass)

        return redirect("/login")
    else:

        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    if request.method == "POST":

        username = db.execute("SELECT username FROM users WHERE id = ?", user_id)[0]["username"]
        now = datetime.now()
        date_time = now.strftime("%d/%m/%Y, %H:%M:%S")
        shares_to_sell = int(request.form.get("shares"))
        symbol = request.form.get("symbol").upper()
        shares = int(db.execute("SELECT SUM(shares) AS sum FROM purchases WHERE symbol = ? AND user_id = ?", symbol, user_id)[0]["sum"])

        #price = lookup(symbol)["price"]

        """Check if shares owned"""
        if shares_to_sell > shares:
            return apology("Too many shares!")

        """Update number of shares"""
        db.execute("INSERT INTO purchases (user_id, username, symbol, shares, date, price) VALUES(?,?,?,?,?,?)", user_id, username, symbol, -(shares), date_time, lookup(symbol)["price"])
        cash = db.execute("SELECT cash FROM users WHERE id=?", user_id)[0]["cash"]
        price = lookup(symbol)["price"]
        new_cash = float(cash) + price * shares
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, user_id)
        return redirect("/")
    else:
        """Get list of symbols owned"""

        symbols_dict = db.execute("SELECT DISTINCT symbol FROM purchases WHERE user_id = ?", user_id)
        symbols = []
        for sym in symbols_dict:
            symbols.append(sym["symbol"])
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
