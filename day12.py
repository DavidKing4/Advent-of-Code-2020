import numpy as np

f = open("input12.txt")
instructions = [x for x in f.read().split("\n")[:-1]]


def nav(instructions, part):
    loc = [0, 0] if part == 1 else [10, 1]
    boat_loc = [0, 0]  # if part 1 loc is boat_loc
    direction = 1  # N:0, E:1, S:2, W:3
    dir_to_val = [[0, 1], [1, 0], [0, -1], [-1, 0]]
    for ins in instructions:
        action = ins[0]
        value = int(ins[1:])
        match action:
            case "N":
                loc[1] += value
            case "S":
                loc[1] -= value
            case "E":
                loc[0] += value
            case "W":
                loc[0] -= value
            case "F":
                match part:
                    case 1:
                        vec_val = [value * i for i in dir_to_val[direction]]
                        loc = [i + j for i, j in zip(loc, vec_val)]
                    case 2:
                        boat_loc = [i + (value * j) for i, j in zip(boat_loc, loc)]
            case "L":
                match part:
                    case 1:
                        direction -= int(value / 90)
                        direction %= 4
                    case 2:
                        acw = np.array([[0, 1], [-1, 0]])
                        rot_array = np.linalg.matrix_power(acw, int((value / 90) % 4))
                        loc = [i for i in np.matmul(np.array(loc), rot_array)]
            case "R":
                if part == 1:
                    direction += int(value / 90)
                    direction %= 4
                elif part == 2:
                    cw = np.array([[0, -1], [1, 0]])
                    rot_array = np.linalg.matrix_power(cw, int((value / 90) % 4))
                    loc = [i for i in np.matmul(np.array(loc), rot_array)]
            case _:
                raise Exception("Unknown action")
    return loc if part == 1 else boat_loc


def manhattan(loc):
    return abs(loc[0]) + abs(loc[1])


print(manhattan(nav(instructions, 1)))
print(manhattan(nav(instructions, 2)))
