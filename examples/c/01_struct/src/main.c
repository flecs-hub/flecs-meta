#include <struct.h>

ECS_STRUCT(Position, {
    int32_t x;
    int32_t y;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Position. This will also register the
     * Position type as a component */
    ECS_META(world, Position);

    /* Create an instance of the Position type */
    Position p = {10, 20};

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Position), &p);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
