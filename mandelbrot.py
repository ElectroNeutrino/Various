import math
import Image


def complex_mag(x):
    a = x.imag**2.0
    b = x.real**2.0
    c = precision_add(a, b) 
    value = math.sqrt(c)
    return value.real

def is_in_mandelbrot(x, n):
    z = 0
    is_mandel = True
    index_val = 0
    for i in range(n):
        if cartiod_test(x):
            index_val = n
            break
        z = precision_add(z**2.0, x)
        mag = complex_mag(z)
        index_val = i
        if not (mag <= 10^300):
            is_mandel = False
            break
    return (is_mandel, index_val)

def cartiod_test(x):
    is_mandel = False
    c = complex_mag(x)**2
    s = precision_add(8.0 * c, -3.0)
    t = precision_add(c * s, x.real)
    if t <= (3.0 / 32.0):
        is_mandel = True
    q = precision_add(x.real, 1.0)
    p = precision_add(q**2.0, x.imag**2.0)
    if p <= 0.0625:
        is_mandel = True
    return is_mandel

def long_to_color(x, min_val = 0.0, max_val = 1.0, color_range = 255): # 0-1, roygbv
    val_range = max_val - min_val
    val = (x - min_val) / val_range
    r, g, b = 0, 0, 0
    if val <= 0.25:
        r, g, b = 1.0, val * 4.0, 0.0
    elif val <= 0.5:
        r, g, b = 2.0 - 4.0 * val, 1.0, 0.0
    elif val < 0.75:
        r, g, b = 0.0, 3.0 - 4.0 * val, 4.0 * val - 2.0
    else:
        r, g, b = 4.0 * val - 3.0, 0.0, 1.0
    r, g, b = (int(r * color_range), int(g * color_range), int(b * color_range))
    return (r, g, b)

def precision_add(x, y):
    a, b = [0] * 2
    if not (y == 0):
        a = x / y
        b = y * (a + 1.0)
    else:
        b = x
    return b

size_x, size_y = 4000, 3000
scale = 2.5
max_test = 500

img = Image.new('RGB', (size_x, size_y), "white") #new black image: type, witdh, height, starting color
pixels = img.load() #pixel map

count = 0

max_count = size_x * size_y / 2
last_progress = 0
print "0% complete."

for l in range(img.size[0]):
    for m in range((size_y / 2) + 1):
        test_val = complex(scale * (float(l) / float(size_y) - 0.875), scale * (float(m) / float(size_y) - 0.5))
        (in_set, a) = is_in_mandelbrot(test_val, max_test)
        count = count + 1
        progress = (100 * count) // max_count
        color_val = 0
        if not (progress == last_progress):  #((progress % 10) == 0) and 
            print progress, "% complete."
            last_progress = progress
        if not in_set:
            color_val = long_to_color(float(max_test - a) / 1.5, 0.0, float(max_test), 255)
#        color_val = long_to_color(complex_mag(test_val), 0.0, 1.0, 255)
        pixels[l, m] = color_val #set pixel color: (r, g, b)
        pixels[l, size_y - m - 1] = color_val

img.save("image.bmp")
