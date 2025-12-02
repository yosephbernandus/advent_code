with open("data.txt") as r:
    result_formatted_list = []
    safe = 0
    lines = r.readlines()
    for line in lines:
        line = line.strip().split()
        is_safe = True
        for i in range(1, len(line)):
            increasing = True if int(line[1]) > int(line[0]) else False
            diff_allowed = [1, 2, 3] if increasing else [-1, -2, -3]

            for i in range(1, len(line)):
                if int(line[i]) - int(line[i - 1]) not in diff_allowed:
                    is_safe = False
                    break

        if is_safe:
            safe += 1

    print(safe)
