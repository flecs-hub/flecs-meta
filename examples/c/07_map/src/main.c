#include <map.h>

ECS_BITMASK(Toppings, {
    Bacon = 0x1,
    Lettuce = 0x2,
    Tomato = 0x4,
    Onion = 0x8,
    Egg = 0x10,
    Turkey = 0x20
});

ECS_STRUCT(Sandwich, {
    int32_t cost;
    Toppings toppings;
});

ECS_STRUCT(Menu, {
    ecs_map(ecs_string_t, Sandwich) items;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Toppings, Sandwich and Menu. Make sure 
     * that dependencies are registered before they are used. */
    ECS_META(world, Toppings);
    ECS_META(world, Sandwich);
    ECS_META(world, Menu);

    /* Create an instance of the Menu type */
    Menu m = { ecs_map_new(Sandwich, 0) };
    
    /* Insert a BLT sandwich. Because ecs_map_t only accepts 64bit integers as
     * key, we need to cast the string key. */
    Sandwich s = {2, Bacon | Lettuce | Tomato};
    ecs_map_set(m.items, (intptr_t)"BLT", &s);

    /* Insert another sandwich */
    s = (Sandwich){3, Bacon | Turkey | Onion};
    ecs_map_set(m.items, (intptr_t)"Turkey sub", &s);

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Menu), &m);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
