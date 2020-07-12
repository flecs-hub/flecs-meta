#include <test.h>

ECS_BITMASK(Toppings, {
    Bacon = 1,
    Lettuce = 2,
    Tomato = 4
});

void Bitmask_bitmask_1() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Toppings);

    {
    Toppings value = Bacon;
    char *str = ecs_ptr_to_str(world, ecs_entity(Toppings), &value);
    test_str(str, "Bacon");
    ecs_os_free(str);
    }  

    ecs_fini(world);
}

void Bitmask_bitmask_2() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Toppings);

    {
    Toppings value = Bacon | Lettuce;
    char *str = ecs_ptr_to_str(world, ecs_entity(Toppings), &value);
    test_str(str, "Bacon | Lettuce");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Bitmask_bitmask_3() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Toppings);

    {
    Toppings value = Bacon | Lettuce | Tomato;
    char *str = ecs_ptr_to_str(world, ecs_entity(Toppings), &value);
    test_str(str, "Tomato | Bacon | Lettuce");
    ecs_os_free(str);
    }    

    ecs_fini(world);
}

void Bitmask_bitmask_0_value() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Toppings);

    {
    Toppings value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(Toppings), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }    

    ecs_fini(world);
}
