from cs50 import get_string


# main #
def main():
    # get input #
    text = get_string("Text: ")

    # parse string #
    num_letters, num_words, num_sentences = parse_text(text)

    # calculate index #
    index = read_index(num_letters, num_words, num_sentences)

    # user output #
    if index < 1:
        print("Before Grade 1")
    if index < 16:
        print(f"Grade {index}")
    else:
        print("Grade 16+")


# quantify text #
def parse_text(text):
    # variables #
    l = len(text)
    counts = [0, 0, 0]

    # iterate string #
    for i in range(l):
        c = text[i]

        if c in [".", "!", "?"]:
            counts[2] += 1
        elif c == ' ':
            counts[1] += 1
        elif c.isalpha():
            counts[0] += 1

    # finalize counts #
    counts[1] += 1
    return counts


# calculate Coleman-Liau index #
def read_index(num_letters, num_words, num_sentences):
    # variables #
    L = (num_letters / num_words) * 100
    S = (num_sentences / num_words) * 100

    # calculate index #
    index = 0.0588 * L - 0.296 * S - 15.8

    # return #
    return round(index)


# call main #
if __name__ == "__main__":
    main()