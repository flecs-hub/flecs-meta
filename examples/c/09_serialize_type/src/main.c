#include <serialize_type.h>

ECS_STRUCT(Position, {
    float x;
    float y;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Position. */
    ECS_META(world, Position);

    /* Pretty print the type. Because the type is an entity, the metadata is 
     * stored in components that support reflection, we can serialize types. */
    char *str = ecs_entity_to_str(world, ecs_entity(Position));
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
