# main #
def main():
    # user input #
    height = get_height()

    # print half-pyramid #
    print_pyramid(height)


# def user input #
def get_height():
    while True:
        # attempt get int #
        try:
            h = int(input("Height: "))
        except ValueError:
            continue

        # check val #
        if (h > 0 and h < 9):
            break

    # give height #
    return h


# define print #
def print_pyramid(h):
    for i in range(h):
        # first half #
        print(" " * (h - 1 - i), end="")
        print("#" * (i + 1), end="")

        # middle space
        print("  ", end="")

        # second half #
        print("#" * (i + 1))


# call main #
if __name__ == "__main__":
    main()