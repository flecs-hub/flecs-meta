#include <runtime_type.h>

typedef struct Position {
    float x;
    float y;
} Position;

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    /* Create new entity for our type. This type will describe Position. */
    ecs_entity_t position_type = ecs_set(world, 0, EcsName, {"Position"});

    /* Add an EcsStruct component to the type that holds our members */
    EcsStruct *type = ecs_get_mut(world, position_type, EcsStruct, NULL);

    /* Add the members to the members vector */
    EcsMember *m_x = ecs_vector_add(&type->members, EcsMember);
    m_x->name = "x";
    m_x->type = ecs_lookup_fullpath(world, "float");

    EcsMember *m_y = ecs_vector_add(&type->members, EcsMember);
    m_y->name = "y";
    m_y->type = ecs_lookup_fullpath(world, "float");

    /* Let the framework know we're done modifying EcsStruct */
    ecs_modified(world, position_type, EcsStruct);

    /* Create a dummy value */
    Position p = {10, 20};

    /* Serialize the value to a string */
    char *str = ecs_ptr_to_str(world, position_type, &p);
    printf("Position: %s\n", str);
    free(str);

    return ecs_fini(world);
}
