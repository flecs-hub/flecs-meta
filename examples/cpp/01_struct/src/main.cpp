#include <struct.h>
#include <iostream>

ECS_STRUCT(Position, {
    int32_t x;
    int32_t y;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions for Position. This will also register the
     * Position type as a component */
    flecs::meta<Position>(world);

    /* Create an instance of the Position type */
    Position p = {10, 20};

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, p) << std::endl;
}
