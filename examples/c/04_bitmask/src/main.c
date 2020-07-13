#include <bitmask.h>

ECS_BITMASK(Toppings, {
    Bacon = 0x1,
    Lettuce = 0x2,
    Tomato = 0x4,
    Onion = 0x8,
    Egg = 0x10,
    Turkey = 0x20
});

ECS_STRUCT(Sandwich, {
    char *name;
    Toppings toppings;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions. Make sure to define Toppings before 
     * Sandwich, as Sandwich uses Toppings. */
    ECS_META(world, Toppings);
    ECS_META(world, Sandwich);

    /* Create an instance of the Sandwich type */
    Sandwich s = { "BLT", Bacon | Lettuce | Tomato };

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Sandwich), &s);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}