#include <bitmask.h>
#include <iostream>

ECS_BITMASK(Toppings, {
    Bacon = 0x1,
    Lettuce = 0x2,
    Tomato = 0x4,
    Onion = 0x8,
    Egg = 0x10,
    Turkey = 0x20
});

ECS_STRUCT(Sandwich, {
    const char *name;
    flecs::bitmask<Toppings> toppings;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    flecs::meta<Toppings>(world);
    flecs::meta<Sandwich>(world);

    /* Create an instance of the Sandwich type */
    Sandwich s = { "BLT", Bacon | Lettuce | Tomato };

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, s) << std::endl;
}
