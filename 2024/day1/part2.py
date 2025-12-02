with open("data.txt") as r:
    first_data = []
    second_data = []
    for a in r:
        replace_new_list = a.replace("\n", "")
        new_list = replace_new_list.split("  ")
        if len(new_list) == 2:
            first_data.append(int(new_list[0].replace(" ", "")))
            second_data.append(int(new_list[1].replace(" ", "")))

    result = 0
    for i in range(len(first_data)):
        counter_same_value = 0
        for j in range(len(second_data)):
            if first_data[i] == second_data[j]:
                counter_same_value += 1

        new_value = first_data[i] * counter_same_value
        result += new_value

    print(result)
