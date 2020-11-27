import math
import os
import matplotlib.pyplot as pyplot

## Initial conditions ##

r_0 = 1.225 # Initial density (kg/m^3)
T_0 = 288.15 # Initial temperature (K)

####



## Simulation settings ##

save_folder = os.getenv("HOME") + "\\Documents\\Programming\\Python\\Standard Atmosphere\\"
dh = 100

# range to simulate (m)
min_height = 0
max_height = 32000
max_count = int((max_height - min_height) / dh)

# lapse rates
k_1 = -0.0065
k_2 = 0
k_3 = 0.001

# layer height (m)
h_1 = 11000
h_2 = 20000
h_3 = 32000

####


## Physical constants (mks) ##

M_1 = 0.028964 # Molar mass of dry air (kg / mol)
R = 8.314 # Ideal Gas Constant (Pa * m^3) / (mol * K)
g = 9.81 # Acceleration due to gravity (m / s^2)

####



## Functions ##

def T1(h):
    return T_0 + k_1 * h

def T2(h):
    return T1(h_1) + k_2 * (h - h_1)

def T3(h):
    return T2(h_2) + k_3 * (h - h_2)

def T(h):
    if (h < 0):
        print("T(h): Height too low:.")
        reutrn -1
    elif (h <= h_1):
        return T1(h)
    elif (h <= h_2):
        return T2(h)
    elif (h <= h_3):
        return T3(h)
    else:
        print("T(h): Height too high.")
        return -1

def Density_Calc(h):


    if (h > h_3):
        print("Density_Calc(h): Height too high.")
        reutrn -1
    elif (h > h_2): # Stratosphere
        M = M_1
        k = k_3
        Ti = T(h_2)
        Temp = T3(h)
        rho_0 = Density_Calc(h_2)
        delta_h = h - h_2
    elif (h > h_1): # Tropopause
        M = M_1
        k = k_2
        Ti = T(h_1)
        Temp = T2(h)
        rho_0 = Density_Calc(h_1)
        delta_h = h - h_1
    elif (h >= 0): # Troposphere
        M = M_1
        k = k_1
        Ti = T_0
        Temp = T1(h)
        rho_0 = r_0
        delta_h = h
    else:
        print("Density_Calc(h): Height too low.")
        return -1


    if ((Temp <= 0) or (Ti <= 0)): # Temperature cannot be negative
        print("Density_Calc(h): Temp negative or zero.")
        return -1
    
    alpha = M * g / R

    if (k == 0):
        S = (delta_h) / Ti
    else:
        S = math.log(Temp / Ti) / k

    rho = rho_0 * (Ti / Temp) * math.exp(-alpha * S)
    return rho

def Sig_Figs(x, figs=3):
    sf = str(round(x, figs - int(math.floor(math.log10(abs(x)))) - 1))
    return sf

####



## Calculate values ##

# notable points
r_1 = Density_Calc(h_1)
T_1 = T(h_1)
P_1 = (r_1 * R * T_1 / M_1) / 1000
r_2 = Density_Calc(h_2)
T_2 = T(h_2)
P_2 = (r_2 * R * T_2 / M_1) / 1000
r_3 = Density_Calc(h_3)
T_3 = T(h_3)
P_3 = (r_3 * R * T_3 / M_1) / 1000

# storage arrays
data = []
height = []
density = []
pressure = []
temperature = []

for n in range(max_count):
    height_val = n * dh
    density_val = Density_Calc(height_val)
    temperature_val = T(height_val)
    pressure_val = (density_val * R * temperature_val / M_1) / 1000
    height.append(height_val)
    density.append(density_val)
    pressure.append(pressure_val)
    temperature.append(temperature_val)
    data.append([height_val, density_val, pressure_val, temperature_val])

####



## Create plot ##

print("Creating plot.")

# create plot area
fig, (ax1, ax2, ax3) = pyplot.subplots(1,3, sharey=True, figsize=(12, 10))

# add labels
fig.suptitle("Standard Atmosphere")
ax1.set_xlabel("Density (kg/m^2)")
ax2.set_xlabel("Pressure (kPa)")
ax3.set_xlabel("Temperature (K)")
ax1.set_ylabel("Height (m)")

# add data
ax1.plot(density, height, label="Density")
ax2.plot(pressure, height, label="Pressure")
ax3.plot(temperature, height, label="Temperature")

# format plots
for ax in (ax1, ax2):
    ax.set_ylim(ymin=0, ymax=max_height)
    ax.set_xlim(xmin=0)
    ax.grid()
    ax.hlines((h_1, h_2), xmin=0, xmax=ax.get_xlim()[1], linestyles="dashed", colors="r")

ax2.yaxis.set_ticks_position("none")
ax3.yaxis.set_ticks_position("none")

ax3.set_ylim(ymin=0, ymax=max_height)
ax3.set_xlim(xmin=200)
ax3.grid()
ax3.hlines((h_1, h_2), xmin=ax3.get_xlim()[0], xmax=ax3.get_xlim()[1], linestyles="dashed", colors="r")

# add notable values
dax1 = ax1.get_xlim()[1] - ax1.get_xlim()[0]
dax2 = ax2.get_xlim()[1] - ax2.get_xlim()[0]
dax3 = ax3.get_xlim()[1] - ax3.get_xlim()[0]
ax3_x_min = ax3.get_xlim()[0]

tag_ratio = 0.6

ax1.text(dax1 * tag_ratio, 9000, "Troposphere\n" + r"$\rho$="   + Sig_Figs(r_1, 3))
ax1.text(dax1 * tag_ratio, 18000, "Tropopause\n" + r"$\rho$="   + Sig_Figs(r_2, 3))
ax1.text(dax1 * tag_ratio, 30000, "Stratosphere\n" + r"$\rho$=" + Sig_Figs(r_3, 3))

ax2.text(dax2 * tag_ratio, 9000, "Troposphere\nP=" + Sig_Figs(P_1, 3))
ax2.text(dax2 * tag_ratio, 18000, "Tropopause\nP=" + Sig_Figs(P_2, 3))
ax2.text(dax2 * tag_ratio, 30000, "Stratosphere\nP=" + Sig_Figs(P_3, 3))

ax3.text(dax3 * tag_ratio + ax3_x_min, 9000, "Troposphere\nT=" + Sig_Figs(T_1, 5))
ax3.text(dax3 * tag_ratio + ax3_x_min, 18000, "Tropopause\nT=" + Sig_Figs(T_2, 5))
ax3.text(dax3 * tag_ratio + ax3_x_min, 30000, "Stratosphere\nT=" + Sig_Figs(T_3, 5))

pyplot.tight_layout()

fig.savefig(save_folder + "Standard Atmosphere.png")

pyplot.show()

########
