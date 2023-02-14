import os
import time

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
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
@login_required
def index():
    """Show portfolio of stocks"""

    # query portfolio #
    portfolio = db.execute("SELECT symbol, sum(shares) FROM trades WHERE user_id = ? GROUP BY symbol", session["user_id"])

    # restructure results #
    total_assets = 0.0
    for stock in portfolio:
        # if stock["sum(shares)"] == 0:
        #     portfolio.remove(stock)
        #     continue

        stock_info = lookup(stock["symbol"])

        stock["company"] = stock_info["name"]
        stock["price"] = stock_info["price"]
        stock["shares"] = stock["sum(shares)"]
        stock["total_value"] = float(stock_info["price"]) * int(stock["shares"])
        total_assets += float(stock["total_value"])

        del stock["symbol"]
        del stock["sum(shares)"]

    # cash balance #
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    total_assets += user_cash

    # pass info #
    return render_template("index.html", portfolio=portfolio, balance=user_cash, total=total_assets)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # check request #
    if request.method == "POST":
        # get purchase request #
        ticker = request.form.get("symbol")
        num_shares = request.form.get("shares")

        # validation #
        ticker_lookup = lookup(ticker)
        if ticker_lookup == None:
            return apology("Invalid ticker symbol")

        try:
            num_shares = int(num_shares)
        except ValueError:
            return apology("Invalid number of shares")

        if num_shares <= 0:
            return apology("Invalid number of shares")

        # get purchase info #
        cur_price = float(ticker_lookup["price"])
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # validate purchase #
        if cur_price * num_shares > user_cash:
            return apology("Insufficient funds to complete transaction")

        # make purchase #
        db.execute("INSERT INTO trades (user_id, symbol, shares, price, time) VALUES(?, ?, ?, ?, ?)",
                   session["user_id"], ticker, num_shares, cur_price, time.time())
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash - cur_price * num_shares, session["user_id"])

        # redirect home #
        return redirect("/")
    else:
        # access user records #
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        balance = balance

        # generate webpage #
        return render_template("buy.html", balance=balance)


@app.route("/add_funds", methods=["POST"])
@login_required
def add_funds():
    """Adds funds into the users' account"""
    # get amount #
    deposit = float(request.form.get("deposit"))

    # validate amount #
    if deposit < 0:
        apology("Invalid deposit amount")

    # make deposit #
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash + deposit, session["user_id"])

    # redirect #
    return redirect("/buy")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # query portfolio #
    trades = db.execute("SELECT * FROM trades WHERE user_id = ?", session["user_id"])

    for trade in trades:
        trade["action"] = "bought" if trade["shares"] > 0 else "sold"
        trade["price"] = trade["price"]
        trade["shares"] = abs(trade["shares"])
        trade["time"] = str(time.ctime(trade["time"]))

    # pass info #
    return render_template("history.html", trades=trades)


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
    # check request #
    if request.method == "POST":
        # obtain ticker query #
        ticker = request.form.get("symbol")

        # validation #
        if ticker == "":
            return apology("Invalid ticker symbol")

        # lookup value #
        ticker_price = lookup(ticker)

        # lookup validation #
        if ticker_price == None:
            return apology("Invalid ticker symbol!")

        # redirect #
        ticker_price["price"] = ticker_price["price"]
        return render_template("quoted.html", price=ticker_price)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # check request type #
    if request.method == "POST":
        # validation #
        if not request.form.get("username"):
            return apology("must provide username")
        elif not request.form.get("password"):
            return apology("must provide password")
        elif not request.form.get("confirmation"):
            return apology("must confirm password")

        # get info #
        username = request.form.get("username")
        password = request.form.get("password")
        confirm_pass = request.form.get("confirmation")

        # validation #
        copy_user = db.execute("SELECT * FROM users WHERE username = ?", username)
        if username == "" or len(copy_user) > 0:
            return apology("Invalid username")
        if not password == confirm_pass or password == "" or confirm_pass == "":
            return apology("Invalid password")

        # add to db #
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))

        return redirect("/login")
    else:
        # redirect user #
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # check request #
    if request.method == "POST":
        # get sell request #
        ticker = request.form.get("symbol")
        num_shares = float(request.form.get("shares"))

        # validation #
        if ticker == "" or len(db.execute("SELECT * FROM trades WHERE user_id = ? AND symbol = ?", session["user_id"], ticker)) == 0:
            return apology("Invalid ticker selected")
        if num_shares < 0 or num_shares > db.execute("SELECT sum(shares) FROM trades WHERE user_id = ? AND symbol = ?", session["user_id"], ticker)[0]["sum(shares)"]:
            return apology("Invalid number of shares")

        # get stock info #
        ticker_lookup = lookup(ticker)
        cur_price = ticker_lookup["price"]
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # make sale #
        db.execute("INSERT INTO trades (user_id, symbol, shares, price, time) VALUES(?, ?, ?, ?, ?)",
                   session["user_id"], ticker, -num_shares, cur_price, time.time())
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_cash + cur_price * num_shares, session["user_id"])

        # redirect home #
        return redirect("/")
    else:
        # get portfolio #
        portfolio = db.execute("SELECT symbol, sum(shares) FROM trades WHERE user_id = ? GROUP BY symbol", session["user_id"])

        # create webpage #
        return render_template("sell.html", portfolio=portfolio)
