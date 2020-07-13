#include <array.h>

ECS_STRUCT(Vert2D, {
    float coord[2];
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Vert2D. This will also register the
     * Vert2D type as a component */
    ECS_META(world, Vert2D);

    /* Create an instance of the Vert2D type */
    Vert2D v = { { 10, 20 } };

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Vert2D), &v);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
