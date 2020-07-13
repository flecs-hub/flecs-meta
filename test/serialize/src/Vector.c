#include <test.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

ECS_VECTOR(VectorBool, bool);
ECS_VECTOR(VectorInt, int32_t);
ECS_VECTOR(VectorString, ecs_string_t);
ECS_VECTOR(VectorEntity, ecs_entity_t);
ECS_VECTOR(VectorPoint, Point);
ECS_VECTOR(VectorLine, Line);
ECS_VECTOR(VectorVectorInt, VectorInt);
ECS_VECTOR(VectorVectorPoint, VectorPoint);
ECS_VECTOR(VectorVectorLine, VectorLine);

void Vector_vector_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorBool);

    {
    ecs_vector_t *value = ecs_vector_from_array(bool, 2, ((bool[]){true, false}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorBool), &value);
    test_str(str, "[true, false]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorInt);

    {
    ecs_vector_t *value = ecs_vector_from_array(int32_t, 2, ((int32_t[]){10, -10}));        
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorInt), &value);
    test_str(str, "[10, -10]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_string() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorString);

    {
    ecs_vector_t *value = ecs_vector_from_array(char*, 2, ((char*[]){"Hello", "World"}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorString), &value);
    test_str(str, "[\"Hello\", \"World\"]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_entity() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorEntity);

    ecs_entity_t e = ecs_set(world, 0, EcsName, {"Foo"});

    {
    ecs_vector_t *value = ecs_vector_from_array(ecs_entity_t, 3, ((ecs_entity_t[]){0, 1000, e}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorEntity), &value);
    test_str(str, "[0, 1000, Foo]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, VectorPoint);

    {
    ecs_vector_t *value = ecs_vector_from_array(Point, 2, ((Point[]){{10, 20}, {30, 40}}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorPoint), &value);
    test_str(str, "[{x = 10, y = 20}, {x = 30, y = 40}]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);
    ECS_META(world, VectorLine);

    {
    ecs_vector_t *value = ecs_vector_from_array(Line, 2, ((Line[]){
        {{10, 20}, {30, 40}},
        {{50, 60}, {70, 80}}
    }));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorLine), &value);
    test_str(str, "[{start = {x = 10, y = 20}, stop = {x = 30, y = 40}}, {start = {x = 50, y = 60}, stop = {x = 70, y = 80}}]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_vector_int() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorInt);
    ECS_META(world, VectorVectorInt);

    {
    ecs_vector_t *v1 = ecs_vector_new(int32_t, 2);
    *ecs_vector_add(&v1, int32_t) = 10;
    *ecs_vector_add(&v1, int32_t) = -20;

    ecs_vector_t *v2 = ecs_vector_new(int32_t, 2);
    *ecs_vector_add(&v2, int32_t) = 30;
    *ecs_vector_add(&v2, int32_t) = -40;

    ecs_vector_t *value = ecs_vector_from_array(ecs_vector_t*, 2, ((ecs_vector_t*[]){v1, v2}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorVectorInt), &value);
    test_str(str, "[[10, -20], [30, -40]]");
    ecs_os_free(str);
    ecs_vector_free(value);
    ecs_vector_free(v1);
    ecs_vector_free(v2);
    }

    ecs_fini(world);
}

void Vector_vector_vector_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, VectorPoint);
    ECS_META(world, VectorVectorPoint);

    {
    ecs_vector_t *v1 = ecs_vector_new(Point, 2);
    *ecs_vector_add(&v1, Point) = (Point){10, -20};
    *ecs_vector_add(&v1, Point) = (Point){30, -40};

    ecs_vector_t *v2 = ecs_vector_new(Point, 2);
    *ecs_vector_add(&v2, Point) = (Point){50, -60};
    *ecs_vector_add(&v2, Point) = (Point){70, -80};

    ecs_vector_t *value = ecs_vector_from_array(ecs_vector_t*, 2, ((ecs_vector_t*[]){v1, v2}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorVectorPoint), &value);
    test_str(str, "[[{x = 10, y = -20}, {x = 30, y = -40}], [{x = 50, y = -60}, {x = 70, y = -80}]]");
    ecs_os_free(str);
    ecs_vector_free(value);
    ecs_vector_free(v1);
    ecs_vector_free(v2);
    }

    ecs_fini(world);
}

void Vector_vector_vector_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);
    ECS_META(world, VectorLine);
    ECS_META(world, VectorVectorLine);

    {
    Line l1 = {{10, 20}, {30, 40}};
    Line l2 = {{50, 60}, {70, 80}};
    Line l3 = {{90, 100}, {110, 120}};
    Line l4 = {{130, 140}, {150, 160}};

    ecs_vector_t *v1 = ecs_vector_new(Line, 2);
    *ecs_vector_add(&v1, Line) = l1;
    *ecs_vector_add(&v1, Line) = l2;

    ecs_vector_t *v2 = ecs_vector_new(Line, 2);
    *ecs_vector_add(&v2, Line) = l3;
    *ecs_vector_add(&v2, Line) = l4;

    ecs_vector_t *value = ecs_vector_from_array(ecs_vector_t*, 2, ((ecs_vector_t*[]){v1, v2}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorVectorLine), &value);
    test_str(str, 
        "[[{start = {x = 10, y = 20}, stop = {x = 30, y = 40}}, "
          "{start = {x = 50, y = 60}, stop = {x = 70, y = 80}}], "
         "[{start = {x = 90, y = 100}, stop = {x = 110, y = 120}}, "
          "{start = {x = 130, y = 140}, stop = {x = 150, y = 160}}]]");

    ecs_os_free(str);
    ecs_vector_free(value);
    ecs_vector_free(v1);
    ecs_vector_free(v2);
    }

    ecs_fini(world);
}

void Vector_vector_null() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorBool);

    {
    ecs_vector_t *value = NULL;
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorBool), &value);
    test_str(str, "nullptr");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_vector_null() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorInt);
    ECS_META(world, VectorVectorInt);

    {
    ecs_vector_t *value = ecs_vector_from_array(ecs_vector_t*, 2, ((ecs_vector_t*[]){NULL, NULL}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorVectorInt), &value);
    test_str(str, "[nullptr, nullptr]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_empty() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorInt);
    ECS_META(world, VectorVectorInt);

    {
    ecs_vector_t *value = ecs_vector_new(ecs_vector_t*, 2);
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorVectorInt), &value);
    test_str(str, "[]");
    ecs_os_free(str);
    ecs_vector_free(value);
    }

    ecs_fini(world);
}

void Vector_vector_vector_empty() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, VectorInt);
    ECS_META(world, VectorVectorInt);

    {
    ecs_vector_t *v1 = ecs_vector_new(int32_t, 0);
    ecs_vector_t *v2 = ecs_vector_new(int32_t, 0);
    ecs_vector_t *value = ecs_vector_from_array(ecs_vector_t*, 2, ((ecs_vector_t*[]){v1, v2}));
    char *str = ecs_ptr_to_str(world, ecs_entity(VectorVectorInt), &value);
    test_str(str, "[[], []]");
    ecs_os_free(str);
    ecs_vector_free(value);
    ecs_vector_free(v1);
    ecs_vector_free(v2);
    }

    ecs_fini(world);
}

