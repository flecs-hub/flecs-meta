#include <map.h>
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
    int32_t cost;
    flecs::bitmask<Toppings> toppings;
});

ECS_STRUCT(Menu, {
    flecs::map<flecs::string, Sandwich> items;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    flecs::meta<Toppings>(world);
    flecs::meta<Sandwich>(world);
    flecs::meta<Menu>(world);

    /* Create an instance of the Menu type */
    Menu m = { {
        {"BLT", {2, Bacon | Lettuce | Tomato}},
        {"Turkey sub", {3, Turkey | Bacon | Onion}}
    } };

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, m) << std::endl;
}
