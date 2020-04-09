#include <vector.h>
#include <iostream>

ECS_STRUCT(Vert2D, {
    float x;
    float y;
});

ECS_STRUCT(Mesh, {
    flecs::vector<Vert2D> vertices;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    flecs::meta<Vert2D>(world);
    flecs::meta<Mesh>(world);

    /* Create an instance of the Mesh type */
    Mesh m = {
        {
            {10, 20},
            {30, 40},
            {50, 60}
        }
    };

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, m) << std::endl;
}
