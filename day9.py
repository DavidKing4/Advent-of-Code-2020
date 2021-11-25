import numpy as np

f = open("input9.txt")
data = [int(x) for x in f.read().split("\n")[:-1]]


def part1(data):
    mask = np.ones(25) - np.identity(25)
    for i, y in enumerate(data[25:]):
        xs = np.array(data[i : i + 25])
        g = np.meshgrid(xs, xs)
        possible = g[0] * mask + g[1] * mask
        if y not in possible:
            return y
            break


ans1 = part1(data)


def part2(data, y):
    p0, p1 = 0, 1
    while True:
        if sum(data[p0:p1]) == y:
            return min(data[p0:p1]) + max(data[p0:p1])
        elif sum(data[p0:p1]) < y:
            p1 += 1
        elif sum(data[p0:p1]) > y:
            p0 += 1


ans2 = part2(data, ans1)
print(ans1, ans2)
