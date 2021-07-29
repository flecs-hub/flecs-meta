#include <struct.h>
#include <iostream>

ECS_STRUCT(Position, {
    int32_t x;
    int32_t y;
})

int main(int, char *[]) {
    flecs::world ecs;

    // Import meta module
    ecs.import<flecs::components::meta>();

    // Insert the meta definitions for Position. This will also register the
    // Position type as a component
    flecs::meta<Position>(ecs);

    // Create an instance of the Position type
    Position p = {10, 20};

    // Pretty print the value
    std::cout << flecs::pretty_print(ecs, p) << std::endl;
}
