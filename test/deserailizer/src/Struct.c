#include <test.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

ECS_STRUCT(Primitives, {
    bool b;
    char ch;
    ecs_byte_t byte;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    char *str;
    ecs_entity_t e;
});

ECS_STRUCT(Struct_w_array, {
    bool before_arr_1;
    int32_t arr_1[2];
    bool before_arr_2;
    int32_t arr_2[2];
    bool after_arr_2;
});

ECS_STRUCT(Struct_w_array_nested_struct, {
    bool before_arr_1;
    Point arr_1[2];
    bool before_arr_2;
    Point arr_2[2];
    bool after_arr_2;
});

ECS_STRUCT(Struct_w_vector, {
    bool before_vec_1;
    ecs_vector(int32_t) vec_1;
    bool before_vec_2;
    ecs_vector(int32_t) vec_2;
    bool after_vec_2;
});

ECS_STRUCT(Struct_w_vector_nested_struct, {
    bool before_vec_1;
    ecs_vector(Point) vec_1;
    bool before_vec_2;
    ecs_vector(Point) vec_2;
    bool after_vec_2;
});

void Struct_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);

    Point value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Point), &value);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 11), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(value.x, 11);
    test_int(value.y, 22);

    ecs_fini(world);
}

void Struct_struct_primitives() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Primitives);

    Primitives value = { 0 };

    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Primitives), &value);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_bool(&it, true), 0); // b
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_char(&it, 'a'), 0); // ch
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_uint(&it, 64), 0); // byte

    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, ECS_MAX_I8), 0); // i8
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, ECS_MAX_I16), 0); // i16
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, ECS_MAX_I32), 0); // i32
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, ECS_MAX_I64), 0); // i64

    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_uint(&it, ECS_MAX_U8), 0); // u8
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_uint(&it, ECS_MAX_U16), 0); // u16
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_uint(&it, ECS_MAX_U32), 0); // u32
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_uint(&it, ECS_MAX_U64), 0); // u64
    
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_string(&it, "Hello"), 0); // str

    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_entity(&it, 2000), 0); // str

    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.b, true);
    test_int(value.ch, 'a');
    test_uint(value.byte, 64);

    test_int(value.i8, ECS_MAX_I8);
    test_int(value.i16, ECS_MAX_I16);
    test_int(value.i32, ECS_MAX_I32);
    test_int(value.i64, ECS_MAX_I64);

    test_int(value.u8, ECS_MAX_U8);
    test_int(value.u16, ECS_MAX_U16);
    test_int(value.u32, ECS_MAX_U32);
    test_int(value.u64, ECS_MAX_U64);

    test_str(value.str, "Hello");
    test_int(value.e, 2000);

    ecs_fini(world);
}

void Struct_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);

    Line value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Line), &value);
    
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

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);

    Point value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Point), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 11), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(value.x, 11);
    test_int(value.y, 22);

    ecs_fini(world);
}

void Struct_nested_struct_by_name() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Line);

    Line value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Line), &value);
    
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_move_name(&it, "stop"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 44), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 33), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_name(&it, "start"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 22), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
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

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_array);

    Struct_w_array value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
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

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_array);

    Struct_w_array value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_array), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "after_arr_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);

    test_int(ecs_meta_move_name(&it, "arr_2"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_move_name(&it, "before_arr_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_move_name(&it, "arr_1"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_name(&it, "before_arr_1"), 0);
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

void Struct_struct_w_array_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Struct_w_array_nested_struct);

    Struct_w_array_nested_struct value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_array_nested_struct), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_bool(&it, true), 0); // before_arr_1
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // arr_1

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0); // arr_1
    
    test_int(ecs_meta_set_bool(&it, false), 0); // before_arr_2
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // arr_2

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_set_int(&it, 50), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_set_int(&it, 70), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 80), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0); // arr_2

    test_int(ecs_meta_set_bool(&it, true), 0); // after_arr_2
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_arr_1, true);
    test_bool(value.before_arr_2, false);
    test_bool(value.after_arr_2, true);

    test_int(value.arr_1[0].x, 10);
    test_int(value.arr_1[0].y, 20);
    test_int(value.arr_1[1].x, 30);
    test_int(value.arr_1[1].y, 40);

    test_int(value.arr_2[0].x, 50);
    test_int(value.arr_2[0].y, 60);
    test_int(value.arr_2[1].x, 70);
    test_int(value.arr_2[1].y, 80);

    ecs_fini(world);
}

void Struct_struct_w_array_nested_struct_by_name() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Struct_w_array_nested_struct);

    Struct_w_array_nested_struct value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_array_nested_struct), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "after_arr_2"), 0);
    test_int(ecs_meta_set_bool(&it, false), 0);

    test_int(ecs_meta_move_name(&it, "arr_2"), 0);
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 50), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 80), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 70), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_move_name(&it, "before_arr_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);

    test_int(ecs_meta_move_name(&it, "arr_1"), 0);
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_name(&it, "before_arr_1"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_arr_1, true);
    test_bool(value.before_arr_2, true);
    test_bool(value.after_arr_2, false);

    test_int(value.arr_1[0].x, 10);
    test_int(value.arr_1[0].y, 20);
    test_int(value.arr_1[1].x, 30);
    test_int(value.arr_1[1].y, 40);

    test_int(value.arr_2[0].x, 50);
    test_int(value.arr_2[0].y, 60);
    test_int(value.arr_2[1].x, 70);
    test_int(value.arr_2[1].y, 80);

    ecs_fini(world);
}

void Struct_struct_w_vector() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_vector);

    Struct_w_vector value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_bool(&it, true), 0); // before_vec_1
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_1
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_set_bool(&it, false), 0); // before_vec_2
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_2
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_set_bool(&it, true), 0); // after_vec_2
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_vec_1, true);
    test_bool(value.before_vec_2, false);
    test_bool(value.after_vec_2, true);

    test_int(ecs_vector_count(value.vec_1), 2);
    test_int(ecs_vector_count(value.vec_2), 2);

    int32_t *arr_1 = ecs_vector_first(value.vec_1, int32_t);
    test_assert(arr_1 != NULL);

    int32_t *arr_2 = ecs_vector_first(value.vec_2, int32_t);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0], 10);
    test_int(arr_1[1], 20);

    test_int(arr_2[0], 30);
    test_int(arr_2[1], 40);

    ecs_fini(world);
}

void Struct_struct_w_vector_by_name() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_vector);

    Struct_w_vector value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "after_vec_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);

    test_int(ecs_meta_move_name(&it, "vec_2"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_move_name(&it, "before_vec_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_move_name(&it, "vec_1"), 0);
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_name(&it, "before_vec_1"), 0);
    test_int(ecs_meta_set_bool(&it, false), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_vec_1, false);
    test_bool(value.before_vec_2, true);
    test_bool(value.after_vec_2, true);

    test_int(ecs_vector_count(value.vec_1), 2);
    test_int(ecs_vector_count(value.vec_2), 2);

    int32_t *arr_1 = ecs_vector_first(value.vec_1, int32_t);
    test_assert(arr_1 != NULL);

    int32_t *arr_2 = ecs_vector_first(value.vec_2, int32_t);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0], 10);
    test_int(arr_1[1], 20);

    test_int(arr_2[0], 30);
    test_int(arr_2[1], 40);

    ecs_fini(world);
}

void Struct_struct_w_vector_nested_struct() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Struct_w_vector_nested_struct);

    Struct_w_vector_nested_struct value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector_nested_struct), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_set_bool(&it, true), 0); // before_vec_1
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_1

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0); // arr_1
    
    test_int(ecs_meta_set_bool(&it, false), 0); // before_vec_2
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_2

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_set_int(&it, 50), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_set_int(&it, 70), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 80), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0); // arr_2

    test_int(ecs_meta_set_bool(&it, true), 0); // after_vec_2
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_vec_1, true);
    test_bool(value.before_vec_2, false);
    test_bool(value.after_vec_2, true);

    test_int(ecs_vector_count(value.vec_1), 2);
    test_int(ecs_vector_count(value.vec_2), 2);

    Point *arr_1 = ecs_vector_first(value.vec_1, Point);
    test_assert(arr_1 != NULL);

    Point *arr_2 = ecs_vector_first(value.vec_2, Point);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0].x, 10);
    test_int(arr_1[0].y, 20);
    test_int(arr_1[1].x, 30);
    test_int(arr_1[1].y, 40);

    test_int(arr_2[0].x, 50);
    test_int(arr_2[0].y, 60);
    test_int(arr_2[1].x, 70);
    test_int(arr_2[1].y, 80);

    ecs_fini(world);  
}

void Struct_struct_w_vector_nested_struct_by_name() {
  ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Point);
    ECS_META(world, Struct_w_vector_nested_struct);

    Struct_w_vector_nested_struct value = { 0 };
    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector_nested_struct), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_move_name(&it, "after_vec_2"), 0);
    test_int(ecs_meta_set_bool(&it, false), 0);

    test_int(ecs_meta_move_name(&it, "vec_2"), 0);
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 50), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 80), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 70), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0);
    
    test_int(ecs_meta_move_name(&it, "before_vec_2"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);

    test_int(ecs_meta_move_name(&it, "vec_1"), 0);
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_push(&it), 0); // Point (elem 0)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 20), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 10), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_push(&it), 0); // Point (elem 1)
    test_int(ecs_meta_move_name(&it, "y"), 0);
    test_int(ecs_meta_set_int(&it, 40), 0);
    test_int(ecs_meta_move_name(&it, "x"), 0);
    test_int(ecs_meta_set_int(&it, 30), 0);
    test_int(ecs_meta_pop(&it), 0); // Point

    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_move_name(&it, "before_vec_1"), 0);
    test_int(ecs_meta_set_bool(&it, true), 0);
    test_int(ecs_meta_pop(&it), 0);
    
    test_bool(value.before_vec_1, true);
    test_bool(value.before_vec_2, true);
    test_bool(value.after_vec_2, false);

    test_int(ecs_vector_count(value.vec_1), 2);
    test_int(ecs_vector_count(value.vec_2), 2);

    Point *arr_1 = ecs_vector_first(value.vec_1, Point);
    test_assert(arr_1 != NULL);

    Point *arr_2 = ecs_vector_first(value.vec_2, Point);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0].x, 10);
    test_int(arr_1[0].y, 20);
    test_int(arr_1[1].x, 30);
    test_int(arr_1[1].y, 40);

    test_int(arr_2[0].x, 50);
    test_int(arr_2[0].y, 60);
    test_int(arr_2[1].x, 70);
    test_int(arr_2[1].y, 80);

    ecs_fini(world);
}

void Struct_struct_reassign_string() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Primitives);

    Primitives value = { 0 };

    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Primitives), &value);
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_move_name(&it, "str"), 0);
    test_int(ecs_meta_set_string(&it, "Hello"), 0); // str
    test_str(value.str, "Hello");

    test_int(ecs_meta_move_name(&it, "str"), 0);
    test_int(ecs_meta_set_string(&it, "World"), 0); // str
    test_str(value.str, "World");

    test_int(ecs_meta_pop(&it), 0);

    test_str(value.str, "World");

    ecs_fini(world);
}

void Struct_struct_reassign_null() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Primitives);

    Primitives value = { 0 };

    ecs_meta_cursor_t it = ecs_meta_cursor(world, ecs_entity(Primitives), &value);
    test_int(ecs_meta_push(&it), 0);

    test_int(ecs_meta_move_name(&it, "str"), 0);
    test_int(ecs_meta_set_string(&it, "Hello"), 0); // str
    test_str(value.str, "Hello");

    test_int(ecs_meta_move_name(&it, "str"), 0);
    test_int(ecs_meta_set_null(&it), 0); // str
    test_str(value.str, NULL);

    test_int(ecs_meta_pop(&it), 0);

    test_str(value.str, NULL);

    ecs_fini(world);
}

void Struct_struct_reassign_vector() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_vector);

    Struct_w_vector value = {
        .vec_1 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){10, 20})),
        .vec_2 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){30, 40}))
    };

    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_1
    test_int(ecs_meta_set_int(&it, 50), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_vector_count(value.vec_1), 2);
    test_int(ecs_vector_count(value.vec_2), 2);

    int32_t *arr_1 = ecs_vector_first(value.vec_1, int32_t);
    test_assert(arr_1 != NULL);

    int32_t *arr_2 = ecs_vector_first(value.vec_2, int32_t);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0], 50);
    test_int(arr_1[1], 60);

    test_int(arr_2[0], 30);
    test_int(arr_2[1], 40);    

    ecs_fini(world);
}

void Struct_struct_reassign_smaller_vector() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_vector);

    Struct_w_vector value = {
        .vec_1 = ecs_vector_from_array(int32_t, 3, ((int32_t[]){10, 20, 30})),
        .vec_2 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){40, 50}))
    };

    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_1
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 70), 0);

    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_vector_count(value.vec_1), 2);
    test_int(ecs_vector_count(value.vec_2), 2);

    int32_t *arr_1 = ecs_vector_first(value.vec_1, int32_t);
    test_assert(arr_1 != NULL);

    int32_t *arr_2 = ecs_vector_first(value.vec_2, int32_t);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0], 60);
    test_int(arr_1[1], 70);

    test_int(arr_2[0], 40);
    test_int(arr_2[1], 50);    

    ecs_fini(world);
}

void Struct_struct_reassign_larger_vector() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_vector);

    Struct_w_vector value = {
        .vec_1 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){10, 20})),
        .vec_2 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){30, 40}))
    };

    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_next(&it), 0);

    test_int(ecs_meta_push(&it), 0); // vec_1
    test_int(ecs_meta_set_int(&it, 50), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 60), 0);
    test_int(ecs_meta_next(&it), 0);
    test_int(ecs_meta_set_int(&it, 70), 0);
    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_vector_count(value.vec_1), 3);
    test_int(ecs_vector_count(value.vec_2), 2);

    int32_t *arr_1 = ecs_vector_first(value.vec_1, int32_t);
    test_assert(arr_1 != NULL);

    int32_t *arr_2 = ecs_vector_first(value.vec_2, int32_t);
    test_assert(arr_2 != NULL);

    test_int(arr_1[0], 50);
    test_int(arr_1[1], 60);
    test_int(arr_1[2], 70);

    test_int(arr_2[0], 30);
    test_int(arr_2[1], 40);    

    ecs_fini(world);
}

void Struct_struct_reassign_vector_null() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ECS_META(world, Struct_w_vector);

    Struct_w_vector value = {
        .vec_1 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){10, 20})),
        .vec_2 = ecs_vector_from_array(int32_t, 2, ((int32_t[]){30, 40}))
    };

    ecs_meta_cursor_t it = ecs_meta_cursor(
        world, ecs_entity(Struct_w_vector), &value);
    
    test_int(ecs_meta_push(&it), 0);
    test_int(ecs_meta_next(&it), 0);

    ecs_meta_set_null(&it);

    test_int(ecs_meta_pop(&it), 0);

    test_int(ecs_vector_count(value.vec_1), 0);
    test_int(ecs_vector_count(value.vec_2), 2);

    int32_t *arr_1 = ecs_vector_first(value.vec_1, int32_t);
    test_assert(arr_1 == NULL);

    int32_t *arr_2 = ecs_vector_first(value.vec_2, int32_t);
    test_assert(arr_2 != NULL);

    test_int(arr_2[0], 30);
    test_int(arr_2[1], 40);    

    ecs_fini(world);
}
