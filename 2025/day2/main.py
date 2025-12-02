# with open("input.txt", "r") as f:
#     input_data = f.read().strip()
#
# # Parse ranges
# ranges = []
# for part in input_data.split(","):
#     start, end = part.split("-")
#     ranges.append((int(start), int(end)))
#
# total = 0
#
# # Check each range
# for start, end in ranges:
#     # Determine min and max digit lengths to check
#     min_len = len(str(start))
#     max_len = len(str(end))
#
#     # Only check even lengths (invalid IDs must have even length)
#     for length in range(min_len, max_len + 1):
#         if length % 2 != 0:
#             continue
#
#         half_len = length // 2
#         pattern_start = 10 ** (half_len - 1)  # e.g., 10 for 2-digit, 100 for 3-digit
#         pattern_end = 10**half_len  # e.g., 100 for 2-digit, 1000 for 3-digit
#
#         for pattern in range(pattern_start, pattern_end):
#             invalid_id = int(str(pattern) + str(pattern))
#             if start <= invalid_id <= end:
#                 total += invalid_id
#
# print(total)


# Part 2

with open("input.txt", "r") as f:
    input_data = f.read().strip()

ranges = []
for part in input_data.split(","):
    start, end = part.split("-")
    ranges.append((int(start), int(end)))

total = 0
seen = set()  # Avoid counting duplicates

for start, end in ranges:
    min_len = len(str(start))
    max_len = len(str(end))

    for length in range(min_len, max_len + 1):
        # Find all divisors of length (pattern lengths)
        for pattern_len in range(1, length):
            if length % pattern_len != 0:
                continue

            repetitions = length // pattern_len
            if repetitions < 2:  # Must repeat at least twice
                continue

            # Generate patterns
            pattern_start = 10 ** (pattern_len - 1)
            pattern_end = 10**pattern_len

            for pattern in range(pattern_start, pattern_end):
                invalid_id_str = str(pattern) * repetitions
                invalid_id = int(invalid_id_str)

                if start <= invalid_id <= end and invalid_id not in seen:
                    seen.add(invalid_id)
                    total += invalid_id

print(total)
