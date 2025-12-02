with open("data.txt") as r:
    first_data = []
    second_data = []
    for a in r:
        replace_new_list = a.replace("\n", "")
        new_list = replace_new_list.split("  ")
        if len(new_list) == 2:
            first_data.append(int(new_list[0].replace(" ", "")))
            second_data.append(int(new_list[1].replace(" ", "")))

    first_data.sort()
    second_data.sort()

    result = 0
    for i in range(len(first_data)):
        result += abs(first_data[i] - second_data[i])

    print(result)
