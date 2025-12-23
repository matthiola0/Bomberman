
import random

def generate_map(width, height, stone_ratio=0.3):
    indices = []
    # 0 to width*height - 1
    for r in range(height):
        for c in range(width):
            idx = r * width + c
            
            # OUTER BOUNDARY: Skip the first/last row and column
            if r == 0 or r == height - 1 or c == 0 or c == width - 1:
                # This is a WALL (implicitly, as it won't be in LEVEL.txt)
                continue

            # Corner areas for player starts (keep clear of walls/stones)
            # These are relative to the inner area (1,1 to width-2, height-2)
            is_start_area = (
                (r <= 2 and c <= 2) or
                (r <= 2 and c >= width - 3) or
                (r >= height - 3 and c <= 2) or
                (r >= height - 3 and c >= width - 3)
            )
            
            if is_start_area:
                indices.append(str(idx))
                continue
                
            # Classic Indestructible pattern
            if r % 2 == 1 and c % 2 == 1:
                # This is a WALL. Don't add it to the file.
                continue
            
            # Decide if it's a road or a stone
            if random.random() < stone_ratio:
                indices.append(str(idx + 1000)) # Stone
            else:
                indices.append(str(idx)) # Road
                
    return "\n".join(indices)

# LEVEL 1 & 2: 15x15
with open("LEVEL1.txt", "w") as f:
    f.write(generate_map(15, 15, 0.4))

with open("LEVEL2.txt", "w") as f:
    f.write(generate_map(15, 15, 0.5))

# LEVEL 3: 25x25
with open("LEVEL3.txt", "w") as f:
    f.write(generate_map(25, 25, 0.6))
