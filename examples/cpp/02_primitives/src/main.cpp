#include <primitives.h>
#include <iostream>

ECS_STRUCT(Primitives, {
    bool a_bool;
    char a_char;
    flecs::byte a_byte;
    int32_t an_int;
    uint32_t a_uint;
    float a_float;
    const char *a_string;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions for Primitives. This will also register the
     * Primitives type as a component */
    flecs::meta<Primitives>(world);

    /* Create an instance of the Primitives type */
    Primitives p = {
        .a_bool = true, 
        .a_char = 'a', 
        .a_byte = 16,
        .an_int = -10, 
        .a_uint = 10,
        .a_float = 20.5, 
        .a_string = "Hello World"
    };

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, p) << std::endl;
}
