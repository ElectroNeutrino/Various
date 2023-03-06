import numpy as np
from matplotlib import pyplot as pyplot

temperature = np.array([250, 300, 350, 5500])


h = 6.62607004e-34 # Plank constant (J * s)
c = 299792458 # speed of light (m / s)
kb = 1.38064952e-23 # Boltzmann constant (J / K)
sb = 5.670374419e-8 # Stefan-Boltzmann constant (W/(m^2 K^4))
wb = 2898 # Wien's displacement constant (µm * K)

max_x = np.amax(wb / temperature) * 4
x = np.linspace(max_x, 0, 5000, endpoint=False)


def Spectral_Radiance(l, T): # wavelength in µm, temperature in Kelvin
    m = l * 1e-6 # convert to µm
    a = 2 * h * c * c / np.power(m,5)
    b = 1 / (np.exp(h * c / (m * kb * T)) - 1)
    return a * b * 1e-6 # W / (sr * m^2 * µm)

y = []
for n in range(len(temperature)):
    y.append(Spectral_Radiance(x, temperature[n])/Spectral_Radiance(wb/temperature[n], temperature[n]))

fig = pyplot.figure()
ax = pyplot.axes(xlim=(0.1, max_x), ylim=(0, 1.1))
ax.set_xlabel("Wavelength (µm)")
ax.set_ylabel("Spectral Radiance Ratio")
ax.grid()
ax.set_xscale('log')

line = []
for n in range(len(y)):
    line.append(ax.plot(x, y[n], label=(str(temperature[n]) + " K"))[0])

#y.append(Spectral_Radiance(x, wb/x))
#line.append(ax.plot(x, y[-1], linestyle="dashed", label="Wien Peak")[0])

fig.legend(loc="upper right")
fig.tight_layout()

fig.savefig("Planck.png")

fig.show()
