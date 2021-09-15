#include <test.h>

ECS_ENUM(Color, {
    Red,
    Green,
    Blue
});

ECS_BITMASK(Toppings, {
    Bacon = 1,
    Lettuce = 2,
    Tomato = 4
});

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

ECS_ARRAY(ArrayInt, int32_t, 2);
ECS_VECTOR(VectorInt, int32_t);

ECS_MAP(MapBoolBool, bool, bool);
ECS_MAP(MapIntBool, int32_t, bool);
ECS_MAP(MapStringBool, ecs_string_t, bool);
ECS_MAP(MapColorBool, Color, bool);
ECS_MAP(MapToppingsBool, Toppings, bool);
ECS_MAP(MapIntInt, int32_t, int32_t);
ECS_MAP(MapIntString, int32_t, char*);
ECS_MAP(MapIntPoint, int32_t, Point);
ECS_MAP(MapIntLine, int32_t, Line);
ECS_MAP(MapIntArrayInt, int32_t, ArrayInt);
ECS_MAP(MapIntVectorInt, int32_t, VectorInt);
ECS_MAP(MapIntMapIntBool, int32_t, MapIntBool);

void Map_map_bool_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, MapBoolBool);

    {
    ecs_map_t *value = ecs_map_new(bool, 0);
    ecs_map_set(value, true, &(bool){false});
    ecs_map_set(value, false, &(bool){true});
    char *str = ecs_ptr_to_str(world, ecs_id(MapBoolBool), &value);
    test_str(str, "{false = true, true = false}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, MapIntBool);

    {
    ecs_map_t *value = ecs_map_new(bool, 0);
    ecs_map_set(value, 1, &(bool){false});
    ecs_map_set(value, 2, &(bool){true});

    char *str = ecs_ptr_to_str(world, ecs_id(MapIntBool), &value);
    test_str(str, "{1 = false, 2 = true}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_string_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, MapStringBool);

    {
    ecs_map_t *value = ecs_map_new(bool, 0);
    ecs_map_set(value, "Foo", &(bool){false});
    ecs_map_set(value, "Bar", &(bool){true});
    char *str = ecs_ptr_to_str(world, ecs_id(MapStringBool), &value);
    test_str(str, "{\"Foo\" = false, \"Bar\" = true}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_enum_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Color);
    ECS_META(world, MapColorBool);

    {
    ecs_map_t *value = ecs_map_new(bool, 0);
    ecs_map_set(value, Green, &(bool){false});
    ecs_map_set(value, Blue, &(bool){true});
    char *str = ecs_ptr_to_str(world, ecs_id(MapColorBool), &value);
    test_str(str, "{Green = false, Blue = true}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_bitmask_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Toppings);
    ECS_META(world, MapToppingsBool);

    {
    ecs_map_t *value = ecs_map_new(bool, 0);
    ecs_map_set(value, Bacon | Lettuce, &(bool){false});
    ecs_map_set(value, Bacon | Lettuce | Tomato, &(bool){true});
    char *str = ecs_ptr_to_str(world, ecs_id(MapToppingsBool), &value);
    test_str(str, "{Bacon | Lettuce = false, Tomato | Bacon | Lettuce = true}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, MapIntInt);

    {
    ecs_map_t *value = ecs_map_new(int32_t, 0);
    ecs_map_set(value, 1, &(int32_t){10});
    ecs_map_set(value, 2, &(int32_t){20});
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntInt), &value);
    test_str(str, "{1 = 10, 2 = 20}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_string() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, MapIntString);

    {
    ecs_map_t *value = ecs_map_new(char*, 0);
    ecs_map_set(value, 1, &(char*){"Hello"});
    ecs_map_set(value, 2, &(char*){"World"});
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntString), &value);
    test_str(str, "{1 = \"Hello\", 2 = \"World\"}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, MapIntPoint);

    {
    ecs_map_t *value = ecs_map_new(Point, 0);
    ecs_map_set(value, 1, (&(Point){10, 20}));
    ecs_map_set(value, 2, (&(Point){30, 40}));
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntPoint), &value);
    test_str(str, "{1 = {x = 10, y = 20}, 2 = {x = 30, y = 40}}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);
    ECS_META(world, MapIntLine);

    {
    ecs_map_t *value = ecs_map_new(Line, 0);
    ecs_map_set(value, 1, (&(Line){{10, 20}, {30, 40}}));
    ecs_map_set(value, 2, (&(Line){{50, 60}, {70, 80}}));
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntLine), &value);
    test_str(str, "{1 = {start = {x = 10, y = 20}, stop = {x = 30, y = 40}}, 2 = {start = {x = 50, y = 60}, stop = {x = 70, y = 80}}}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_array_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayInt);
    ECS_META(world, MapIntArrayInt);

    {
    ecs_map_t *value = ecs_map_new(ArrayInt, 0);
    ecs_map_set(value, 1, (&(int32_t[]){10, 20}));
    ecs_map_set(value, 2, (&(int32_t[]){30, 40}));
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntArrayInt), &value);
    test_str(str, "{1 = [10, 20], 2 = [30, 40]}");
    ecs_os_free(str);
    ecs_map_free(value);
    }

    ecs_fini(world);
}

void Map_map_int_vector_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorInt);
    ECS_META(world, MapIntVectorInt);

    {
    ecs_vector_t *v1 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){10, 20}));
    ecs_vector_t *v2 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){30, 40}));
    ecs_map_t *value = ecs_map_new(ArrayInt, 0);
    ecs_map_set(value, 1, &v1);
    ecs_map_set(value, 2, &v2);
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntVectorInt), &value);
    test_str(str, "{1 = [10, 20], 2 = [30, 40]}");
    ecs_os_free(str);
    ecs_map_free(value);
    ecs_vector_free(v1);
    ecs_vector_free(v2);
    }

    ecs_fini(world);
}

void Map_map_int_map_int_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, MapIntBool);
    ECS_META(world, MapIntMapIntBool);

    {
    ecs_map_t *m1 = ecs_map_new(bool, 0);
    ecs_map_set(m1, 1, &(bool){true});
    ecs_map_set(m1, 2, &(bool){false});

    ecs_map_t *m2 = ecs_map_new(bool, 0);
    ecs_map_set(m2, 3, &(bool){false});
    ecs_map_set(m2, 4, &(bool){true});

    ecs_map_t *value = ecs_map_new(ecs_map_t*, 0);
    ecs_map_set(value, 5, &m1);
    ecs_map_set(value, 6, &m2);
    char *str = ecs_ptr_to_str(world, ecs_id(MapIntMapIntBool), &value);
    test_str(str, "{5 = {1 = true, 2 = false}, 6 = {4 = true, 3 = false}}");
    ecs_os_free(str);
    ecs_map_free(value);
    ecs_map_free(m1);
    ecs_map_free(m2);
    }

    ecs_fini(world);
}
