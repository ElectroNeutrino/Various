import numpy
import multiprocessing



def mp_func_test(ra, wa, wa_shape, tv, lock):
    wa = wa.reshape(wa_shape)
    with lock:
        tv.value = wa[1][1]
        for x in range(wa_shape[0]):
            for y in range(wa_shape[1]):
                wa[x][y] = -ra[x][y]
                tv.value = 5
        wa[1][1] = 100
    return
                


if __name__ == '__main__':

    shape = (4, 4)
    write_lock = multiprocessing.Lock()
    read_array = numpy.random.rand(shape[0], shape[1]) # Create an array
    write_array = numpy.zeros(shape[0] * shape[1])
    write_array[:] = multiprocessing.Array('d', write_array) # Convert to flat multiprocess array
    
    test_value = multiprocessing.Value('d', 10) # create multiprocess scalar
    my_args=(read_array, write_array, shape, test_value, write_lock)
    p = multiprocessing.Process(target=mp_func_test, args=my_args)

    write_array = write_array.reshape(shape) # convert to original shape

    p.start()
    p.join()

    print("read_array")
    print(read_array[:])

    print("write_array")
    print(write_array[:])

    print("test_value")
    print(test_value.value)
