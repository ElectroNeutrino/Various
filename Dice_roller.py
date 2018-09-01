import math
from time import time

def RandNum (initial_value): # creates a psuedo-random number from a seed using a linear congruential generator
    z = initial_value * (2 ** 32)  # un-normalize
    n = 3 ** 15
    a, b = ((8 * n) + 5), 1 #generator variables, 'a' works best when "a mod 8 = 5" 
    return float((a * z + b) % 2 ** 32) / float(2 ** 32) #calculate random number and re-normalize 
    
x = (time() % 2 ** 32) / float(2 ** 32)
x = RandNum(x)
x = RandNum(x)
x = RandNum(x)

roll_count = [0] * 6
total_rolls = 0
roll_probs = [0] * 6

while True:
    die_size = input('Die size: ')
    while True:
        x = RandNum(x)
        print int(math.ceil(x * die_size))
        new_roll = raw_input('Reroll this die (Y/n)? ')
        if new_roll.lower().startswith("n"):
            break
    new_die = raw_input('New die (Y/n)? ')
    if new_die.lower().startswith("n"):
        break
raw_input("Press Enter to exit.")
