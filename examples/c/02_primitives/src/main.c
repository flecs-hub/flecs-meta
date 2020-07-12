#include <primitives.h>

ECS_STRUCT(Primitives, {
    bool a_bool;
    char a_char;
    ecs_byte_t a_byte;
    int32_t an_int;
    uint32_t a_uint;
    float a_float;
    char *a_string;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Primitives. This will also register the
     * Primitives type as a component */
    ECS_META(world, Primitives);

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
    char *str = ecs_ptr_to_str(world, ecs_entity(Primitives), &p);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}