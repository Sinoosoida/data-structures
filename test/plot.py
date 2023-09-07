import csv
import matplotlib.pyplot as plt

x_hash = []
y_hash = []
x_avl = []
y_avl = []

with open('data.csv') as csvfile:
    reader = csv.reader(csvfile)
    next(reader)
    for row in reader:
        if row[0] == 'HASH':
            x_hash.append(int(row[1]))
            y_hash.append(float(row[2]))
        elif row[0] == 'AVL':
            x_avl.append(int(row[1]))
            y_avl.append(float(row[2]))

plt.plot(x_hash, y_hash, label='HASH',markersize=12)
plt.plot(x_avl, y_avl, label='AVL', markersize=12)
plt.xlabel('Количество элементов')
plt.ylabel('Время выполнения')
plt.xscale('log') # логарифмическая ось X
plt.yscale('log') #
plt.legend()

plt.savefig('result.png')