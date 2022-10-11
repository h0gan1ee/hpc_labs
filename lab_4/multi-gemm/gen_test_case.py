import numpy as np

print(3)

a = 10
b = 4
c = 8

print(a)
print(b)

arr = np.random.default_rng().uniform(low=0.0, high=100.0, size=a*b)

for num in arr:
    print(np.around(num, 6))

print(b)
print(c)

arr = np.random.default_rng().uniform(low=0.0, high=100.0, size=b*c)

for num in arr:
    print(np.around(num, 6))

a = 100
b = 200
c = 400

print(a)
print(b)

arr = np.random.default_rng().uniform(low=0.0, high=500.0, size=a*b)

for num in arr:
    print(np.around(num, 6))

print(b)
print(c)

arr = np.random.default_rng().uniform(low=0.0, high=500.0, size=b*c)

for num in arr:
    print(np.around(num, 6))

a = 600
b = 60
c = 600

print(a)
print(b)

arr = np.random.default_rng().uniform(low=0.0, high=2000.0, size=a*b)

for num in arr:
    print(np.around(num, 6))

print(b)
print(c)

arr = np.random.default_rng().uniform(low=0.0, high=2000.0, size=b*c+100)

for num in arr:
    print(np.around(num, 6))

print()
