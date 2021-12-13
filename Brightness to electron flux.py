import math

F_0 = 100 # photon flux for magnitude 0 star, photons s^-1 mm^-2 nm^-1 

f = float(input("f-ratio:  ")) # objective aperature in mm
pixel_size = float(input("Pixel size (microns):  ")) # pixel pitch of sensor in micrometers
quantum_efficiency = float(input("Quantumn efficiency ratio:  ")) # quantum efficiency of sensor
BandPass = float(input("Filter bandpass (nm):  ")) # filter bandwidth in nm
brightness = float(input("Brightness (mag / arcsec^2):  ")) # brightness in magnitude per arcsecond^2

e = 33415.0 * quantum_efficiency * F_0 * ((pixel_size ** 2) / (f ** 2)) * BandPass * math.pow(10, -brightness / 2.5)

print('\n\n' + f'{e:.3}' + '  electrons/pixel/s')

input("\n\nPress enter to exit.")
