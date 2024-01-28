#  This is a python code that makes the conversions
#  To compare our results with this code

def convert(num, base):
    if base == 2:
        decimal = int(num, 2)
        octal   = oct(decimal)
        hexadecimal     = hex(decimal)
        print("\n\nOctal: ", octal)
        print("\n\nDecimal: ", decimal)
        print("\n\nHex: ", hexadecimal)
    elif base == 8:
        decimal = int(num, 8)
        binary  = bin(decimal)
        hexadecimal = hex(decimal)
        print("\n\nBinary: ", binary)
        print("\n\nDecimal: ", decimal)
        print("\n\nHex: ", hexadecimal)
    elif base == 10:
        binary  = bin(int(num))
        octal   = oct(int(num))
        hexadecimal = hex(int(num))
        print("\n\nBinary: ", binary)
        print("\n\nOctal: ", octal)
        print("\n\nHex: ", hexadecimal)
    elif base == 16:
        decimal = int(num, 16)
        binary  = bin(decimal)
        octal   = oct(decimal)
        print("\n\nBinary: ", binary)
        print("\n\nOctal: ", octal)
        print("\n\nDecimal: ", decimal)

num = input("Please enter the number >>>  ")
base = int(input("System of the input number [2 for binary, 8 for octal, 10 for decimal, 16 for hex]>>>  "))
convert(num, base)