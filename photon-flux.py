import math

F_0 = 100 # photon flux for magnitude 0 star, photons s^-1 mm^-2 nm^-1 

Aperature = float(input("Aperature (mm):  ")) # objective aperature in mm

BandPass = float(input("Filter bandpass (nm):  ")) # filter bandwidth in nm

Magnitude = float(input("Magnitude:  "))

F = (math.pi/4.0) * F_0 * (Aperature ** 2) * BandPass * math.pow(10, -Magnitude / 2.5)

print('\n\n' + f'{F:.4}' + '  photons/s')

input("\n\nPress enter to exit.")
