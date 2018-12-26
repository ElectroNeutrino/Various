import numpy
import multiprocessing
from multiprocessing import SimpleQueue


def mp_func_test(ra, wa_shape, tv, lock, queue, queue2):
    wa = numpy.zeros(wa_shape[0] * wa_shape[1])
    wa = wa.reshape(wa_shape)
    with lock:
        tv.value = wa[1][1]
        for x in range(wa_shape[0]):
            for y in range(wa_shape[1]):
                wa[x][y] = -ra[x][y]
                tv.value = 5
        wa[1][1] = 100
        queue.put(wa)
        queue2.put(tv.value)

    return
                


if __name__ == '__main__':

    shape = (10, 4)
    write_lock = multiprocessing.Lock()
    mp_queue = SimpleQueue()
    mp2_queue = SimpleQueue()
    read_array = numpy.random.rand(shape[0], shape[1]) # Create an array
    write_array = numpy.zeros(shape)
    
    test_value = multiprocessing.Value('d', 10) # create multiprocess scalar
    my_args=(read_array, shape, test_value, write_lock, mp_queue, mp2_queue)
    p = multiprocessing.Process(target=mp_func_test, args=my_args)

    p.start()
    p.join()

    if not mp_queue.empty():
        write_array = mp_queue.get()

    if not mp2_queue.empty():
        queue_value = mp2_queue.get()

    print("read_array")
    print(read_array[:])

    print("write_array")
    print(write_array[:])

    print("test_value")
    print(test_value.value)

    print("queue_value")
    print(queue_value)
