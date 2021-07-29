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
                {const_cast<char*>("x"), world.lookup("float").id()},
                {const_cast<char*>("y"), world.lookup("float").id()}
            },
            false
        });

    /* Create a dummy value */
    Position p = {10, 20};

    /* Serialize the value to a string */
    std::cout << flecs::pretty_print(world, position_type, p) << std::endl;
}
