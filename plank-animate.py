import numpy as np
from matplotlib import pyplot as pyplot
from matplotlib.animation import FuncAnimation


#ranges to model
max_temp = 400
max_x = 35



#Physical constants
h = 6.62607004e-34 # Plank constant (J * s)
c = 299792458 # speed of light (m / s)
kb = 1.38064952e-23 # Boltzmann constant (J / K)
wb = 2898 # Wien's displacement constant (µm * K)



#set up arrays to iterate through
temperature_range = np.flip(np.linspace(max_temp, 0, 40, endpoint=False))
x = np.linspace(max_x, 0, 500, endpoint=False)



#create canvas
fig = pyplot.figure()
ax = pyplot.axes()
ax.set_xlabel("Wavelength (µm)")
ax.set_ylabel("Spectral Radiance (W/sr*m^2*µm)")
line, = ax.plot([], [])
text = ax.text(0, 0,'')



#function definitions
def Spectral_Radiance(T): #temperature in Kelvin
    m = x * 1e-6 # convert from µm to m
    a = 2 * h * c * c / np.power(m,5)
    b = 1 / (np.exp(h * c / (m * kb * T)) - 1)
    return a * b * 1e-6 # W / (sr * m^2 * µm)

def anim_init():
    text.set_text("T= " + f'{0:g}' + " K")
    line.set_data([], [])
    return line, text

def animate(i):
    text.set_text("T= " + f'{i:g}' + " K")
    y = Spectral_Radiance(i)
    line.set_data(x, y)
    return line, text




#create animation
anim = FuncAnimation(fig,
                     animate,
                     init_func=anim_init,
                     frames=temperature_range,
                     interval=85,
                     blit=True)

ax.plot(x, Spectral_Radiance(wb/x), linestyle="dashed") #Wien curve



#format display
max_y = np.amax(Spectral_Radiance(max_temp))
text.set_x(max_x*0.75)
text.set_y(max_y*0.75)
ax.set_xlim(xmin=0, xmax=max_x)
ax.set_ylim(ymin=0, ymax=max_y*1.1)
ax.grid()
fig.tight_layout()




#output
anim.save("Planck_animated.gif")
pyplot.show()
