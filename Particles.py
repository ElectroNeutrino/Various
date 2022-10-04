import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


## Globals

epsilon=sys.float_info.epsilon
particle_limit = 500
n_bins = (lambda x: 20 if x > 20 else x)(particle_limit)
hist_smoothing = 10
dt = 0.01
radius = 1/particle_limit
particle_list = []
frame_limit = 200
hist_x_max = 3
save = True


## Class for particle creation
class Particles():
    def __init__(self, x_pos_in=0, y_pos_in=0, x_vel_in=0, y_vel_in=0, mass_in=1):
        self.x_pos, self.y_pos = x_pos_in, y_pos_in
        self.x_vel, self.y_vel = x_vel_in, y_vel_in
        self.mass = mass_in



## Called each frame to update particle positions
def Particle_Move(current_frame,step=dt):
    global particle_list # declare for editing within function
    global ps1, ps2, ps3, particle_energies

    #provide periodic update in console for sanity
    if ((current_frame+1)%5 == 0 and save == True):
        print("Current frame:", current_frame+1, "of", frame_limit)

    #initialise this frame with current step value and scratchpad particle list
    time_step = step
    prev_ts = step
    elapsed = 0
    collision_count = 0


    #loop over all collisions until the entire time step is checked
    while elapsed < step:
        collision = False

        #check every particle
        for i,particle in enumerate(particle_list):

            # Find if time to next collision is within time step
            
            for j,p2 in enumerate(particle_list):#check for potential collision with other particles, exclude self-collision
                if i == j:
                    continue
                if Get_Distance(particle,p2) < 2*radius - epsilon: #exit if particles overlap
                    print("Particles", (i,j), "overlapping.")
                    print("\tDistance:", Get_Distance(particle, p2))
                    print("\tFrame:", current_frame)
                    print("\tUndoing time step", prev_ts)
                    Collide(i, j)
                    Update_Positions(current_frame, -prev_ts)
                    sys.exit()
                tc = Collision_Check(particle, p2, step)
                if tc < time_step: #update time step if time to collision is less than current time step
                    time_step = tc
                    collision = True
                    pc1, pc2 = i, j

            tw = Wall_Check(particle) #check when particle will collide with wall
            if tw <= time_step: #update time step if time to collision is less than current time step
                time_step = tw
                collision = True
                pc1, pc2 = i, -1
            
        #update particle positions by time_step
        Update_Positions(current_frame,time_step)

        #handle any collisions
        if collision:
            collision = False
            Collide(pc1, pc2)
            collision_count += 1

        if collision_count > particle_limit*particle_limit:
            print("Too many collisions in a single frame.")
            sys.exit()
        #update total elapsed time during frame
        prev_ts = time_step
        elapsed += time_step
        time_step = step - elapsed

    #update position array
    particle_positions = [[p.x_pos for p in particle_list],
                          [p.y_pos for p in particle_list]]

    #update running average of velocities
    pe.pop(0)
    pe.append(particle_energies[:])
    #update kinetic energies
    particle_energies = [p.mass*(p.x_vel*p.x_vel + p.y_vel*p.y_vel)/2 for p in particle_list]
    pe_avg = [sum(p[i] for p in pe)/len(pe) for i in range(particle_limit)]
    energy_avg = sum(pe_avg)/len(pe_avg)

    #update plot
    scatter_plot.set_offsets(list(zip(particle_positions[0], particle_positions[1])))
    ax2.cla()
    histrogram_plot = ax2.hist(pe_avg, bins=np.linspace(0,hist_x_max,n_bins))
    ke_avg_line = ax2.axvline(energy_avg, color="red")
    ax2.plot(np.linspace(0,hist_x_max,40), (lambda x: particle_limit*(1-np.exp(-3/(20*energy_avg)))*np.exp(-x/energy_avg))(np.linspace(0,hist_x_max,40)))
    ax2.set_xlim((0,hist_x_max))
    ax2.set_ylim((0, np.ceil(particle_limit*(1-np.exp(-3/(20*energy_avg))))))
    return


## Check if two particles will collide within the specified time
def Collision_Check(p1, p2, step=dt):

    #find relative positions and velocities
    delta_x = p2.x_pos - p1.x_pos
    delta_y = p2.y_pos - p1.y_pos
    delta_vx = p2.x_vel - p1.x_vel
    delta_vy = p2.y_vel - p1.y_vel

    #a = dv^2
    #b/2 = -dv * dr
    #c = dr^2
    dv2 = delta_vx * delta_vx + delta_vy * delta_vy
    dvdr = delta_x * delta_vx + delta_y * delta_vy
    dr2 = delta_x * delta_x + delta_y * delta_y

    #return double max step time if particles too far from eachother to collide or not moving towards eachother
    if ((np.sqrt(dr2) - 2*radius) > np.sqrt(dv2)*step) or dvdr >=0:
        return 2*step
    elif Get_Distance(p1,p2) <= 2*radius:
        return 0

    # simplify for quadratic equation
    #(-b+-sqrt(b^2-4ac))/2a ->
    #-p +- sqrt(p^2 - q)
    p = dvdr/dv2
    q = (dr2 - 4*radius*radius)/dv2

    #return double max if solution is imaginary (implies no collision)
    if p*p < q:
        return 2*step

    #find solutions
    t1 = -p - np.sqrt(p*p - q)
    t2 = -p + np.sqrt(p*p - q)

    #return minimum non-zero number
    ret_time = min([t for t in [t1,t2, 2*step] if t>0])
    return ret_time


## Check the least non-zero time for a particle to collde with a wall
def Wall_Check(p1):
    t_wall = []
    #get time to closest vertical wall
    if p1.x_vel < 0:
        t_wall.append((radius-p1.x_pos)/p1.x_vel)
    elif p1.x_vel > 0:
        t_wall.append((1-radius-p1.x_pos)/p1.x_vel)

    #get time to closest horizontal wall
    if p1.y_vel < 0:
        t_wall.append((radius-p1.y_pos)/p1.y_vel)
    elif p1.y_vel > 0:
        t_wall.append((1-radius-p1.y_pos)/p1.y_vel)
    t_wall.append(2*dt)
    return min([t for t in t_wall if 0<=t])

## Update velocities of one or two particles after a collision
def Collide(pc1, pc2):
    p1 = particle_list[pc1]
    #particle-wall collision
    if pc2 == -1:
        d_wall_x = min([p1.x_pos, 1-p1.x_pos])
        d_wall_y = min([p1.y_pos, 1-p1.y_pos])
        if d_wall_x < d_wall_y:
            p1.x_vel *= -1
        elif d_wall_y < d_wall_x:
            p1.y_vel *= -1
        else:
            p1.x_vel *= -1
            p1.y_vel *= -1
    #particle-particle collision
    else:
        p2 = particle_list[pc2]

        #get relative positions
        dr_x, dr_y = p2.x_pos - p1.x_pos, p2.y_pos - p1.y_pos
        #get center of mass velocity
        v_x_avg = (p1.mass*p1.x_vel + p2.mass*p2.x_vel)/(p1.mass + p2.mass)
        v_y_avg = (p1.mass*p1.y_vel + p2.mass*p2.y_vel)/(p1.mass + p2.mass)
        #get velocities relative to center of mass
        v1_x, v1_y = p1.x_vel - v_x_avg, p1.y_vel - v_y_avg
        v2_x, v2_y = p2.x_vel - v_x_avg, p2.y_vel - v_y_avg

        #get the scaled normal in reference to particle 1
        dr_2 = dr_x*dr_x + dr_y*dr_y
        nr_x, nr_y = dr_x/dr_2, dr_y/dr_2

        #get the velocities parallel to the normal
        v1_p = dr_x*v1_x + dr_y*v1_y
        v2_p = dr_x*v2_x + dr_y*v2_y
        v1p_x, v1p_y = v1_p*nr_x, v1_p*nr_y
        v2p_x, v2p_y = v2_p*nr_x, v2_p*nr_y

        #reverse relative parallel velocities and adjust to absolute velocities
        p1.x_vel, p1.y_vel = p1.x_vel - 2*v1p_x, p1.y_vel - 2*v1p_y
        p2.x_vel, p2.y_vel = p2.x_vel - 2*v2p_x, p2.y_vel - 2*v2p_y
    return

## Update particle positions by time_step
def Update_Positions(current_frame, time_step = dt):
    for i,particle in enumerate(particle_list):
        px, py = particle.x_pos + time_step * particle.x_vel, particle.y_pos + time_step * particle.y_vel
        #sanity check that particle is not outside of bounds
        if min([px-radius, py-radius]) < -epsilon or max([px+radius, py+radius]) > 1 + epsilon:
            print("Particle", i, "outside of bounds.")
            print("\tPrevious:", (particle.x_pos, particle.y_pos))
            print("\tCurrent:", (px, py))
            print("\tVelocity:", (particle.x_vel, particle.y_vel))
            print('\tTime Step:', time_step)
            print("\tFrame:", current_frame)
            sys.exit()
        particle.x_pos, particle.y_pos = px, py

## Find the distance between two particles
def Get_Distance(p1, p2):
    delta_x = p2.x_pos - p1.x_pos
    delta_y = p2.y_pos - p1.y_pos
    return np.sqrt(delta_x * delta_x + delta_y * delta_y)
    

## Populate array of particles
for new_particle in range(particle_limit):
    create_particle = True
    while create_particle:
        x_pos, y_pos = (1-2*radius)*np.random.random_sample() + radius, (1-2*radius)*np.random.random_sample() + radius
        x_vel, y_vel = 2*np.random.random_sample()-1, 2*np.random.random_sample()-1
        p_add = Particles(x_pos, y_pos, x_vel, y_vel)
        create_particle = False
        for particle in particle_list: #re-initialize current particle if it's overlapping another
            if Get_Distance(particle,p_add) < 2*radius:
                create_particle = True
    particle_list.append(p_add)



## Initialize data
particle_positions = [[p.x_pos for p in particle_list],
                      [p.y_pos for p in particle_list]]
particle_energies = [p.mass*(p.x_vel*p.x_vel + p.y_vel*p.y_vel)/2 for p in particle_list]
energy_avg = sum(particle_energies)/len(particle_energies)
pe = []
for i in range(hist_smoothing):
    pe.append(particle_energies[:])


## Initialize visuals
fig = plt.figure(figsize=(10,6))
ax1 = fig.add_subplot(1,3,(1,2), xlim=(0,1),ylim=(0,1))
ax2 = fig.add_subplot(1,3,3, xlim=(0,3))
ax1.set_xticks([])
ax1.set_yticks([])

#particles
scatter_plot=ax1.scatter(particle_positions[0], particle_positions[1], s=10)
#histogram
histrogram_plot = ax2.hist(particle_energies, bins=np.linspace(0,hist_x_max,n_bins))
#average energy
ke_avg_line = ax2.axvline(energy_avg, color="red")
#normalised MB-statistics N = N0 * A * e^(-x/E), A = 1 - e^(x1/E)
ax2.plot(np.linspace(0,hist_x_max,40), (lambda x: particle_limit*(1-np.exp(-hist_x_max/(n_bins*energy_avg)))*np.exp(-x/energy_avg))(np.linspace(0,hist_x_max,40)))
ax2.set_xlim((0,hist_x_max))
ax2.set_ylim((0, np.ceil(particle_limit*(1-np.exp(-3/(20*energy_avg))))))

## Run for a few steps to stabilize
print("Stabilizing.")
stab_limit = 20
for i in range(stab_limit):
    print("\t", i+1, "of", stab_limit)
    Particle_Move(0)

print("Stablising complete, creating animation.")
## Create graphics
anim = FuncAnimation(fig, Particle_Move, frames=frame_limit, interval=10)
if save:
    anim.save("Particles.gif", fps=25, dpi=600/3)
else:
    plt.show()

