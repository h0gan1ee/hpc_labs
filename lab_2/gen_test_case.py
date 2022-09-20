import numpy as np

print(3)

a = 3
b = 4
c = 5

print(a)
print(b)

arr = np.random.default_rng().uniform(low=0.0, high=100.0, size=a*b)

for num in arr:
    print(num)

print(b)
print(c)

arr = np.random.default_rng().uniform(low=0.0, high=100.0, size=b*c)

for num in arr:
    print(num)

a = 100
b = 200
c = 300

print(a)
print(b)

arr = np.random.default_rng().uniform(low=0.0, high=500.0, size=a*b)

for num in arr:
    print(num)

print(b)
print(c)

arr = np.random.default_rng().uniform(low=0.0, high=500.0, size=b*c)

for num in arr:
    print(num)

a = 1000
b = 1000
c = 1000

print(a)
print(b)

arr = np.random.default_rng().uniform(low=0.0, high=2000.0, size=a*b)

for num in arr:
    print(num)

print(b)
print(c)

arr = np.random.default_rng().uniform(low=0.0, high=2000.0, size=b*c)

for num in arr:
    print(num)
