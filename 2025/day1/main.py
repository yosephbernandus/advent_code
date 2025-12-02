with open("input.txt", "r") as f:
    lines = [line.strip() for line in f]

position = 50
password = 0

for line in lines:
    direction = line[0]
    distance = int(line[1:])

    if direction == "L":
        position = (position - distance) % 100
    else:
        position = (position + distance) % 100

    if position == 0:
        password += 1

print(password)
