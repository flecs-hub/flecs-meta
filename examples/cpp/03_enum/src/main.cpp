#include <enum.h>
#include <iostream>

ECS_ENUM(Color, {
    Red,
    Green,
    Blue
});

ECS_STRUCT(Vert2D, {
    float x;
    float y;
    Color color;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    flecs::meta<Color>(world);
    flecs::meta<Vert2D>(world);

    /* Create an instance of the Vert2D type */
    Vert2D v = {10, 20, Green };

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, v) << std::endl;
}
