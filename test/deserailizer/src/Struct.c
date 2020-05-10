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
    bool before_arr_1;
    int32_t arr_1[2];
    bool before_arr_2;
    int32_t arr_2[2];
    bool after_arr_2;
});

void Struct_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Point);

    Point value = { 0 };
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

    Line value = { 0 };
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

void Struct_struct_by_name() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Point);

    Point value = { 0 };
    ecs_meta_iter_t it = ecs_meta_iter(world, ecs_entity(Point), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_cursor_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_move_cursor_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 11), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(value.x, 11);
    test_int(value.y, 22);

    ecs_fini(world);
}

void Struct_nested_struct_by_name() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Point);
    ECS_META(world, Line);

    Line value = { 0 };
    ecs_meta_iter_t it = ecs_meta_iter(world, ecs_entity(Line), &value);
    
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_move_cursor_name(&it, "stop"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_cursor_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 44), 0);
    test_int(ecs_meta_move_cursor_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 33), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_cursor_name(&it, "start"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_cursor_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_move_cursor_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 11), 0);
    test_int(ecs_meta_pop(&it), 0);   

    test_int(ecs_meta_pop(&it), 0);
    
    test_int(value.start.x, 11);
    test_int(value.start.y, 22);
    test_int(value.stop.x, 33);
    test_int(value.stop.y, 44);

    ecs_fini(world);
}

void Struct_struct_w_array() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Struct_w_array);

    Struct_w_array value = { 0 };
    ecs_meta_iter_t it = ecs_meta_iter(
        world, ecs_entity(Struct_w_array), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_bool(&it, true), 0); // before_arr_1
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // arr_1
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_set_bool(&it, false), 0); // before_arr_2
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // arr_2
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_set_bool(&it, true), 0); // after_arr_2
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_arr_1, true);
    test_bool(value.before_arr_2, false);
    test_bool(value.after_arr_2, true);

    test_int(value.arr_1[0], 10);
    test_int(value.arr_1[1], 20);

    test_int(value.arr_2[0], 30);
    test_int(value.arr_2[1], 40);

    ecs_fini(world);
}

void Struct_struct_w_array_by_name() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsComponentsMeta, 0);

    ECS_META(world, Struct_w_array);

    Struct_w_array value = { 0 };
    ecs_meta_iter_t it = ecs_meta_iter(
        world, ecs_entity(Struct_w_array), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_cursor_name(&it, "after_arr_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);

    test_int(ecs_meta_move_cursor_name(&it, "arr_2"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_move_cursor_name(&it, "before_arr_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_move_cursor_name(&it, "arr_1"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_cursor_name(&it, "before_arr_1"), 0);
    test_int(ecs_meta_set_bool(&it, false), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_arr_1, false);
    test_bool(value.before_arr_2, true);
    test_bool(value.after_arr_2, true);

    test_int(value.arr_1[0], 10);
    test_int(value.arr_1[1], 20);

    test_int(value.arr_2[0], 30);
    test_int(value.arr_2[1], 40);

    ecs_fini(world);
}
