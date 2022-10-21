import numpy as np

print(1)

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

# print_samples(8, 8, 8)
# print_samples(64, 64, 64)
# print_samples(128, 128, 128)
# print_samples(256, 256, 256)
# print_samples(512, 512, 512)
print_samples(1024, 1024, 1024)

print()
