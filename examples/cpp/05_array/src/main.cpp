#include <array.h>
#include <iostream>

ECS_STRUCT(Vert2D, {
    float coord[2];
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    flecs::meta<Vert2D>(world);

    /* Create an instance of the Sandwich type */
    Vert2D v = { { 10, 20 } };

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, v) << std::endl;
}
