import math
import os
import matplotlib.pyplot as pyplot

## Initial conditions ##

r_0 = 1.225 # Initial density (kg/m^3)
T_0 = 288.15 # Initial temperature (K)

########



## Simulation settings ##

save_folder = os.getenv("HOME") + "\\Documents\\Programming\\Python\\Standard Atmosphere\\"
dh = 100

# lapse rates
lapse_rate = [-0.0065,
              0,
              0.001,
              0.0028,
              0,
              -0.0028,
              -0.002,
              0]

# layer height (m)
layer = [0,
         11000,
         20000,
         32000,
         47000,
         51000,
         71000,
         80000,
         90000]

layer_label = ["Ground",
               "Troposphere",
               "Tropopause",
               "Stratosphere",
               "Stratosphere",
               "Stratopause",
               "Mesosphere",
               "Mesosphere",
               "Mesopause"]

# range to simulate (m)
min_height = min(layer)
max_height = max(layer)
max_count = int((max_height - min_height) / dh)

########



## Physical constants (mks) ##

M_1 = 0.028964 # Molar mass of dry air (kg / mol)
R = 8.314 # Ideal Gas Constant (Pa * m^3) / (mol * K)
g = 9.81 # Acceleration due to gravity (m / s^2)

########



## Functions ##

def Tn(Ti, h, hi, k):
    return Ti + k * (h - hi)


def T(h):
    if (h < 0):
        print("T(h): Height too low:.")
        return -1
    elif (h > max_height):
        print("T(h): Height too high.")
        return -1

    if (h <= layer[1]):
        k = lapse_rate[0]
        hi = layer[0]
        Ti = T_0
    elif (h <= layer[2]):
        k = lapse_rate[1]
        hi = layer[1]
        Ti = T(hi)
    elif (h <= layer[3]):
        k = lapse_rate[2]
        hi = layer[2]
        Ti = T(hi)
    elif (h <= layer[4]):
        k = lapse_rate[3]
        hi = layer[3]
        Ti = T(hi)
    elif (h <= layer[5]):
        k = lapse_rate[4]
        hi = layer[4]
        Ti = T(hi)
    elif (h <= layer[6]):
        k = lapse_rate[5]
        hi = layer[5]
        Ti = T(hi)
    elif (h <= layer[7]):
        k = lapse_rate[6]
        hi = layer[6]
        Ti = T(hi)
    elif (h <= layer[8]):
        k = lapse_rate[7]
        hi = layer[7]
        Ti = T(hi)
    return Tn(Ti, h, hi, k)


def Density_Calc(h):
    if (h > layer[8]):
        print("Density_Calc(h): Height too high.")
        return -1
    elif (h > layer[7]): # Mesosphere
        M = M_1
        k = lapse_rate[7]
        Ti = T(layer[7])
        rho_0 = Density_Calc(layer[7])
        delta_h = h - layer[6]
    elif (h > layer[6]): # Mesosphere
        M = M_1
        k = lapse_rate[6]
        Ti = T(layer[6])
        rho_0 = Density_Calc(layer[6])
        delta_h = h - layer[5]
    elif (h > layer[5]): # Mesosphere
        M = M_1
        k = lapse_rate[5]
        Ti = T(layer[5])
        rho_0 = Density_Calc(layer[5])
        delta_h = h - layer[5]
    elif (h > layer[4]): # Stratopause
        M = M_1
        k = lapse_rate[4]
        Ti = T(layer[4])
        rho_0 = Density_Calc(layer[4])
        delta_h = h - layer[4]
    elif (h > layer[3]): # Upper Stratosphere
        M = M_1
        k = lapse_rate[3]
        Ti = T(layer[3])
        rho_0 = Density_Calc(layer[3])
        delta_h = h - layer[3]
    elif (h > layer[2]): # Lower Stratosphere
        M = M_1
        k = lapse_rate[2]
        Ti = T(layer[2])
        rho_0 = Density_Calc(layer[2])
        delta_h = h - layer[2]
    elif (h > layer[1]): # Tropopause
        M = M_1
        k = lapse_rate[1]
        Ti = T(layer[1])
        rho_0 = Density_Calc(layer[1])
        delta_h = h - layer[1]
    elif (h >= layer[0]): # Troposphere
        M = M_1
        k = lapse_rate[0]
        Ti = T_0
        rho_0 = r_0
        delta_h = h - layer[0]
    else:
        print("Density_Calc(h): Height too low.")
        return -1

    Temp = T(h)
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

########



## Calculate values ##

# notable points
note_density = []
note_temp = []
note_pressure = []
for n in range(len(layer)):
    density_val = Density_Calc(layer[n])
    note_density.append(density_val)
    temperature_val = T(layer[n])
    note_temp.append(temperature_val)
    pressure_val = (density_val * R * temperature_val / M_1) / 1000
    note_pressure.append(pressure_val)

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

########



## Create plot ##

print("Creating plot.")

# create plot area
fig, (ax1, ax2, ax3) = pyplot.subplots(1,3, sharey=True, figsize=(12, 10))

# add labels
fig.suptitle("Standard Atmosphere")
ax1.set_xlabel("Density (kg/m^3)")
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
    ax.hlines((layer[1], layer[2], layer[4], layer[5], layer[7]), xmin=0, xmax=ax.get_xlim()[1], linestyles="dashed", colors="r")

ax2.yaxis.set_ticks_position("none")
ax3.yaxis.set_ticks_position("none")

ax3.set_ylim(ymin=0, ymax=max_height)
ax3.grid()
ax3.hlines((layer[1], layer[2], layer[4], layer[5], layer[7]), xmin=ax3.get_xlim()[0], xmax=ax3.get_xlim()[1], linestyles="dashed", colors="r")

# add notable values
dax1 = ax1.get_xlim()[1] - ax1.get_xlim()[0]
dax2 = ax2.get_xlim()[1] - ax2.get_xlim()[0]
dax3 = ax3.get_xlim()[1] - ax3.get_xlim()[0]
ax3_x_min = ax3.get_xlim()[0]
ax3_x_max = ax3.get_xlim()[1]

tag_ratio = 0.65 #left/right position of the text
line_adjust = -0.0353 * max_height # adjust text to be just below layer marker

for n in (1, 2, 4, 5, 7, 8):
    ax1.text(tag_ratio * dax1, layer[n] + line_adjust, layer_label[n] + "\n" + r"$\rho$="   + Sig_Figs(note_density[n], 3))
    ax2.text(tag_ratio * dax2, layer[n] + line_adjust, layer_label[n] + "\nP=" + Sig_Figs(note_pressure[n], 3))
    if ((n == 4) or (n == 5)):
        ax3.text(ax3_x_max - tag_ratio * dax3, layer[n] + line_adjust, layer_label[n] + "\nT=" + Sig_Figs(note_temp[n], 5))
    else:
        ax3.text(ax3_x_min + tag_ratio * dax3, layer[n] + line_adjust, layer_label[n] + "\nT=" + Sig_Figs(note_temp[n], 5))

pyplot.tight_layout()

fig.savefig(save_folder + "Standard Atmosphere.png")

pyplot.show()

########
