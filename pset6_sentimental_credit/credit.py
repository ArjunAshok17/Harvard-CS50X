from cs50 import get_int


# main #
def main():
    # user input #
    number = get_card_num()

    # calculations #
    card = get_card(number)

    # output #
    print(card)


# user input #
def get_card_num():
    # get string #
    number = get_int("Number: ")

    # return #
    return number


# find bank #
def get_card(number):
    # variables #
    temp_num = number
    luhn_sum = 0
    l = len(str(number))
    bank_id = number // (10 ** (l - 2))

    # invalid check #
    if not l in (13, 15, 16):
        return "INVALID"

    # Luhn's num #
    for i in range(l):
        # update number #
        digit = int(temp_num % 10)
        temp_num = temp_num // 10

        # add num #
        if i % 2 == 0:
            luhn_sum += digit
        # * 2 & sum digits #
        else:
            luhn_sum += sum_digits(digit * 2)

    if (luhn_sum % 10 != 0):
        return "INVALID"

    # check against banks #
    if l == 15 and (bank_id == 34 or bank_id == 37):
        return "AMEX"
    elif l == 16 and (bank_id >= 51 and bank_id <= 55):
        return "MASTERCARD"
    elif (l == 13 or l == 16) and (bank_id // 10 == 4):
        return "VISA"
    else:
        return "INVALID"


# helper #
def sum_digits(num):
    # variables #
    sum = 0

    # each digit #
    for i in range(len(str(num))):
        sum += int(num % 10)
        num //= 10

    # return #
    return sum


# call main #
if __name__ == "__main__":
    main()