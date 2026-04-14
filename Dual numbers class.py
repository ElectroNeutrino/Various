class dual_number:
    def __init__(self, a: float, b: float):
        self.real_part = a
        self.dual_part = b

    def __add__(self, rhs): #(a, b) + (c,d) = (a+c, b+d)
        a = self.real_part + rhs.real_part
        b = self.dual_part + rhs.dual_part
        return dual_number(a, b)

    def __sub__(self, rhs): #(a, b) - (c,d) = (a-c, b-d)
        a = self.real_part - rhs.real_part
        b = self.dual_part - rhs.dual_part
        return dual_number(a, b)

    def __mul__(self, rhs): #(a, b) * (c,d) = (ac, ad+bc)
        if isinstance(rhs, dual_number):
            a = self.real_part * rhs.real_part
            b = (self.real_part * rhs.dual_part) + (self.dual_part * rhs.real_part)
        else: # (a, b) * c = (a*c, b*c)
            a = self.real_part * rhs
            b = self.dual_part * rhs
        return dual_number(a, b)

    def __truediv__(self, rhs): #(a, b) / (c,d) = (a/c, (bc-ad)/c^2)
        if isinstance(rhs, dual_number):
            a = self.real_part / rhs.real_part
            b = ((self.dual_part * rhs.real_part) - (self.real_part * rhs.dual_part)) / (rhs.real_part * rhs.real_part)
        else: # (a, b) / c = (a/c, b/c)
            a = self.real_part / rhs
            b = self.dual_part / rhs
        return dual_number(a, b)

    def __rmul__(self, lhs: float): # a * (b, c) = (a*b, a*c)
        a = lhs * self.real_part
        b = lhs * self.dual_part
        return dual_number(a, b)

    def __rtruediv__(self, lhs): #a / (b,c) = (a/b, -ac/b^2)
        a = lhs / self.real_part
        b = (-lhs * self.dual_part) / (self.real_part * self.real_part)
        return dual_number(a, b)
    def __eq__(self, rhs) -> bool: # (a, b) == (c, d) iff a == c and b == d
        return ((self.real_part == rhs.real_part) and (self.dual_part == rhs.dual_part))

    def __repr__(self):
        return f'({self.real_part} + {self.dual_part}e)'

one = dual_number(1, 2)
two = dual_number(3, 4)
print(f'{one} + {two} = {one + two}')
print(f'{one} - {two} = {one - two}')
print(f'{one} * {two} = {one * two}')
print(f'{one} / {two} = {one / two}')
print(f'2 * {one} = {2*one}')
print(f'{two} * 3 = {two*3}')
print(f'2 / {one} = {2/one}')
print(f'{two} / 3 = {two/3}')
print(f'{one} == {two}: {one == two}')
print(f'{one} + {dual_number(2, 2)} == {two}: {(one + dual_number(2, 2)) == two}')
