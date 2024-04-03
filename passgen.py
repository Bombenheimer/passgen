#!/usr/bin/env python3

"""
This program was designed by Bombenheimer.
Follow me on GitHub for more projects like these and collaborate!

GitHub: https://github.com/Bombenheimer/
Linkedin: https://www.linkedin.com/in/bruce-smith-4a4941296/
"""
# IMPORTING NECESSARY MODULES
from random import *
from time import sleep

# PRINT WELCOME MESSAGE
def PrintWelcome():
    MSG = """
    Welcome to the Linux password generator.
    Lets generate a password for your new user(s).

    Press Enter to Continue.
    """

    for char in MSG:
        print(char, end='', flush=True)
        sleep(0.01)

    userCont = input("")

    return 0

# PROMPT THE USER FOR PASSWORD RULES
def PromptUser():
    numUsers = 0
    passwordLength = 0
    hasNumbers = False
    hasSymbols = False
    hasUppercase = False
    hasLowercase = False

    PROMPT_RULE1 = """
    Enter how many users will need passwords.
    """

    PROMPT_RULE2 = """
    Enter your desired password length.
    """
    
    PROMPT_RULE3 = """
    Do you want your password to contain numbers?
    """

    PROMPT_RULE4 = """
    Would you like your password to contain special characters?
    """

    PROMPT_RULE5 = """
    Would you like your password to contain uppercase letters?
    """

    PROMPT_RULE6 = """
    Would you like your password to contain lowercase letters?
    """

    for char in PROMPT_RULE1:
        print(char, end='', flush=True)
        sleep(0.01)

    numUsers = input("Number of users: ")

    for char in PROMPT_RULE2:
        print(char, end='', flush=True)
        sleep(0.01)

    passwordLength = input("Password Length: ")

    for char in PROMPT_RULE3:
        print(char, end='', flush=True)
        sleep(0.01)

    hasNumbers = input("Enter an option (yes/no): ")

    for char in PROMPT_RULE4:
        print(char, end='', flush=True)
        sleep(0.01)

    hasSymbols = input("Enter an option (yes/no): ")

    for char in PROMPT_RULE5:
        print(char, end='', flush=True)
        sleep(0.01)

    hasUppercase = input("Enter an option (yes/no): ")

    for char in PROMPT_RULE6:
        print(char, end='', flush=True)
        sleep(0.01)

    hasLowercase = input("Enter an option (yes/no): ")

    return numUsers, passwordLength, hasNumbers, hasSymbols, hasUppercase, hasLowercase

# GET NAMES OF USERS
def GetUsernames(numUsers):
    userList = []
    
    for i in range(numUsers):
        tmpInput = input(f"    Enter the username for user number {i + 1}: ")
        userList.append(tmpInput)
        print()

    return userList

# GENERATING THE PASSWORD
def GeneratePasswords(numUsers, userList, passwordLength, hasNumbers, hasSymbols, hasUppercase, hasLowercase):
    lowercase = "abcdefghijklmnopqrstuvwxyz"
    uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    numbers = "1234567890"
    symbols = "!@#$%^&*()~`|\\/?><,[]{}-_+="	
    characters = []
    passwordList = []
    choices = [lowercase, uppercase, numbers, symbols]

    if (hasNumbers == "no"):
        choices.remove(numbers)
    if (hasSymbols == "no"):
        choices.remove(symbols)
    if (hasUppercase == "no"):
        choices.remove(uppercase)
    if (hasLowercase == "no"):
        choices.remove(lowercase)

    for i in range(numUsers):
        characters.clear()
        for j in range(passwordLength):
            character = choice(choices)
            char = choice(character)
            characters.append(char)
            shuffle(characters)

        password = ''.join(characters)
        passwordList.append(password)

    print("    Generating passwords...")
    sleep(3)

    return passwordList

# WRITE PASSWORDS AND THEIR INFORMATION TO A FILE
def PrintToFile(passwordList, numUsers, userList, passwordLength):
    MSG = """
    What would you like your password file's name to be?
    """
    for char in MSG:
        print(char, end='', flush=True)
        sleep(0.01)

    filename = input("Filename (Enter name or path): ")

    with open(filename, 'a') as file:
        for i in range(numUsers):
            file.write(f"    User {userList[i]}'s password is: {passwordList[i]}" + '\n')

        if (passwordLength < 8):
            file.write("    Password Strength: BAD")
        elif (passwordLength >= 8 and passwordLength <= 10):
            file.write("    Password Strength: OK")
        elif (passwordLength >= 11 and passwordLength <= 12):
            file.write("    Password Strength: GREAT")
        elif (passwordLength > 12):
            file.write("    Password Strength: EXCELLENT")
    
    print()
    print("    Writing to file {filename}...")
    sleep(1)
    print()
    print(f"    Be sure that {filename} is safe from tampering from unauthorized users. Bye!")
    print()
    print("    [Process Completed.]")
    sleep(1)

    return 0

# MAIN FUNCTION
def main():
    PrintWelcome()
    numUsers, passwordLength, hasNumbers, hasSymbols, hasUppercase, hasLowercase = PromptUser()
    print()
    userList = GetUsernames(int(numUsers))
    passwordList = GeneratePasswords(int(numUsers), userList, int(passwordLength), hasNumbers, hasSymbols, hasUppercase, hasLowercase)
    PrintToFile(passwordList, int(numUsers), userList, int(passwordLength))

    return 0

# CHECK IF PROGRAM IS BEING IMPORTED OR RAN, AND CHECK EXECUTION TIME
if __name__ == "__main__":
    main()
