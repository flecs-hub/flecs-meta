#include <enum.h>

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
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions. Make sure to define Color before Vert2D as
     * it is used by Vert2D. */
    ECS_META(world, Color);
    ECS_META(world, Vert2D);

    /* Create an instance of the Vert2D type */
    Vert2D v = {10, 20, Green };

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Vert2D), &v);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}