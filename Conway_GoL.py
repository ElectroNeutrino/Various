####################################
####################################
##                                ##
## This is a simple demonstration ##
## of Conway's Game of Life. This ##
## demo uses the pygame module.   ##
##                                ##
## "L" shows gridlines.           ##
##                                ##
## "Spacebar" starts and stops    ##
## the simulation.                ##
##                                ##
## Click and drag the mouse       ##
## pointer to fill cells.         ##
##                                ##
####################################
####################################


import pygame
import math



# define colors
White = (255, 255, 255)
Black = (0, 0, 0)
Gray = (191, 191, 191)

# define screen element sizes (in pixels)
x_res = 1600
y_res = 1200
square_size = 15

x_range = x_res / square_size
y_range = y_res / square_size

# game board
board = []


#Gosper = [[

def vertical_lines(n, surface):
    for i in range((x_res / n) + 1):
        pygame.draw.line(surface, Gray, (i * n, 0), (i * n, y_res))

def horizontal_lines(n, surface):
    for i in range((y_res / n) + 1):
        pygame.draw.line(surface, Gray, (0, i * n), (x_res, i * n))

def fill(surface):
    for x in range(x_range):
        for y in range(y_range):
            if board[x][y] == 1:
                draw_square(x, y, surface)

def clear():
    for x in range(x_range):
        for y in range(y_range):
            board[x][y] = 0

def draw_square(x, y, surface):
    pygame.draw.rect(surface, Black, (x * square_size, y * square_size, square_size, square_size))

def check_square():

    Wrap = True
    Clamp = False
    
    #create tally board
    square_count = []
    for i in range(x_range):
        square_count.append([0] * (y_range))

    #check neighbors for living cells
    for x in range(x_range):
        x_1 = x - 1
        x_2 = x + 1
        for y in range(y_range):
            square_count[x][y] = 0
            y_1 = y - 1
            y_2 = y + 1

            if Wrap:
                # wrap around the edges
                if x_1 < 0:
                    x_1 = x_range - 1
                if x_2 >= x_range:
                    x_2 = 0

                if y_1 < 0:
                    y_1 = y_range - 1
                elif y_2 >= y_range:
                    y_2 = 0
                
                # add all ajacent squares
                square_count[x][y] += board[x_1][y_1]
                square_count[x][y] += board[x_1][y]
                square_count[x][y] += board[x_1][y_2]
                square_count[x][y] += board[x][y_1]
                square_count[x][y] += board[x][y_2]
                square_count[x][y] += board[x_2][y_1]
                square_count[x][y] += board[x_2][y]
                square_count[x][y] += board[x_2][y_2]

            elif Clamp:
                # clamp to board area
                if x_1 < 0:
                    x_1 = x
                if x_2 >= x_range:
                    x_2 = x

                if y_1 < 0:
                    y_1 = y
                elif y_2 >= y_range:
                    y_2 = y
                
                # add all ajacent squares
                square_count[x][y] += board[x_1][y_1]
                square_count[x][y] += board[x_1][y]
                square_count[x][y] += board[x_1][y_2]
                square_count[x][y] += board[x][y_1]
                square_count[x][y] += board[x][y_2]
                square_count[x][y] += board[x_2][y_1]
                square_count[x][y] += board[x_2][y]
                square_count[x][y] += board[x_2][y_2]
            else:
                #dead edge
                if (x_1 < 0) or (x_2 >= x_range) or (y_1 < 0) or (y_2 >= y_range):
                    square_count[x][y] = 0
                else:
                    square_count[x][y] += board[x_1][y_1]
                    square_count[x][y] += board[x_1][y]
                    square_count[x][y] += board[x_1][y_2]
                    square_count[x][y] += board[x][y_1]
                    square_count[x][y] += board[x][y_2]
                    square_count[x][y] += board[x_2][y_1]
                    square_count[x][y] += board[x_2][y]
                    square_count[x][y] += board[x_2][y_2]
                    


    # adjust cells based on score
    for x in range(x_range):
        for y in range(y_range):
            if ((square_count[x][y] < 2) or (square_count[x][y] > 3)):
                board[x][y] = 0
            elif square_count[x][y] == 3:
                board[x][y] = 1


#game control variables
Play_game = False
Draw = False
Show_lines = False

# initialize display
pygame.init()
resolution = (x_res, y_res)
display = pygame.display.set_mode(resolution, pygame.DOUBLEBUF)

# adjust size of board list to match square count in x and y directions
for i in range(x_range):
    board.append([0] * (y_range))

# main game loop
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()
        elif (event.type == pygame.MOUSEBUTTONUP) and (event.button == 1):
            Draw = False
        elif (event.type == pygame.MOUSEBUTTONDOWN) and (event.button == 1):
            mouse_pos = event.pos
            x = int(math.floor(mouse_pos[0] / square_size))
            y = int(math.floor(mouse_pos[1] / square_size))
            board[x][y] = 1
            Draw = True
        elif (event.type == pygame.MOUSEMOTION) and Draw:
            mouse_pos = event.pos
            x = int(math.floor(mouse_pos[0] / square_size))
            y = int(math.floor(mouse_pos[1] / square_size))
            board[x][y] = 1
        elif (event.type == pygame.KEYUP) and (event.key == pygame.K_SPACE):
            Play_game = not Play_game
        elif (event.type == pygame.KEYUP) and (event.key == pygame.K_l):
            Show_lines = not Show_lines
        elif (event.type == pygame.KEYUP) and (event.key == pygame.K_c):
            clear()

    # blank screen
    pygame.draw.rect(display, White, (0, 0, x_res, y_res))

    fill(display)
    if Play_game and not Draw:
        check_square()
    
    # draw grid
    if Show_lines:
        vertical_lines(square_size, display)
        horizontal_lines(square_size, display)

    pygame.display.flip()

    # slow game speed if not drawing to prevent from going too fast to see
    if Play_game and not Draw:
        pygame.time.wait(50)
    else:
        pygame.time.wait(5)
