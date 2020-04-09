#include <runtime_type.h>
#include <iostream>

struct Position {
    float x;
    float y;
};

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    auto position_type = flecs::entity(world, "Position")
        .set<flecs::components::meta::Struct>({
            {
                {"x", world.lookup("float").id()},
                {"y", world.lookup("float").id()}
            }
        });

    /* Create a dummy value */
    Position p = {10, 20};

    /* Serialize the value to a string */
    std::cout << flecs::pretty_print(world, position_type, p) << std::endl;
}
