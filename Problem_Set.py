import csv
import random



#### Import Data ####

# Opens LaTex formatted problems in Exercises.txt and imports it as tab delimited arrray grouped by line and column
with open("Exercises.txt") as problem_in:
    reader = csv.reader(problem_in, delimiter="\t")
    Imported_Data = list(reader)

# Opens LaTeX formatting header
with open("Problem_Set_Start.txt") as Problem_Start_in:
    Problem_Start = Problem_Start_in.read()
with open("Problem_Key_Start.txt") as Problem_Key_Start_in:
    Problem_Key_Start = Problem_Key_Start_in.read()

# Opens LaTeX formatting footer
with open("Problem_Set_End.txt") as Problem_End_in:
    Problem_End = Problem_End_in.read()

# Grabs the line number where each excercise is labeled and stores them
Exercise_label_position = []
for i in range(len(Imported_Data)):
    if "Exercise" in str(Imported_Data[i]): # Find a line with the text "Exercise"
        Exercise_label_position.append(i) # Store its location

# Finds the number of problems in each set
# Each label should have <Name>, <First Problem Number>, <Last Problem Number> #
Exercise_size = []
Excercise_List = []
for i in range(len(Exercise_label_position)):
    Current_Exercise = Exercise_label_position[i]
    Exercise_size.append(int(Imported_Data[Current_Exercise][2]) - int(Imported_Data[Current_Exercise][1]) + 1) # number of problems in each set = (last problem number - first problem number) + 1 since counting starts at 1.
    Excercise_List.append(Imported_Data[Current_Exercise][0:2])




#### Format data into structured lists. ####

Exercises = []
Problems = []
for label_index in range(len(Exercise_label_position)): # Runs through each element of Exercise_label_position
    i = Exercise_label_position[label_index] # Grab the index of the excercise label
    for j in range(i+1, i+Exercise_size[label_index]+1): # Grab the index of the current problem
        Problems.append(Imported_Data[j][0]) # Locate problem and append it to Problem list
    Exercises.append(Problems) # Append the problem to Excercises
    Problems = [] # Clear the problem list




#### Generate randomized selection of problems ####

Problem_Set = []
Problem_Key = []

for Exercise_Index,Exercise in enumerate(Exercises):
    Sample_List = Exercise[:] # copy problems in exercise
    for i in range(2): # add n problems to selected problems
        Sample_Index = random.randrange(len(Sample_List))
        Sample = Sample_List.pop(Sample_Index) #Sample a random problem from list
        Problem_Set.append(Sample)
        Problem_Key.append(Excercise_List[Exercise_Index][0] + '.' + str(Sample_Index + int(Excercise_List[Exercise_Index][1])))

Problem_Indices = list(range(len(Problem_Set)))
random.shuffle(Problem_Indices)
Problem_Set_Shuffled = []
Problem_Key_Shuffled = []

for i in range(len(Problem_Indices)):
    Problem_Set_Shuffled.append(Problem_Set[Problem_Indices[i]])
    Problem_Key_Shuffled.append(Problem_Key[Problem_Indices[i]])
Problem_Key_Shuffled = [item.replace("Exercise ","") for item in Problem_Key_Shuffled]

#### Output problems and key into a LaTeX format ####

Problem_Set_String = ''

for Problem in Problem_Set_Shuffled:
    Problem_Set_String += '&' + Problem + "&\\\\\n"

Problem_Set_String = Problem_Set_String[:-3] # Strip the last \\
Problem_Set_String += '\n'

Output_Set = Problem_Start + Problem_Set_String + Problem_End

with open("Problem_Set.tex", "w") as of:
    of.write(Output_Set)



Problem_Key_String = ''

for Problem in Problem_Key_Shuffled:
    Problem_Key_String += '&' + Problem + "&\\\\\n"

Problem_Key_String = Problem_Key_String[:-3] # Strip the last \\
Problem_Key_String += '\n'

Output_Key = Problem_Key_Start + Problem_Key_String + Problem_End

with open("Problem_Key.tex", "w") as of:
    of.write(Output_Key)
