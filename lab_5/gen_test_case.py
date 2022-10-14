import numpy as np

print(5)

def print_samples(a, b, c):
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

print_samples(10, 20, 30)
print_samples(50, 50, 50)
print_samples(1000, 3, 50)
print_samples(200, 400, 300)
print_samples(800, 100, 800)

print()
