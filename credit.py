from cs50 import get_int

""" Check if a credit card number is valid or not """

# Prompt the user for a credit card number
card = get_int("Card number: ")

# Multiplying every other digit by 2
sum1 = 0
for i in range(2, 17, 2):
    digit = ((card % pow(10, i) - card % pow(10, i - 1)) / pow(10, i - 1)) * 2
    if digit > 9:
        sum1 += digit % 10 + (digit - digit % 10) / 10
    else:
        sum1 += digit

# Add digits that weren't multiplied by 2
sum2 = 0
for i in range(1, 17, 2):
    sum2 += ((card % pow(10, i) - card % pow(10, i - 1)) / pow(10, i - 1))

finalSum = sum1 + sum2

# Card length
length = 0
if card % pow(10, 16) / pow(10, 15) == card / pow(10, 15) and card % pow(10, 15) / pow(10, 14) != card / pow(10, 14):
    length = 16
elif card % pow(10, 15) / pow(10, 14) == card / pow(10, 14) and card % pow(10, 14) / pow(10, 13) != card / pow(10, 13):
    length = 15
elif card % pow(10, 13) / pow(10, 12) == card / pow(10, 12) and card % pow(10, 12) / pow(10, 11) != card / pow(10, 11):
    length = 13

# Starting digits
startDigits = 0
if length == 16:
    startDigits = (card - card % pow(10, 15)) / pow(10, 15) * 10 + (card % pow(10, 15) - card % pow(10, 14)) / pow(10, 14)
elif length == 15:
    startDigits = (card - card % pow(10, 14)) / pow(10, 14) * 10 + (card % pow(10, 14) - card % pow(10, 13)) / pow(10, 13)
elif length == 13:
    startDigits = (card - card % pow(10, 12)) / pow(10, 12) * 10 + (card % pow(10, 12) - card % pow(10, 11)) / pow(10, 11)

# Type of card
if finalSum % 10 == 0:
    if length == 15 and (startDigits == 34 or startDigits == 37):
        print("AMEX")
    elif length == 16 and (startDigits == 51 or startDigits == 52 or startDigits == 53 or startDigits == 54 or startDigits == 55):
        print("MASTERCARD")
    elif (length == 16 or length == 13) and (startDigits - startDigits % 10) / 10 == 4:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
