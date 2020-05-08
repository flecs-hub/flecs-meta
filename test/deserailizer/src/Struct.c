#include <test.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

void Struct_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Point);

    Point value = {10, 20};
    ecs_meta_iter_t it = ecs_meta_iter(world, ecs_entity(Point), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 11), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(value.x, 11);
    test_int(value.y, 22);

    ecs_fini(world);
}

void Struct_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Point);
    ECS_META(world, Line);

    Line value = {{10, 20}, {30, 40}};
    ecs_meta_iter_t it = ecs_meta_iter(world, ecs_entity(Line), &value);
    
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 11), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 33), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 44), 0);
    test_int(ecs_meta_pop(&it), 0);   

    test_int(ecs_meta_pop(&it), 0); 
    
    test_int(value.start.x, 11);
    test_int(value.start.y, 22);
    test_int(value.stop.x, 33);
    test_int(value.stop.y, 44);

    ecs_fini(world);
}
