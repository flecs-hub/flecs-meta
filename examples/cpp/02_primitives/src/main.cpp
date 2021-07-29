#include <primitives.h>
#include <iostream>

ECS_STRUCT(Primitives, {
    bool a_bool;
    char a_char;
    flecs::byte_t a_byte;
    int32_t an_int;
    uint32_t a_uint;
    float a_float;
    const char *a_string;
})

int main(int, char *[]) {
    flecs::world ecs;

    /* Import meta module */
    ecs.import<flecs::components::meta>();

    /* Insert the meta definitions for Primitives. This will also register the
     * Primitives type as a component */
    flecs::meta<Primitives>(ecs);

    /* Create an instance of the Primitives type */
    Primitives p;
    p.a_bool = true;
    p.a_char = 'a';
    p.a_byte = 16;
    p.an_int = -10;
    p.a_uint = 10;
    p.a_float = 20.5;
    p.a_string = "Hello World";

    /* Pretty print the value */
    std::cout << flecs::pretty_print(ecs, p) << std::endl;
}
