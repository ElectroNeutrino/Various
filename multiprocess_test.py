import numpy
import multiprocessing



def mp_func_test(ra, ra_shape, tv, lock):
    ra = ra.reshape(ra_shape)
    with lock:
        tv.value = ra[1][1]


if __name__ == '__main__':

    write_lock = multiprocessing.Lock()
    ra = numpy.random.rand(10, 4) # Create 4x4 array
    ra_shape = ra.shape # store array shape
    ra = multiprocessing.Array('d', ra.reshape(ra_shape[0] * ra_shape[1])) # Convert to flat multiprocess array
    read_array = (numpy.frombuffer(ra.get_obj())) # make numpy type array handle
    
    test_value = multiprocessing.Value('d', 0) # create multiprocess scalar
    p = multiprocessing.Process(target=mp_func_test,
                                args=(read_array, ra_shape, test_value, write_lock))

    read_array = read_array.reshape(ra_shape) # convert to original shape

    p.start()
    p.join()

    print("read_array")
    print(read_array[:])

    print("test_value")
    print(test_value.value)
