import math
import os

####### first 100 prime numbers ######
prime = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
         59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
         127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
         191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251,
         257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
         331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
         401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
         467, 479, 487, 491, 499, 503, 509, 521, 523, 541];



input_number = raw_input('Number to factor: ')

try:
   input_number = int(input_number)
except ValueError:
   raw_input("Value must be an integer. Press Enter to exit.")
   os._exit(1)

input_number = abs(input_number)

if (input_number < 2):
    raw_input("Value cannot be prime. Press Enter to exit.")
    os._exit(1)

# determine the index of the lagest prime thats less or equal to sqrt(input) #
for i in range(0, len(prime)):
    if (prime[i] <= input_number):
        max_prime_index = i

prime_factors = [];
for i in range(0, max_prime_index + 1):
    while (input_number % prime[i] == 0):
        prime_factors.append(prime[i])
        input_number = input_number / prime[i]

print prime_factors

raw_input("Press Enter to exit.")
