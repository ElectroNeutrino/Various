from math import *

Radius = input("Radius: ")

alpha = 2.0 * pi / 5.0

f = 2.0 - 2.0 * cos(alpha)

A = 1.0 / sqrt(f)
B = sqrt(A * A - 1.0 / 4.0)
C = A - B

h2 = 0.75 - C * C

L = 1.0 / sqrt(A * A + h2 / 4.0)

######  Icosahedron  ########
##
##
##       /\
##      /  \
##   L /    \ L
##    /      \
##   /________\
##       L
##


print "L =", Radius * L, '\n'

beta = acos((2.0 - L * L) / 2.0)
delta = 0.5 * (pi - beta)

######  2V - Icosa  ########
##
##
##            /\
##           /  \
##        p /    \ p
##         /      \
##        /___q____\
##       /\        /\
##      /  \      /  \
##   p /  q \    / q  \ p
##    /      \  /      \
##   /________\/________\
##       p          p
##

p = Radius * sqrt(2.0 * (1 - cos(beta / 2.0)))
S = sqrt(1.0 - L * L / 4.0)
q = Radius * L / (2.0 * S)

print "2v:"
print "p =", p
print "q =", q

######  3V - Icosa  ########
##
##
##                 /\
##                /  \
##             p /    \ p
##              /      \
##             /___q____\
##            /\        /\
##           /  \      /  \
##        p /    \ r  /    \ p
##         /      \  / r    \
##        /____r___\/________\
##       /\        /\   r    /\
##      /  \    r /  \      /  \
##   p /  q \    /  r \    / q  \ p
##    /      \  /      \  /      \
##   /________\/________\/________\
##       p         p         p
##
##

tau = pi - 2.0 * beta / 3.0 - delta
T = L / sqrt(3.0)
p = Radius * sqrt(2.0 * (1 - cos(beta / 3.0)))
l = sin(2.0 * beta / 3.0) / sin(tau)
n = L - l
O = sin(delta) / sin(2.0 * beta / 3.0)
phi = acos((2.0 * O * O - n * n) / (2.0 * O * O))
q = Radius * sqrt(2.0 * (1.0 - cos(phi)))
s = sqrt(l * l + T * T - 2.0 * l * T * cos(pi / 6))
gamma = asin(s / O)
r = Radius * sqrt(2.0 * (1.0 - cos(gamma)))

print "3v:"
print "p =", p
print "q =", q
print "r =", r

######  4V - Icosa  ########
##
##
##                      /\
##                     /  \
##                  p /    \ p
##                   /      \
##                  /___q____\
##                 /\        /\
##                /  \ r    /  \
##             p /    \    / r  \ p
##              /      \  /      \
##             /___s____\/________\
##            /\        /\    s   /\
##           /  \      /  \      /  \
##        p /    \ s  / t  \    /    \ p
##         /      \  /    t \  / s    \
##        /___r____\/___t____\/________\
##       /\        /\        /\   r    /\
##      /  \      /  \      /  \      /  \
##   p /  q \  r /  s \  s /  r \    / q  \ p
##    /      \  /      \  /      \  /      \
##   /________\/________\/________\/________\
##       p         p         p         p
##
##

raw_input("Press enter to quit.")

