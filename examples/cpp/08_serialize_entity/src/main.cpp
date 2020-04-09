#include <serialize_entity.h>
#include <iostream>

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
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions */
    flecs::meta<Position>(world);
    flecs::meta<AgentKind>(world);
    flecs::meta<Agent>(world);

    auto e = flecs::entity(world, "Player One")
        .set<Position>({10, 20})
        .set<Agent>({ Human });

    /* Pretty print the player entity */
    std::cout << flecs::pretty_print(world, e) << std::endl;
}
