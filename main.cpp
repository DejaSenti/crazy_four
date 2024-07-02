#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

using namespace std;

enum Sides
{
    RED = 0,
    GREEN,
    BLUE,
    YELLOW,
    NUM_SIDES
};

const std::string COLOUR_VALUES[NUM_SIDES] = 
{
    "Red",
    "Green",
    "Blue",
    "Yellow"
};

inline Sides& operator++(Sides& side, int) 
{
    const int i = static_cast<int>(side) + 1;
    side = static_cast<Sides>((i) % static_cast<int>(NUM_SIDES));
    return side;
}

static void rotate_cube(std::array<Sides, NUM_SIDES>& cube)
{
    for (int i = cube.size() - 1; i > 0; i--)
    {
        std::swap(cube[i], cube[i - 1]);
    }
}

static void heap_algorithm(int num, std::array<Sides, NUM_SIDES> array, std::vector<std::array<Sides, NUM_SIDES>>& result)
{
    if (num == 1)
    {
        result.push_back(array);
        return;
    }

    for (int i = 0; i < num - 1; i++)
    {
        if (num % 2 == 0)
        {
            std::swap(array[i], array[num - 1]);
        }
        else
        {
            std::swap(array[0], array[num - 1]);
        }

        heap_algorithm(num - 1, array, result);
    }
}

static std::vector<array<Sides, NUM_SIDES>> generate_all_permutations(void)
{
    std::vector<array<Sides, NUM_SIDES>> result;

    std::array<Sides, NUM_SIDES> initial_permutation;
    std::generate(initial_permutation.begin(), initial_permutation.end(), [n = static_cast<Sides>(0)] () mutable { return n++; });

    heap_algorithm(initial_permutation.size(), initial_permutation, result);

    return result;
}

static bool is_rotation(std::array<Sides, NUM_SIDES> cand1, std::array<Sides, NUM_SIDES>& cand2)
{
    if (cand1.size() != cand2.size())
    {
        return false;
    }

    for (int i = 0; i < cand1.size(); i++, rotate_cube(cand1))
    {
        if (cand1 == cand2)
        {
            return true;
        }
    }

    return false;
}

static bool is_existing_permutation(std::array<Sides, NUM_SIDES>& candidate, std::vector<std::array<Sides, NUM_SIDES>>& permutations)
{
    for (auto& perm : permutations)
    {
        if (is_rotation(perm, candidate))
        {
            return true;
        }
    }

    return false;
}

static std::vector<std::array<Sides, NUM_SIDES>> get_cube_permutations(void)
{
    std::vector<std::array<Sides, NUM_SIDES>> result;

    std::vector<std::array<Sides, NUM_SIDES>> all_perms = generate_all_permutations();

    for (auto& perm : all_perms)
    {
        if (is_existing_permutation(perm, result))
        {
            continue;
        }

        result.push_back(perm);
    }

    return result;
}

static std::array<Sides, NUM_SIDES> get_next_permutation(void)
{
    static std::vector<std::array<Sides, NUM_SIDES>> all_permutations = get_cube_permutations();
    static auto iter = all_permutations.begin();

    if (iter != all_permutations.end())
    {
        auto result = *iter;
        iter++;

        return result;
    }
    else
    {
        return { NUM_SIDES };
    }
}

static bool is_colour_placement_correct(std::vector<std::array<Sides, NUM_SIDES>>& cubes)
{
    for (int i = 0; i < NUM_SIDES; i++)
    {
        std::array<int, NUM_SIDES> colour_counters = { 0 };

        for (auto& cube : cubes)
        {
            colour_counters[static_cast<int>(cube[i])]++;
        }

        for (auto& counter : colour_counters)
        {
            if (counter > 1)
            {
                return false;
            }
        }
    }

    return true;
}

enum Status
{
    SUCCESS,
    FAILURE
};

static Status place_next_cube(std::vector<std::array<Sides, NUM_SIDES>>& cubes)
{
    if (!is_colour_placement_correct(cubes))
    {
        return FAILURE;
    }

    if (cubes.size() == NUM_SIDES)
    {
        return SUCCESS;
    }

    auto current_cube = get_next_permutation();

    for (int i = 0; i < NUM_SIDES; i++)
    {
        cubes.push_back(current_cube);

        if (place_next_cube(cubes) == SUCCESS)
        {
            return SUCCESS;
        }
        else
        {
            cubes.pop_back();
        }

        rotate_cube(current_cube);
    }

    return FAILURE;
}

void print_cube_row(std::vector<std::array<Sides, NUM_SIDES>>& cubes)
{

    for (auto& cube : cubes)
    {
        for (int i = 0; i < cube.size(); i++)
        {
            std::cout << std::right << std::setw(10) << std::setfill(' ');
            std::cout << COLOUR_VALUES[static_cast<int>(cube[i])];

            if (i < cube.size() - 1) [[likely]]
            {
                std::cout << " | ";
            }
        }

        std::cout << std::endl;
    }
}

int main(void)
{
    std::vector<std::array<Sides, NUM_SIDES>> cubes;

    if (place_next_cube(cubes) != SUCCESS)
    {
        std::cout << "Failed!" << std::endl;
    }

    print_cube_row(cubes);

    return 0;
}