The Puzzle:
==========
There are 4 cubes, where each cube has 2 opposite blank sides, and 4 adjacent sides, each with one of four colours on it - Red, Green, Blue, Yellow.
Each cube has the coloured sides in a different order, so no two cubes are identical.
The goal is to arrange the cubes in a row so that each side of the resulting 4x1 rectangular cuboid shows all 4 colours.

The Plan:
=========
The first cube can be placed whichever way.
Each cube that is placed should be checked against all cubes placed before it to make sure that no colour repeats in the row. 
If the colour check passes, this placement is successful and the next cube may be placed.
If the placed cube is the last cube, this counts as a successful placement immediately.
If the next cube placement is not successful (meaning no further permutation yielded the correct result), the current cube should be rotated and placed again.
If the next cube placement is successful, the current cube placement is also successful.

The Pseudocode:
==============
bool place_next_cube(cubes_list, total_num_cubes, current_cube_index)
    for cube orientation
        place cube
        if colour check is good
            if cube is last
                return success
            else
                place next cube
                if placement successful
                    return success
                else
                    remove next cube
        
    return failure

Generating Cubes:
================
There are N possible values on each side of the cube, therefore there are N! possible options of value orders.
Each option which is just a rotation of another option doesn't count, and there are N - 1 rotations for each permutation, so a total of N * (N - 1) options out of N! that don't count, so a total of N! - N * (N - 1) options.

for permutation
    if permutation is rotation of any previous permutation
        continue
    else
        add to existing permutations

Generating Permutations:
=======================
Heap's algorithm:

generate(num, array, result)
    if num == 1
        result.append(array)
        return

    generate(k - 1, array, result)

    for i = 0; i < k - 1; i++
        if k is even
            swap array[i], array[k - 1]
        else
            swap array[0], array[k - 1]

        generate(k - 1, array, result)

generate_all_permutations()
    for i = 0; i < max; i++
        array.append(i)

    generate(max, array, result)

Rotation:
========
rotate(array)
    for i = length(array) - 1; i > 0; i--
        swap array[i], array[i - 1]