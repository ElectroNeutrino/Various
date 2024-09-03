import random
import numpy as np
import scipy
import matplotlib.pyplot as pyplot
from matplotlib.patches import Polygon
from mpl_toolkits.mplot3d import axes3d
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from matplotlib import colormaps as cm


### Config ###

# Constants and globals
x_range = (-1.0,1.0)
y_range = (-1.0,1.0)
z_range = (-1.0,1.0)
node_count = 50
max_edge_swap_loops_count = 10
nudge_loop_max = 5
refinement_loop_max = 5
square_boundary = False


### Functions ###

# Parametric function that defines the boundary line if not square
def Boundary(s):
    return 0.8 * np.cos(2*np.pi*s), 0.6 * np.sin(2*np.pi*s)

# Function f(x,y) for the forcing of the elliptic PDE (-div^2)z = f
def Forcing(x, y):
    return (-1.0)

# Function to compute the load vector
# Coord transform to l,h
def Load(l, h, p0, p1, p2, triangle_area):
    dp = p2-p1
    p = (p1 + dp*l)*(1-h) + p0*h
    return (node_count*node_count*2.0*triangle_area*(1-h)*h)*Forcing(p[0], p[1])

# Take a triangle and find the circle that passes through all three corners
# by soliving a linear equation of all three points
def CircumscribedCircleFromPoints(triangle):
    point1, point2, point3 = triangle
    x1, y1 = point1
    x2, y2 = point2
    x3, y3 = point3

    # Return NaN,inf if the points are all colinear (within precision)
    if (np.absolute(np.linalg.det(np.array([[(x2-x1),(x3-x2)],[(y2-y1),(y3-y2)]]))) < 1e-14):
        return np.nan,np.nan,np.inf

    # find center coordinates and radius of circle
    M = np.array([[x1, y1, 1],[x2, y2,1],[x3, y3, 1]])
    V = np.array([np.inner(point1,point1),np.inner(point2,point2),np.inner(point3,point3)])
    A,B,C = np.linalg.solve(M,V)
    x_0, y_0 = A/2.0, B/2.0
    r_0 = np.sqrt(C + x_0*x_0 + y_0*y_0)
    
    return x_0, y_0, r_0

# Take two points from triangle and compare cross product of that with the test point and cross product with the third point
# (excludes triangle vertices
def IsPointInTriangle(point, triangle):
    # get each corner in the triangle
    triangle_point1, triangle_point2, triangle_point3 = np.array(TriangleToPoints(triangle))

    # Line vectors between triangle corners
    line13 = triangle_point1-triangle_point3
    line21 = triangle_point2-triangle_point1
    line32 = triangle_point3-triangle_point2

    # Line vectors from triangle corner to point
    line10 = triangle_point1-np.array(point)
    line20 = triangle_point2-np.array(point)
    line30 = triangle_point3-np.array(point)

    # Get cross products of the point with each pair of triangle corners
    c1 = np.cross(line20, line30)
    c2 = np.cross(line30, line10)
    c3 = np.cross(line10, line20)

    # Get cross products of one triangle corner with each pair of the others
    d1 = np.cross(line21, -line13)
    d2 = np.cross(line32, -line21)
    d3 = np.cross(line13, -line32)

    # Point is inside if the signs of the point cross product match the sign of the
    # corresponding triangle corner cross product
    return ((c1*d1>0) and (c2*d2>0) and (c3*d3>0))

# Check if a point is on the edge of a triangle
def IsPointOnTriangle(point, triangle):
    triangle_point1, triangle_point2, triangle_point3 = np.array(TriangleToPoints(triangle))

    # Line vectors between triangle corners
    line13 = triangle_point1-triangle_point3
    line21 = triangle_point2-triangle_point1
    line32 = triangle_point3-triangle_point2

    on_corner = (np.array_equal(point,triangle_point1) or np.array_equal(point,triangle_point2) or np.array_equal(point,triangle_point3))

    # Check if x-coordinate of point is between at least one pair of points
    is_inbetween_x = ((point[0]>triangle_point1[0]) != (point[0]>triangle_point3[0])) or ((point[0]>triangle_point2[0]) != (point[0]>triangle_point1[0])) or ((point[0]>triangle_point3[0]) != (point[0]>triangle_point2[0]))
    # Check if y-coordinate of point is between at least one pair of points
    is_inbetween_y = ((point[1]>triangle_point1[1]) != (point[1]>triangle_point3[1])) or ((point[1]>triangle_point2[1]) != (point[1]>triangle_point1[1])) or ((point[1]>triangle_point3[1]) != (point[1]>triangle_point2[1]))

    if on_corner or not (is_inbetween_x and is_inbetween_y):
        return False,[]

    if (line13[0]!=0.0):
        m13 = line13[1]/line13[0]
        on_line13 = (np.absolute(m13*(point[0]-triangle_point1[0]) + triangle_point1[1] - point[1]) < 1e-15)
    else:
        on_line13 = (point[0]==triangle_point1[0])
    if (line21[0]!=0.0):
        m21 = line21[1]/line21[0]
        on_line21 = (np.absolute(m21*(point[0]-triangle_point2[0]) + triangle_point2[1] - point[1]) < 1e-15)
    else:
        on_line21 = (point[0]==triangle_point2[0])
    if (line32[0]!=0.0):
        m32 = line32[1]/line32[0]
        on_line32 = (np.absolute(m32*(point[0]-triangle_point3[0]) + triangle_point3[1] - point[1]) < 1e-15)
    else:
        on_line32 = (point[0]==triangle_point3[0])
    if on_line13:
        return True,[triangle[0],triangle[2]]
    elif on_line21:
        return True,[triangle[1],triangle[0]]
    elif on_line32:
        return True,[triangle[2],triangle[1]]
    else:
        return False,[]

def NodeOnBoundary(node_index):
    for entry in boundary_points:
        if ((node_list[node_index][0] == entry[0]) and (node_list[node_index][1] == entry[1])):
            return True
    return False

# Take a node index and return a point
def NodeToPoint(node):
    return node_list[node]

# Take a triangle (array of 3 node indices) and return points
def TriangleToPoints(triangle):
    point1 = NodeToPoint(triangle[0])
    point2 = NodeToPoint(triangle[1])
    point3 = NodeToPoint(triangle[2])
    return [point1, point2, point3]

# Find if two triangles share an edge
def TrianglesAdjecent(triangle1, triangle2):
    shared_corners = [corner for corner in triangle1 if corner in triangle2]
    return (len(shared_corners)==2)

# Split a triangle into three more at a point
def SplitTriangle(node, triangle):

    on_triangle, line_on = IsPointOnTriangle(NodeToPoint(node), triangle)
    
    if IsPointInTriangle(NodeToPoint(node), triangle):
        new_triangles = [[node, triangle[0], triangle[1]],
                         [node, triangle[1], triangle[2]],
                         [node, triangle[2], triangle[0]]]
    elif on_triangle:
        node_not_on = [node for node in triangle if node not in line_on][0]
        new_triangles = [[node, node_not_on, line_on[0]],
                         [node, node_not_on, line_on[1]]]
    else:
        new_triangles = [triangle]

    return new_triangles

# Calculate triangle areas
def CalcTriangleArea(triangle):
# area = (x0*(y1-y1) + x1*(y2-y0) + x2*(y0-y1)) / 2
    p0, p1, p2 = TriangleToPoints(triangle)
    triangle_area = (p0[0]*(p1[1]-p2[1]) + p1[0]*(p2[1]-p0[1]) + p2[0]*(p0[1]-p1[1]))/2.0
    return np.absolute(triangle_area)

# Delaunay process for checking triangle quality
def DelaunayCheck(new_triangle):
    x_0, y_0, r_0 = CircumscribedCircleFromPoints(TriangleToPoints(new_triangle))
    new_triangle_index = triangle_list.index(new_triangle)
    adjacent_triangle_indices = [triangle_list.index(triangle) for triangle in triangle_list
                                 if TrianglesAdjecent(new_triangle, triangle)]
    # Go through each triangle adjacent to current new triangle
    triangle_count = 0
    for triangle_index in adjacent_triangle_indices:
        triangle = triangle_list[triangle_index]
        shared_nodes = [node for node in new_triangle if node in triangle]
        not_shared = [node for node in triangle if node not in shared_nodes][0]
        # Check if non-shared node of adjacent triangle is within circumscribed triangle, swap edges if so
        dx = node_list[not_shared][0]-x_0
        dy = node_list[not_shared][1]-y_0
        if (((np.linalg.norm((dx,dy))-r_0)<-1e-15) and (new_triangle in triangle_list)):
            if SwapTriangleEdges(new_triangle,triangle):
                triangle_count = triangle_count + 1
    return triangle_count

# Swap the edges between two adjacent triangles
def SwapTriangleEdges(triangle1, triangle2):
    if TrianglesAdjecent(triangle1, triangle2):
        triangle1_index = triangle_list.index(triangle1)
        triangle2_index = triangle_list.index(triangle2)
        shared_nodes = [node for node in triangle1 if node in triangle2]
        not_shared_nodes = [node for node in (triangle1+triangle2) if node not in shared_nodes]
        t1 = [not_shared_nodes[0], not_shared_nodes[1], shared_nodes[0]]
        t2 = [not_shared_nodes[0], not_shared_nodes[1], shared_nodes[1]]
        del triangle_list[triangle1_index]
        triangle_list.insert(triangle1_index,t1)
        del triangle_list[triangle2_index]
        triangle_list.insert(triangle2_index,t2)
        return True
    return False

# Add points from array to mesh using Delaunay process to check adjacent triangles as each point is added
def AddPoints(points):
    point_count = 0
    points_to_add = len(points)
    for point in points:
        point_count = point_count + 1
        if ((point_count%50)==0):
            print("\t\tAdding point {} out of {}.".format(point_count, points_to_add))
        new_triangles_list = []
        for triangle in triangle_list:
            if IsPointInTriangle(point,triangle):# or IsPointOnTriangle(point,triangle)[0]:
                node_list.append(np.array(point))
                # Get index and get new triangles after splitting
                triangle_index = triangle_list.index(triangle)
                new_triangles_list = SplitTriangle(len(node_list)-1,triangle)
                if len(new_triangles_list)>0:
                    # Remove triangle at index and insert new triangles
                    del triangle_list[triangle_index]
                    for new_triangle in new_triangles_list[::-1]:
                        triangle_list.insert(triangle_index,new_triangle)
                    # exit triangle loop after adding new triangles to start using updated list
                    break
        # Go through each new triangle
        for new_triangle in new_triangles_list:
            DelaunayCheck(new_triangle)


### Make triangles ###

print("Making triangles.")
if square_boundary:
    # Create all nodes in an NxN array of points
    node_list = [np.array((p, q)) for q in np.linspace(-1.0, 1.0, node_count, endpoint=True)
                        for p in np.linspace(-1.0, 1.0, node_count, endpoint=True)]

    # List which nodes are on the boundary
    boundary_points = [node for node in node_list if ((node[0]*node[0])==1.0 or (node[1]*node[1])==1.0)]

    # Create list of triangles from node indices (go from [i][j] -> index during list comprehension)
    triangle_list = ([[i + j*node_count, (i+1) + j*node_count, (i+1) + (j+1)*node_count] for i in range(node_count-1) for j in range(node_count-1)] +
                     [[i + j*node_count, (i+1) + (j+1)*node_count, i + (j+1)*node_count] for i in range(node_count-1) for j in range(node_count-1)])
    print("\tTotal nodes and triangles: nodes={}, triangles={}".format(len(node_list), len(triangle_list)))
    
else:
    # Create parameter space for boundary lines
    L = np.linspace(0, 1.0, 2*node_count, endpoint=False)

    # Create list of boundary points
    boundary_points = np.array([(Boundary(s)[0], Boundary(s)[1]) for s in L])

    # Create list of (x,y) coordinates for grid of nodes
    min_distance = (np.sqrt(2.0)/node_count)
    grid_points = [(p, q) for q in np.linspace(-1.0, 1.0, node_count, endpoint=False)
                                    for p in np.linspace(-1.0, 1.0, node_count, endpoint=False)
                                        if len([boundary_point for boundary_point in boundary_points
                                        if np.linalg.norm((boundary_point[0]-p,boundary_point[1]-q))<min_distance])==0]

    # Create initial node list
    node_list = [np.array((-1.5,-1.5)),np.array((1.5,-1.5)),np.array((1.5,1.5)),np.array((-1.5,1.5))]

    # Create initial triangle list
    triangle_list = [[0,1,2],[0,2,3]]

    print("\tAdding boundary nodes.")
    # Add each point in the boundary
    AddPoints(boundary_points)

    # Remove scaffolding triangles from list (triangles connected to initial node list)
    for t_index in reversed(range(len(triangle_list))):
        triangle = triangle_list[t_index]
        if ((0 in triangle) or (1 in triangle) or (2 in triangle) or (3 in triangle)):
            del triangle_list[t_index]

    # Remove initial nodes from node list and update the triangle list with new node values
    for i in range(4):
        del node_list[0]
        for j in range(len(triangle_list)):
            for k in range(len(triangle_list[j])):
                triangle_list[j][k] = triangle_list[j][k] - 1
                if triangle_list[j][k] < 0:
                    print("Error removing node {}, triangle {}.".format(i, [j,k]))
                    exit()


    # Add grid points
    print("\tAdding grid nodes.")
    AddPoints(grid_points)
    print("\tTotal nodes and triangles after cleanup: nodes={}, triangles={}".format(len(node_list), len(triangle_list)))


internal_indices = [i for i in range(len(node_list)) if not NodeOnBoundary(i)]

# Refine triangles
print("\tRefining triangles.")
for refining_loop_index in range(refinement_loop_max):
    # Loop through and swap edges on bad quality triangle pairs
    print("\t\tEdge Swapping loops.")
    for loop_count in range(max_edge_swap_loops_count):
        changed_count = 0
        for triangle in triangle_list:
            changed_count = changed_count + DelaunayCheck(triangle)
        loop_count = loop_count + 1
        if (changed_count == 0):
            print("\t\t\tSwapping loop {} completed, 0 triangle pairs changed.".format(loop_count, changed_count))
            break
        if (loop_count%5)==0:
            if changed_count==1:
                plural = ''
            else:
                plural = 's'
            print("\t\t\tSwapping loop {} completed, {} triangle pair{} changed.".format(loop_count, changed_count, plural))

    # Nudge points to improve mesh quality
    print("\t\tNudging node positions.")
    for nudging_loop_index in range(nudge_loop_max):
        # For each node, get center of the triangles connected to it and move it there
        refining_index_array = internal_indices
        # Shuffle to even out systematic effects
        random.shuffle(refining_index_array)
        for node in refining_index_array:
            shared_triangles = [triangle for triangle in triangle_list if node in triangle]
            shared_nodes = list(set([triangle_node for triangle in shared_triangles for triangle_node in triangle if triangle_node!=node]))
            shared_points = np.array([node_list[node_from_shared] for node_from_shared in shared_nodes])
            shared_triangle_center = np.mean(shared_points, axis=0)
            node_list[node] = shared_triangle_center*0.1 + node_list[node]*0.9
        if ((nudging_loop_index+1)%10==0):
            print("\t\t\tNudging loop {} of {} complete.".format(nudging_loop_index+1, nudge_loop_max))
    print("\tRefinment loop {} complete.".format(refining_loop_index+1))

triangle_areas = []
for triangle_index in range(len(triangle_list)):
    triangle_areas.append(CalcTriangleArea(triangle_list[triangle_index]))


## Create basis for numeric modeling

# Create basis out of nodes not on the boundary
# basis_list[i][1] = node index for basis i
# basis_list[i][0][j][k] = k={0,1}: node indices for triangle j in basis i, k=2: index of triangle j
basis_list = []
# Go through each node that isn't on a boundary
# Use indices so that they are guaranteed presorted
for node_index in range(len(node_list)):
    if node_index in internal_indices:
        base_triangles = []
        # Go through each triangle
        for triangle_index in range(len(triangle_list)):
            # Check if the basis node is part of the triangle
            if node_index in triangle_list[triangle_index]:
                # Create list of the other two nodes in the triangle
                triangle_nodes = list(set(triangle_list[triangle_index]).difference([node_index]))
                # Add triangle index to end of node list
                triangle_nodes.append(triangle_index)
                base_triangles.append(triangle_nodes)
        basis_list.append([base_triangles,node_index])

# z function for each triangle of a basis
# z = 1 + (dz/dx)*(x-x0) + (dz/dy)*(y-y0)
# basis_func_list[i][j] = [dz/dx, dz/dy, triangle_index] for basis i, triangle j
basis_func_list = []
load_vector = []
# Go through each basis to find the basis functions and load vectors
# Use indices so that they are presorted
for base_index in range(len(basis_list)):
    base = basis_list[base_index]
    func_list = []
    b = 0
    p0 = NodeToPoint(base[1])
    # Go through each triangle in the basis
    for triangle in base[0]:
        p1 = NodeToPoint(triangle[0])
        p2 = NodeToPoint(triangle[1])
        d1 = np.array([p1[0]-p0[0], p1[1]-p0[1], 1.0])
        d2 = np.array([p2[0]-p0[0], p2[1]-p0[1], 1.0])
        dp = p2-p1
        nx,ny,nz = np.cross(d1, d2)
        triangle_func = np.array([nx/nz, ny/nz, triangle[2]])
        func_list.append(triangle_func)
        triangle_area = triangle_areas[triangle[2]]
        b = b + scipy.integrate.dblquad(Load, 0.0, 1.0, 0.0, 1.0, args=(p0, p1, p2, triangle_area))[0]
    basis_func_list.append(func_list)
    load_vector.append(b)

stiffness_matrix = np.zeros((len(basis_func_list), len(basis_func_list)))

# Go through each basis function pair to create siffness matrix
for i in range(len(basis_func_list)):
    for j in range(len(basis_func_list)):
        basis_sum = 0
        # Check every triangle in each pair and see if they are the same
        for triangle_i in basis_func_list[i]:
            for triangle_j in basis_func_list[j]:
                if triangle_i[2] == triangle_j[2]:
                    triangle_index = int(triangle_i[2])
                    # sum of each shared triangle in dot(div(f_i), div(f_j))*A^2
                    basis_sum = basis_sum + (triangle_i[0]*triangle_j[0] + triangle_i[1]*triangle_j[1])
        stiffness_matrix[i][j] = basis_sum
    

### Update values ###

#load_vector = -2.0*np.ones(len(basis_func_list))
z_nodes = np.linalg.solve(stiffness_matrix, load_vector)

# Prep nodes for 3D plotting
for node_index in range(len(node_list)):
    node_list[node_index] = np.append(node_list[node_index],0.0)

max_z = 0.0
for i in range(len(basis_func_list)):
    node_index = basis_list[i][1]
    node_list[node_index][2] = z_nodes[i]
    if z_nodes[i]*z_nodes[i] > max_z*max_z:
        max_z = z_nodes[i]


### Plot ###

# Create canvas
fig = pyplot.figure()
# Create axes
ax = pyplot.axes(projection='3d')
# Initialize colormap
cmap = pyplot.get_cmap()


# Plot triangles
for triangle in triangle_list:
    collection = Poly3DCollection([TriangleToPoints(triangle)], alpha=0.75, edgecolors='none', facecolors=cmap((NodeToPoint(triangle[2])[2])/max_z))
    ax.add_collection3d(collection)

# Format display
fig.suptitle("Mesh Generated Solver")
ax.set_xlim(xmin=x_range[0], xmax=x_range[1])
ax.set_ylim(ymin=y_range[0], ymax=y_range[1])
ax.set_zlim(zmin=z_range[0], zmax=z_range[1])
ax.set(xlabel='X', ylabel='Y', zlabel='Z')
fig.tight_layout()

# Show display
pyplot.show()

########
