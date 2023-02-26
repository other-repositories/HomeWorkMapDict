
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
# Чтение данных из файлов
x1, y1 = [], []
with open('dict_memory.txt', 'r',encoding = 'utf-8') as f:
    for line in f:
        x1.append(float(line.split()[0]))
        y1.append(float(line.split()[1]))

x2, y2 = [], []
with open('map_memory.txt', 'r',encoding = 'utf-8') as f:
    for line in f:
        x2.append(float(line.split()[0]))
        y2.append(float(line.split()[1]))

# Построение графика

fig1 = plt.figure()
ax1 = fig1.add_subplot(111)
ax1.plot(x1, y1, label='python memory')
ax1.plot(x2, y2, label='c++ memory')


# Чтение данных из файлов
x3, y3 = [], []
with open('dict_time.txt', 'r',encoding = 'utf-8') as f:
    for line in f:
        x3.append(float(line.split()[0]))
        y3.append(float(line.split()[1]))

x4, y4 = [], []
with open('map_time.txt', 'r',encoding = 'utf-8') as f:
    for line in f:
        x4.append(float(line.split()[0]))
        y4.append(float(line.split()[1]))

# Построение графика
fig2 = plt.figure()
ax2 = fig2.add_subplot(111)
ax2.plot(x3, y3, label='python time')
ax2.plot(x4, y4, label='c++ time')
plt.legend()
plt.show()


#import time
#import sys
#
#def time_test(n):
#    d = {}
#    start = time.time()
#    for i in range(n):
#        d[i] = i
#    end = time.time()
#    return end-start
#
#def memory_test(n):
#    d = {}
#    for i in range(n):
#        d[i] = i
#    return sys.getsizeof(d)
#
#if __name__ == '__main__':
#    with open('dict_time.txt', 'w') as f:
#        for i in range(1, 10000001, 1000000):
#            f.write('{} {}\n'.format(i, time_test(i)))
#
#    with open('dict_memory.txt', 'w') as f:
#        for i in range(1, 10000001, 1000000):
#            f.write('{} {}\n'.format(i, memory_test(i)))