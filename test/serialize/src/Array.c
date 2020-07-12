#include <test.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

ECS_STRUCT(Struct_w_array, {
    int32_t ints[2];
});

ECS_ARRAY(ArrayBool, bool, 2);
ECS_ARRAY(ArrayInt, int32_t, 2);
ECS_ARRAY(ArrayString, ecs_string_t, 2);
ECS_ARRAY(ArrayEntity, ecs_entity_t, 2);
ECS_ARRAY(ArrayPoint, Point, 2);
ECS_ARRAY(ArrayLine, Line, 2);
ECS_ARRAY(ArrayArrayInt, ArrayInt, 2);
ECS_ARRAY(ArrayArrayString, ArrayString, 2);
ECS_ARRAY(ArrayArrayPoint, ArrayPoint, 2);
ECS_ARRAY(ArrayArrayLine, ArrayLine, 2);

void Array_array_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayBool);

    {
    bool value[] = {true, false};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayBool), value);
    test_str(str, "[true, false]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayInt);

    {
    int32_t value[] = {10, -20};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayInt), value);
    test_str(str, "[10, -20]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_string() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayString);

    {
    char* value[] = {"Hello", "World"};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayString), value);
    test_str(str, "[\"Hello\", \"World\"]");
    ecs_os_free(str);
    }

    {
    char* value[] = {"Hello", NULL};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayString), value);
    test_str(str, "[\"Hello\", nullptr]");
    ecs_os_free(str);
    }    

    ecs_fini(world);
}

void Array_array_entity() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayEntity);

    ecs_entity_t e1 = 2000;
    ecs_entity_t e2 = ecs_set(world, 0, EcsName, {"Foo"});

    {
    ecs_entity_t value[] = {e1, e2};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayEntity), value);
    test_str(str, "[2000, Foo]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_struct_array_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_array);

    {
    Struct_w_array value = {{10, -20}};
    char *str = ecs_ptr_to_str(world, ecs_entity(Struct_w_array), &value);
    test_str(str, "{ints = [10, -20]}");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, ArrayPoint);

    {
    Point value[] = {{10, 20}, {30, 40}};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayPoint), &value);
    test_str(str, "[{x = 10, y = 20}, {x = 30, y = 40}]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);
    ECS_META(world, ArrayLine);

    {
    Line value[] = {{{10, 20}, {30, 40}}, {{50, 60}, {70, 80}}};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayLine), &value);
    test_str(str, "[{start = {x = 10, y = 20}, stop = {x = 30, y = 40}}, {start = {x = 50, y = 60}, stop = {x = 70, y = 80}}]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_array_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayInt);
    ECS_META(world, ArrayArrayInt);

    {
    int value[2][2] = {{10, 20}, {30, 40}};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayArrayInt), value);
    test_str(str, "[[10, 20], [30, 40]]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_array_string() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, ArrayString);
    ECS_META(world, ArrayArrayString);

    {
    char* value[2][2] = {{"Hello", "World"}, {"Foo", "Bar"}};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayArrayString), value);
    test_str(str, "[[\"Hello\", \"World\"], [\"Foo\", \"Bar\"]]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_array_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, ArrayPoint);
    ECS_META(world, ArrayArrayPoint);

    {
    Point value[2][2] = {{{10, 20}, {30, 40}}, {{50, 60}, {70, 80}}};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayArrayPoint), value);
    test_str(str, "[[{x = 10, y = 20}, {x = 30, y = 40}], [{x = 50, y = 60}, {x = 70, y = 80}]]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Array_array_array_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);
    ECS_META(world, ArrayLine);
    ECS_META(world, ArrayArrayLine);

    {
    Line l1 = {{10, 20}, {30, 40}};
    Line l2 = {{50, 60}, {70, 80}};
    Line l3 = {{90, 100}, {110, 120}};
    Line l4 = {{130, 140}, {150, 160}};

    Line value[2][2] = {{l1, l2}, {l3, l4}};
    char *str = ecs_ptr_to_str(world, ecs_entity(ArrayArrayLine), value);
    test_str(str, 
        "[[{start = {x = 10, y = 20}, stop = {x = 30, y = 40}}, "
          "{start = {x = 50, y = 60}, stop = {x = 70, y = 80}}], "
         "[{start = {x = 90, y = 100}, stop = {x = 110, y = 120}}, "
          "{start = {x = 130, y = 140}, stop = {x = 150, y = 160}}]]");
    ecs_os_free(str);
    }

    ecs_fini(world);
}
