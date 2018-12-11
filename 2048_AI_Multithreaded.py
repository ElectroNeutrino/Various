import numpy
import random
import math
import time
import threading

###############
# Definitions #
###############

# initialize random seed
numpy.random.seed(100)

# Multithreading definitions
writing_lock = threading.Lock()
thread_count = 7

# Player
input_source = "AI"

# AI running values
population_count = 10
run_max = 1000
gen_max = 100
pair_count = math.floor(math.sqrt(population_count))

# Define number of input nodes (number of game cells)
node_in = 16
# Define size of internal nodes
node_1 = 32
# Define number of output nodes (number of directions)
node_out = 4

testing = False

# Total total_score (when cells combine, add new cell value to total_score)
total_score = 0

# Enumerate input values
input_type = ['up', 'down', 'left', 'right',]

# Since everything is a power of two, only need to store the exponents
# Create empty 4x4 array to represent game board (exponents) and display board
game_board = numpy.zeros((4, 4), dtype=numpy.int64) 
display_board = numpy.zeros((4, 4), dtype=numpy.int64) 

# Cell value at row x colum y = game_board[x][y]

#  0 1 2 3 y
#0 * * * *
#1 * * * *
#2 * * * *
#3 * * * *
#x




########################
# Function Definitions #
########################

# format float seconds to HH:MM:SS
def format_seconds(seconds):
    hours = math.floor(seconds / 3600)
    seconds -= hours * 3600
    minutes = math.floor(seconds / 60)
    seconds -= minutes * 60

    f = "{:02}:{:02}:{:05.2f}".format(hours, minutes, seconds)

    return f

# Fill a random cell, 10% change of 4 instead of 2
def random_value(start = False):
    continue_game = False

    # keep going if even one cell is empty
    for x in range(4):
        for y in range(4):
            if game_board[x][y] == 0:
                continue_game = True

    # Select one of the 16 cells as the initial value
    i = random.choice(range (16))
    exclude_list = []
    while continue_game and len(exclude_list) < 16 and not (game_board[numpy.unravel_index(i,(4,4))] == 0):
        # if cell not empty, add to exclude list and choose another cell
        i = random.choice([j for j in range (16) if j not in exclude_list])
        exclude_list.append(i)
    
    is_four = (numpy.random.randint(0,9) == 0)
    if is_four and not start:
        game_board[x][y] = 2
    else:
        game_board[x][y] = 1
    
    return continue_game

# Update the display board and print it
def print_display():
    for x in range(4):
        for y in range(4):
            if game_board[x][y] == 0:
                display_board[x][y] = 0
            else:
                display_board[x][y] = math.pow(2, game_board[x][y])
    print (display_board)
    print ("Score:", total_score, '\n')
    return

# Add the value to the total score
def update_score(value):
    global total_score
    total_score += int(math.pow(2, value))
    return

# Move square items. If target cell is empty, move value. If equal, combine
def move(direction):
    moved = False
    # collapse, combine, collapse
    if direction == 'up':
        for y in range(4): # check per column
            for x in range(3):
                i = 1
                # while current cell is empty, loop until non-empty cell or end of list
                while game_board[x][y] == 0 and (x+i) < 4:
                    game_board[x][y] = game_board[x+i][y]
                    game_board[x+i][y] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
            for x in range(3):
                # if pair found, increment current cell and empty cell below
                if game_board[x][y] == game_board[x+1][y] and not game_board[x][y] == 0:
                    game_board[x][y] += 1
                    game_board[x+1][y] = 0
                    update_score(game_board[x][y])
                    moved = True
            for x in range(3):
                i = 1
                # while current cell is empty, loop until non-empty cell or end of list
                while game_board[x][y] == 0 and (x+i) < 4:
                    game_board[x][y] = game_board[x+i][y]
                    game_board[x+i][y] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
    elif direction == 'down':
        for y in range(4): # check per column
            for x in reversed(range(1,4)):
                i = 1
                # while current cell is empty, loop up until non-empty cell or end of list
                while game_board[x][y] == 0 and (x-i) >= 0:
                    game_board[x][y] = game_board[x-i][y]
                    game_board[x-i][y] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
            for x in reversed(range(1,4)):
                # if pair found, increment current cell and empty cell above
                if game_board[x][y] == game_board[x-1][y] and not game_board[x][y] == 0:
                    game_board[x][y] += 1
                    game_board[x-1][y] = 0
                    update_score(game_board[x][y])
                    moved = True
            for x in reversed(range(1,4)):
                i = 1
                # while current cell is empty, loop up until non-empty cell or end of list
                while game_board[x][y] == 0 and (x-i) >= 0:
                    game_board[x][y] = game_board[x-i][y]
                    game_board[x-i][y] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
    elif direction == 'left':
        for x in range(4): # check per row
            for y in range(3):
                i = 1
                # while current cell is empty, loop right until non-empty cell or end of list
                while game_board[x][y] == 0 and (y+i) < 4:
                    game_board[x][y] = game_board[x][y+i]
                    game_board[x][y+i] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
            for y in range(3):
                # if pair found, increment current cell and empty cell to right
                if game_board[x][y] == game_board[x][y+1] and not game_board[x][y] == 0:
                    game_board[x][y] += 1
                    game_board[x][y+1] = 0
                    update_score(game_board[x][y])
                    moved = True
            for y in range(3):
                i = 1
                # while current cell is empty, loop right until non-empty cell or end of list
                while game_board[x][y] == 0 and (y+i) < 4:
                    game_board[x][y] = game_board[x][y+i]
                    game_board[x][y+i] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
    elif direction == 'right':
        for x in range(4): # check per row
            for y in reversed(range(1,4)):
                i = 1
                # while current cell is empty, loop right until non-empty cell or end of list
                while game_board[x][y] == 0 and (y-i) >= 0:
                    game_board[x][y] = game_board[x][y-i]
                    game_board[x][y-i] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
            for y in reversed(range(1,4)):
                # if pair found, increment current cell and empty cell to right
                if game_board[x][y] == game_board[x][y-1] and not game_board[x][y] == 0:
                    game_board[x][y] += 1
                    game_board[x][y-1] = 0
                    update_score(game_board[x][y])
                    moved = True
            for y in reversed(range(1,4)):
                i = 1
                # while current cell is empty, loop right until non-empty cell or end of list
                while game_board[x][y] == 0 and (y-i) >= 0:
                    game_board[x][y] = game_board[x][y-i]
                    game_board[x][y-i] = 0
                    i += 1
                    if not game_board[x][y] == 0:
                        moved = True
    if moved:
        continue_game = random_value()
        if input_source == "User":
            print_display()
        return continue_game
    else:
        if input_source == "User":
            return True
        else:
            return False

# Sigmoid function for value clamping.
def sigmoid(input_x):  
    return numpy.exp(-numpy.logaddexp(0, -input_x))

# Binomial function to compute pair combinations
def binomial(n, k):
    """
    A fast way to calculate binomial coefficients by Andrew Dalke (contrib).
    """
    if 0 <= k <= n:
        ntok = 1
        ktok = 1
        for t in range(1, min(k, n - k) + 1):
            ntok *= n
            ktok *= t
            n -= 1
        return ntok // ktok
    else:
        return 0

# Initialize board
def initialize_board():
    global game_board
    game_board = numpy.zeros((4, 4), dtype=numpy.int64)
    random_value(True)
    random_value(True)
    return

def calculate_move(input_intermediate_array, bias_1, intermediate_output_array, bias_out, pop):
    input_values = numpy.zeros((node_in, 1), dtype=numpy.float64)
    # input values from game_board
    for x in range(4):
        for y in range(4):
            input_values[x*4 + y][0] = game_board[x][y]

    # calculate next layer
    intermediate_values = sigmoid(input_intermediate_array[pop].dot(input_values) + bias_1[pop])
    #calculate output values
    output_values = sigmoid(intermediate_output_array[pop].dot(sigmoid(input_intermediate_array[pop].dot(input_values) + bias_1[pop])) + bias_out[pop])

    # get direction to move
    ai_direction = input_type[numpy.argmax(output_values)]

    return ai_direction


def run_single_pop(input_intermediate_array, bias_1, intermediate_output_array, bias_out, average_score, best_index, j):
    for pop in [k for k in range(population_count) if (k%4==j)]:
        running_score = 0
        running_steps = 0
        input_values = numpy.zeros((node_in, 1), dtype=numpy.float64)
        for run in range(run_max):
            global total_score
            total_score = 0
            # empty board and set initial values
            initialize_board()

            # calculate first move
            ai_direction = calculate_move(input_intermediate_array, bias_1, intermediate_output_array, bias_out, pop)
            
            # Run single game
            index = 1
            while(move(ai_direction)):
                ai_direction = calculate_move(input_intermediate_array, bias_1, intermediate_output_array, bias_out, pop)
                index += 1
            running_score += total_score
            running_steps += index

        # lock for multithreading write
        writing_lock.acquire()

        # update averages, and store best performers
        average_score[pop][0] = running_score / run_max
        average_steps[pop][0] = running_steps / run_max

        # calculate best index
        if average_score[pop][0] > best_index[0][1]:
            for i in reversed(range(pair_count - 1)):
                best_index[i+1] = best_index[i]
            best_index[0] = (int(pop), average_score[pop][0], average_steps[pop][0])

        # unlock for multithreading
        writing_lock.release()
    return



##########################
#                        #
#    Playing the game    #
#                        #
##########################


###################
# User Input Mode #
###################

if input_source == "User":
    # User input
    initialize_board()
    print_display()
    user_move = input("Direction?")
    if user_move.lower() not in input_type:
        print ("Usage: up, down, left, right.")
        user_move = input("Direction?")

    while (move(user_move)):
        user_move = input("Direction?")
        if user_move.lower() not in input_type:
            print ("Usage: up, down, left, right.")
            user_move = input("Direction?")




#################
# AI Input Mode #
#################

elif input_source == "AI":

    # calcuating number of pairs for Genetic Algorithm
    combination_count = binomial(pair_count, 2) + (pair_count * 2.0) # best, best combinations, and best + random
    while combination_count > population_count:
        print ("Number of pairs too high. Combinations:", combination_count, ". Reducing by 1.")
        pair_count -= 1
        combination_count = binomial(pair_count, 2) + (pair_count * 2.0)

    # initialized values
    running_score = 0
    running_steps = 0

    ###########
    # AI Part #
    ###########

    # initialize AI weighted arrays and biases
    input_intermediate_array = numpy.random.rand(population_count, node_1, node_in) #input to first level
    intermediate_output_array = numpy.random.rand(population_count, node_out, node_1) #second level to output

    bias_1 = numpy.random.rand(population_count, node_1, 1) #bias from input to first level
    bias_out = numpy.random.rand(population_count, node_out, 1) #bias from second level to output

    print ("Number of threads:", thread_count)
    print ("Number of generations:", gen_max)
    print ("Population per generation:", population_count)
    print ("Max runs per population member:", run_max)
    print ("Carryover per generation:", pair_count, '\n')

    time_start = time.time()
    t_1 = time_start

    for gen in range(gen_max):

        print ("Generation:", gen + 1)

        # initialize empty averages
        best_index = numpy.zeros((pair_count, 3), dtype=numpy.float64)
        average_score = numpy.zeros((population_count, 1), dtype=numpy.float64)
        average_steps = numpy.zeros((population_count, 1), dtype=numpy.float64)

        # spin off population runs into n separate threads
        running_threads = []
        for j in range(thread_count):
            # loop through performance for each member
            running_threads.append(threading.Thread(target=run_single_pop, args=(input_intermediate_array, bias_1, intermediate_output_array, bias_out, average_score, best_index, j)))
            running_threads[-1].start()

        # wait for all threads to exit
        for thread in running_threads:
            thread.join()

        # Output every n generations
        if ((gen+1)%10 == 0) or gen == (gen_max-1):
            time_end = time.time()
            time_average = (time_end - time_start) / int(gen_max / 10)
            time_start = time_end
            t_2 = time.time()
            remaining_gens = gen_max - gen
            estimated_time = ((t_2 - t_1) / (gen + 1)) * remaining_gens
            print ("Average time per generation:", format_seconds(round(time_average, 2)))
            print ("Estimated time to completion:", format_seconds(round(estimated_time, 2)))
            print ("Best performers:")
            for i  in range(pair_count):
                print ("\tID:", int(best_index[i][0]), ", Average Score:", round(best_index[i][1], 2), ", Average Steps:", round(best_index[i][2], 2))
            print('\n')

    #########################
    # Genetic Algoritm Part #
    #########################

        # only calculate nex generation if not last gen
        if not gen == (gen_max - 1):
            #copy population to temp values for calculation
            new_IIA = input_intermediate_array.copy()
            new_IOA = intermediate_output_array.copy()
            new_bias_1 = bias_1.copy()
            new_bias_out = bias_out.copy()

            # Calculate new population members (take best and averages of them, fill rest with random)
            calc_index = 0
            for x in range(pair_count):
                for y in range(x,pair_count):
                    if x==y: # Add best
                        input_intermediate_array[calc_index] = new_IIA[int(best_index[x][0])]
                        intermediate_output_array[calc_index] = new_IOA[int(best_index[x][0])]

                        bias_1[calc_index] = new_bias_1[int(best_index[x][0])]
                        bias_out[calc_index] = new_bias_out[int(best_index[x][0])]
                    else: # average and add pairs of best
                        input_intermediate_array[calc_index] = (new_IIA[int(best_index[x][0])] + new_IIA[int(best_index[y][0])]) * 0.5
                        intermediate_output_array[calc_index] = (new_IOA[int(best_index[x][0])] + new_IOA[int(best_index[y][0])]) * 0.5

                        bias_1[calc_index] = (new_bias_1[int(best_index[x][0])] + new_bias_1[int(best_index[y][0])]) * 0.5
                        bias_out[calc_index] = (new_bias_out[int(best_index[x][0])] + new_bias_out[int(best_index[y][0])]) * 0.5
                    calc_index += 1

            for x in range(pair_count): # average and add best with random
                input_intermediate_array[calc_index] = (new_IIA[int(best_index[x][0])] + numpy.random.rand(node_1, node_in)) * 0.5
                intermediate_output_array[calc_index] = (new_IOA[int(best_index[x][0])] + numpy.random.rand(node_out, node_1)) * 0.5

                bias_1[calc_index] = (new_bias_1[int(best_index[x][0])] + numpy.random.rand(node_1,1)) * 0.5
                bias_out[calc_index] = (new_bias_out[int(best_index[x][0])] + numpy.random.rand(node_out,1)) * 0.5
                calc_index += 1

            while calc_index < population_count: #fill rest with random
                input_intermediate_array[calc_index] = numpy.random.rand(node_1, node_in)
                intermediate_output_array[calc_index] = numpy.random.rand(node_out, node_1) * 0.5

                bias_1[calc_index] = numpy.random.rand(node_1,1) * 0.5
                bias_out[calc_index] = numpy.random.rand(node_out,1) * 0.5
                calc_index += 1

        

    t_2 = time.time()
    total_elapsed = round(t_2 - t_1, 3)
    print ("Total time:", format_seconds(total_elapsed))



################
# Testing Mode #
################

elif input_source == "Test":
    input_intermediate_array = input("input_intermediate_array")
    intermediate_output_array = input("ntermediate_output_array")
    bias_1 = input("bias_1")
    bias_out = input("bias_out")


# Run sample AI with visual reference
if testing == True and not input_source == "User":
    
    if not input_source == "Test":
        input_intermediate_array = input_intermediate_array[int(best_index[i][0])].copy()
        intermediate_output_array = intermediate_output_array[int(best_index[i][0])].copy()
        bias_1 = bias_1[int(best_index[i][0])].copy()
        bias_out = bias_out[int(best_index[i][0])].copy()

    total_score = 0
    initialize_board()
    # calculate first move
    # input values from game_board
    for x in range(4):
        for y in range(4):
            input_values[x*4 + y][0] = game_board[x][y]

    # calculate next layer
    intermediate_values = sigmoid(input_intermediate_array.dot(input_values) + bias_1)
    #calculate output values
    output_values = sigmoid(intermediate_output_array.dot(intermediate_values) + bias_out)

    # get direction to move
    ai_direction = input_type[numpy.argmax(output_values)]

    print_display()
    print ("AI selected move", ai_direction)
    input("Press Enter for first move.")

    while(move(ai_direction)):
        # input values from game_board
        for x in range(4):
            for y in range(4):
                input_values[x*4 + y][0] = game_board[x][y]

        # calculate next layer
        intermediate_values = sigmoid(input_intermediate_array.dot(input_values) + bias_1)
        #calculate output values
        output_values = sigmoid(intermediate_output_array.dot(intermediate_values) + bias_out)
        
        # get direction to move
        ai_direction = input_type[numpy.argmax(output_values)]

        print_display()
        print ("AI selected move", ai_direction)
        input("Press Enter for next move.")

    print("Game Over")
