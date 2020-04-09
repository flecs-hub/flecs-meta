#include <serialize_type.h>
#include <iostream>

ECS_STRUCT(Position, {
    float x;
    float y;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    auto position_type = flecs::meta<Position>(world);

    /* Pretty print the type. Because the type is an entity, the metadata is 
     * stored in components that support reflection, we can serialize types. */
    std::cout << flecs::pretty_print(world, position_type) << std::endl;
}
