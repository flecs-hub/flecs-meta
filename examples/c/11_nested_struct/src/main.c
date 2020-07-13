#include <nested_struct.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Position. This will also register the
     * Position type as a component */
    ECS_META(world, Point);
    ECS_META(world, Line);

    /* Create an instance of the Position type */
    Line l = {{10, 20}, {30, 40}};

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Line), &l);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
