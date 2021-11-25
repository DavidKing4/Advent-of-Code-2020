f = open("input10.txt")
adapters = [int(x) for x in f.read().split("\n")[:-1]]
adapters.sort()
adapters_ = [x for x in adapters]
adapters_.insert(0, 0)
diff = [x - y for x, y in zip(adapters, adapters_)]
ones = sum([1 if x == 1 else 0 for x in diff])
threes = sum([1 if x == 3 else 0 for x in diff])
print(ones * (threes + 1))

gaps = []
t = 0
for x in diff:
    if x == 1:
        t += 1
    else:
        gaps.append(t)
        t = 0
gaps.append(t)


def pos(x: int):
    if x == 0 or x == 1:
        return 1
    if x == 2:
        return 2
    if x == 3:
        return 4
    if x == 4:
        return 7


gap_pos = [pos(i) for i in gaps]
total = 1
for x in gap_pos:
    total *= x

print(total)
