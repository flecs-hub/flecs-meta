#include <test.h>

ECS_ENUM(Color, {
    Red,
    Green,
    Blue
});

ECS_ENUM(Primes, {
    One = 1,
    Two = 2,
    Three = 3,
    Five = 5,
    Seven = 7
});

void Enum_enum() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Color);

    {
    Color value = Red;
    char *str = ecs_ptr_to_str(world, ecs_entity(Color), &value);
    test_str(str, "Red");
    ecs_os_free(str);
    }

    {
    Color value = Green;
    char *str = ecs_ptr_to_str(world, ecs_entity(Color), &value);
    test_str(str, "Green");
    ecs_os_free(str);
    }

    {
    Color value = Blue;
    char *str = ecs_ptr_to_str(world, ecs_entity(Color), &value);
    test_str(str, "Blue");
    ecs_os_free(str);
    }    

    ecs_fini(world);
}

void Enum_enum_explicit_values() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Primes);

    {
    Primes value = One;
    char *str = ecs_ptr_to_str(world, ecs_entity(Primes), &value);
    test_str(str, "One");
    ecs_os_free(str);
    }

    {
    Primes value = Two;
    char *str = ecs_ptr_to_str(world, ecs_entity(Primes), &value);
    test_str(str, "Two");
    ecs_os_free(str);
    }

    {
    Primes value = Three;
    char *str = ecs_ptr_to_str(world, ecs_entity(Primes), &value);
    test_str(str, "Three");
    ecs_os_free(str);
    }    

    {
    Primes value = Five;
    char *str = ecs_ptr_to_str(world, ecs_entity(Primes), &value);
    test_str(str, "Five");
    ecs_os_free(str);
    }    

    {
    Primes value = Seven;
    char *str = ecs_ptr_to_str(world, ecs_entity(Primes), &value);
    test_str(str, "Seven");
    ecs_os_free(str);
    }            

    ecs_fini(world);
}

void Enum_enum_invalid_value() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Color);

    {
    Color value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(Color), &value);
    test_str(str, NULL);
    ecs_os_free(str);
    }

    ecs_fini(world);
}
