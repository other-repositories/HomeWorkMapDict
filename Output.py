import time
import sys

def time_test(n):
    d = {}
    start = time.time()
    for i in range(n):
        d[i] = i
    end = time.time()
    return end-start

def memory_test(n):
    d = {}
    for i in range(n):
        d[i] = i
    return sys.getsizeof(d)

if __name__ == '__main__':
    with open('dict_time.txt', 'w') as f:
        for i in range(1, 8):
            f.write('{} {}\n'.format(10**i, time_test(10**i)))

    with open('dict_memory.txt', 'w') as f:
        for i in range(1, 8):
            f.write('{} {}\n'.format(10**i, memory_test(10**i)))
