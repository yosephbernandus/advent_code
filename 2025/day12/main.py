#!/usr/bin/env python3

class Region:
    def __init__(self, x, y, to_fit):
        self.x = x
        self.y = y
        self.to_fit = to_fit  # Array of 6 integers

def region_fits(region):
    """Check if all presents fit in the region based on area."""
    # Each present occupies 9 cells (3x3 grid)
    required_area = sum(count * 9 for count in region.to_fit)
    available_area = region.x * region.y
    return available_area >= required_area

def parse_input(filename):
    """Parse the input file and return list of regions."""
    regions = []

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()

            # Skip empty lines
            if len(line) < 4:
                continue

            # Check if this is a region line (contains 'x' and ':')
            if 'x' not in line or ':' not in line:
                continue

            # Parse region: "WxH: count0 count1 count2 count3 count4 count5"
            parts = line.split(':')
            if len(parts) != 2:
                continue

            # Parse dimensions
            dimensions = parts[0].strip()
            if 'x' not in dimensions:
                continue

            dim_parts = dimensions.split('x')
            if len(dim_parts) != 2:
                continue

            try:
                x = int(dim_parts[0])
                y = int(dim_parts[1])
            except ValueError:
                continue

            # Parse counts
            description = parts[1].strip()
            count_strs = description.split()

            if len(count_strs) != 6:
                continue

            try:
                to_fit = [int(cs) for cs in count_strs]
            except ValueError:
                continue

            regions.append(Region(x, y, to_fit))

    return regions

def main():
    regions = parse_input("input.txt")

    print(f"Parsed {len(regions)} regions")

    result = 0
    for i, region in enumerate(regions):
        if region_fits(region):
            print(f"Region {i + 1} ({region.x}x{region.y}): ✓ Fits")
            result += 1
        else:
            print(f"Region {i + 1} ({region.x}x{region.y}): ✗ Does not fit")

    print(f"\nTotal regions that can fit all presents: {result}")

if __name__ == "__main__":
    main()
