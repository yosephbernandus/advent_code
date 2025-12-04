# with open("input.txt", "r") as f:
#     lines = [line.strip() for line in f]
#
# total = 0
#
# for bank in lines:
#     max_joltage = 0
#     n = len(bank)
#
#     # Try all pairs
#     for i in range(n):
#         for j in range(i + 1, n):
#             joltage = int(bank[i] + bank[j])
#             max_joltage = max(max_joltage, joltage)
#
#     total += max_joltage
#
# print(total)


# part 2
with open("input.txt", "r") as f:
    lines = [line.strip() for line in f]


def find_max_k_digits(s, k):
    n = len(s)
    if k == 0:
        return ""
    if k == n:
        return s

    result = []
    start = 0

    for i in range(k):
        # Need (k-i) digits total, so search up to position n-(k-i)+1
        end = n - (k - i) + 1

        # Find max digit in range [start, end)
        max_digit = "0"
        max_pos = start
        for j in range(start, end):
            if s[j] > max_digit:
                max_digit = s[j]
                max_pos = j

        result.append(max_digit)
        start = max_pos + 1

    return "".join(result)


total = 0

for bank in lines:
    max_joltage_str = find_max_k_digits(bank, 12)
    total += int(max_joltage_str)

print(total)
