#include <serialize_entity.h>

ECS_STRUCT(Position, {
    float x;
    float y;
});

ECS_ENUM(AgentKind, {
    Human,
    AI
});

ECS_STRUCT(Agent, {
    AgentKind kind;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Position, AgentKind and Agent. Make sure 
     * that dependencies are registered before they are used. */
    ECS_META(world, Position);
    ECS_META(world, AgentKind);
    ECS_META(world, Agent);

    /* Create a player entity */
    ecs_entity_t player = ecs_set(world, ecs_set(world, ecs_set(world, 0,
        EcsName, {"Player One"}),
        Agent, {Human}),
        Position, {10, 20});

    /* Pretty print the player entity */
    char *str = ecs_entity_to_str(world, player);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
