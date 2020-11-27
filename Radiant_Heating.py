import os
import time
import csv
import matplotlib.pyplot as pyplot

## Physical constants ##

emissivity = 1.0
SB_constant = 5.670e-8
albedo = 0.3
a = emissivity * SB_constant
b = 1000

########



## Simulation parameters

save_folder = os.getenv("HOME") + "\\Documents\\Programming\\Python\\Radiant Heating\\"

P = 342
dt = 0.001
seconds = 2000
max_count = int(seconds / dt)
data_count = 100

T0 = 0

t_start = time.time()

########



## Functions ##

def dT(T):
    return (P * (1 - albedo) - a * T**4) * dt / b


def Tn(T):
    return T + dT(T)

########



## Calculate values and store##

Tf = (P * (1 - albedo) / a)**(1/4)

Temp = []
simulation_time = []

Temp.append(T0)
simulation_time.append(0)

print("Total iterations: ", max_count)

for n in range(1,max_count):
    Temp.append(Tn(Temp[n-1]))
    simulation_time.append(n * dt)
    if (n%1000000==0):
        print("Current iteration (millions):", int(n/1000000), "\n\tEstimated time remaining: ", int(round((max_count / n - 1) * (time.time() - t_start), 0)))

print(max_count, " iterations complete, elapsed time: ", round(time.time() - t_start, 2), "seconds")

########



## Create plot ##

print("Creating plot.")

fig = pyplot.figure()
ax = fig.gca()
ax.plot(simulation_time, Temp)
ax.hlines(Tf, xmin=simulation_time[0], xmax=simulation_time[max_count-1], linestyles="dashed", colors="r")
ax.grid()
ax.set_ylim(ymin=0)
ax.set_xlim(xmin=0)
ax.set_xlabel("Time (s)")
ax.set_ylabel("Temperature (K)")
fig.suptitle("Radiant Heating of a Blackbody")
ax.set_title(label="Input Power = " + str(P) + " W, Final Temp = " + str(round(Tf,2)) +" K", fontsize=10)


########



## Export data ##

print("Exporting data.")

data = []
data.append(["Time", "Temp", "Final Temp", "P", "T0"])
data.append([simulation_time[0],Temp[0],Tf, P, T0])


if (max_count > data_count):
    skip = int(max_count/data_count)
    for n in range(1,data_count):
        data.append([simulation_time[int(n * skip)],Temp[int(n * skip)]])
else:
    for n in range(1,max_count):
        data.append([simulation_time[n],Temp[n]])

    
with open(save_folder + "Radiant Heating.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerows(data)


fig.savefig(save_folder + "Radiant Heating.png")
fig.show()

########
