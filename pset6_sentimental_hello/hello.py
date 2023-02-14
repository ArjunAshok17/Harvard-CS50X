from cs50 import get_string


# main #
def main():
    hello()


def hello():
    # user input #
    name = get_string("What is your name?\n")

    # output #
    print(f"hello, {name}")


# run main #
if __name__ == "__main__":
    main()