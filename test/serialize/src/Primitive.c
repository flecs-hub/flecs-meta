#include <test.h>

void Primitive_bool() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(bool) = ecs_lookup_fullpath(world, "flecs.core.bool");
    test_assert(ecs_entity(bool) != 0);

    {
    bool value = true;
    char *str = ecs_ptr_to_str(world, ecs_entity(bool), &value);
    test_str(str, "true");
    ecs_os_free(str);
    }
    {
    bool value = false;
    char *str = ecs_ptr_to_str(world, ecs_entity(bool), &value);
    test_str(str, "false");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Primitive_byte() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);
    ecs_entity_t ecs_entity(ecs_byte_t) = ecs_lookup_fullpath(world, "flecs.core.ecs_byte_t");
    test_assert(ecs_entity(ecs_byte_t) != 0);

    {
    ecs_byte_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_byte_t), &value);
    test_str(str, "0x0");
    ecs_os_free(str);
    }

    {
    ecs_byte_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_byte_t), &value);
    test_str(str, "0xa");
    ecs_os_free(str);
    }

    {
    ecs_byte_t value = 11;
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_byte_t), &value);
    test_str(str, "0xb");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Primitive_char() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(char) = ecs_lookup_fullpath(world, "flecs.core.char");
    test_assert(ecs_entity(char) != 0);

    {
    char value = 'a';
    char *str = ecs_ptr_to_str(world, ecs_entity(char), &value);
    test_str(str, "'a'");
    ecs_os_free(str);
    }

    {
    char value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(char), &value);
    test_str(str, "''");
    ecs_os_free(str);
    }

    {
    char value = '\n';
    char *str = ecs_ptr_to_str(world, ecs_entity(char), &value);
    test_str(str, "'\\n'");
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Primitive_i8() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(int8_t) = ecs_lookup_fullpath(world, "flecs.core.int8_t");
    test_assert(ecs_entity(int8_t) != 0);

    {
    int8_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(int8_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    int8_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int8_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    int8_t value = -10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int8_t), &value);
    test_str(str, "-10");
    ecs_os_free(str);
    }    

    {
    int8_t value = ECS_MAX_I8;
    char *str = ecs_ptr_to_str(world, ecs_entity(int8_t), &value);
    test_str(str, ECS_MAX_I8_STR);
    ecs_os_free(str);
    }    

    {
    int8_t value = ECS_MIN_I8;
    char *str = ecs_ptr_to_str(world, ecs_entity(int8_t), &value);
    test_str(str, ECS_MIN_I8_STR);
    ecs_os_free(str);
    }        

    ecs_fini(world);
}

void Primitive_i16() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(int16_t) = ecs_lookup_fullpath(world, "flecs.core.int16_t");
    test_assert(ecs_entity(int16_t) != 0);

    {
    int16_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(int16_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    int16_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int16_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    int16_t value = -10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int16_t), &value);
    test_str(str, "-10");
    ecs_os_free(str);
    }    

    {
    int16_t value = ECS_MAX_I16;
    char *str = ecs_ptr_to_str(world, ecs_entity(int16_t), &value);
    test_str(str, ECS_MAX_I16_STR);
    ecs_os_free(str);
    }    

    {
    int16_t value = ECS_MIN_I16;
    char *str = ecs_ptr_to_str(world, ecs_entity(int16_t), &value);
    test_str(str, ECS_MIN_I16_STR);
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Primitive_i32() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(int32_t) = ecs_lookup_fullpath(world, "flecs.core.int32_t");
    test_assert(ecs_entity(int32_t) != 0);

    {
    int32_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(int32_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    int32_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int32_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    int32_t value = -10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int32_t), &value);
    test_str(str, "-10");
    ecs_os_free(str);
    }    

    {
    int32_t value = ECS_MAX_I32;
    char *str = ecs_ptr_to_str(world, ecs_entity(int32_t), &value);
    test_str(str, ECS_MAX_I32_STR);
    ecs_os_free(str);
    }    

    {
    int32_t value = ECS_MIN_I32;
    char *str = ecs_ptr_to_str(world, ecs_entity(int32_t), &value);
    test_str(str, ECS_MIN_I32_STR);
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Primitive_i64() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(int64_t) = ecs_lookup_fullpath(world, "flecs.core.int64_t");
    test_assert(ecs_entity(int64_t) != 0);

    {
    int64_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(int64_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    int64_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int64_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    int64_t value = -10;
    char *str = ecs_ptr_to_str(world, ecs_entity(int64_t), &value);
    test_str(str, "-10");
    ecs_os_free(str);
    }    

    {
    int64_t value = ECS_MAX_I64;
    char *str = ecs_ptr_to_str(world, ecs_entity(int64_t), &value);
    test_str(str, ECS_MAX_I64_STR);
    ecs_os_free(str);
    }    

    {
    int64_t value = ECS_MIN_I64;
    char *str = ecs_ptr_to_str(world, ecs_entity(int64_t), &value);
    test_str(str, ECS_MIN_I64_STR);
    ecs_os_free(str);
    }

    ecs_fini(world);
}

void Primitive_iptr() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(intptr_t) = ecs_lookup_fullpath(world, "flecs.core.intptr_t");
    test_assert(ecs_entity(intptr_t) != 0);

    {
    intptr_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(intptr_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    intptr_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(intptr_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    intptr_t value = -10;
    char *str = ecs_ptr_to_str(world, ecs_entity(intptr_t), &value);
    test_str(str, "-10");
    ecs_os_free(str);
    }    

    ecs_fini(world);
}

void Primitive_u8() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(uint8_t) = ecs_lookup_fullpath(world, "flecs.core.uint8_t");
    test_assert(ecs_entity(uint8_t) != 0);

    {
    uint8_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint8_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    uint8_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint8_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    uint8_t value = ECS_MAX_U8;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint8_t), &value);
    test_str(str, ECS_MAX_U8_STR);
    ecs_os_free(str);
    }          

    ecs_fini(world);
}

void Primitive_u16() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(uint16_t) = ecs_lookup_fullpath(world, "flecs.core.uint16_t");
    test_assert(ecs_entity(uint16_t) != 0);

    {
    uint16_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint16_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    uint16_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint16_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    uint16_t value = ECS_MAX_U16;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint16_t), &value);
    test_str(str, ECS_MAX_U16_STR);
    ecs_os_free(str);
    }          

    ecs_fini(world);
}

void Primitive_u32() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(uint32_t) = ecs_lookup_fullpath(world, "flecs.core.uint32_t");
    test_assert(ecs_entity(uint32_t) != 0);

    {
    uint32_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint32_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    uint32_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint32_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    uint32_t value = ECS_MAX_U32;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint32_t), &value);
    test_str(str, ECS_MAX_U32_STR);
    ecs_os_free(str);
    }          

    ecs_fini(world);
}

void Primitive_u64() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(uint64_t) = ecs_lookup_fullpath(world, "flecs.core.uint64_t");
    test_assert(ecs_entity(uint64_t) != 0);

    {
    uint64_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint64_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    uint64_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint64_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    {
    uint64_t value = ECS_MAX_U64;
    char *str = ecs_ptr_to_str(world, ecs_entity(uint64_t), &value);
    test_str(str, ECS_MAX_U64_STR);
    ecs_os_free(str);
    }          

    ecs_fini(world);
}

void Primitive_uptr() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(uintptr_t) = ecs_lookup_fullpath(world, "flecs.core.uintptr_t");
    test_assert(ecs_entity(uintptr_t) != 0);

    {
    uintptr_t value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(uintptr_t), &value);
    test_str(str, "0");
    ecs_os_free(str);
    }

    {
    uintptr_t value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(uintptr_t), &value);
    test_str(str, "10");
    ecs_os_free(str);
    }    

    ecs_fini(world);
}

void Primitive_float() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(float) = ecs_lookup_fullpath(world, "flecs.core.float");
    test_assert(ecs_entity(float) != 0);

    {
    float value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(float), &value);
    test_str(str, "0.000000");
    ecs_os_free(str);
    }

    {
    float value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(float), &value);
    test_str(str, "10.000000");
    ecs_os_free(str);
    }    

    {
    float value = 10.5;
    char *str = ecs_ptr_to_str(world, ecs_entity(float), &value);
    test_str(str, "10.500000");
    ecs_os_free(str);
    }        

    {
    float value = -10.5;
    char *str = ecs_ptr_to_str(world, ecs_entity(float), &value);
    test_str(str, "-10.500000");
    ecs_os_free(str);
    }        


    ecs_fini(world);
}

void Primitive_double() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(double) = ecs_lookup_fullpath(world, "flecs.core.double");
    test_assert(ecs_entity(double) != 0);

    {
    double value = 0;
    char *str = ecs_ptr_to_str(world, ecs_entity(double), &value);
    test_str(str, "0.000000");
    ecs_os_free(str);
    }

    {
    double value = 10;
    char *str = ecs_ptr_to_str(world, ecs_entity(double), &value);
    test_str(str, "10.000000");
    ecs_os_free(str);
    }    

    {
    double value = 10.5;
    char *str = ecs_ptr_to_str(world, ecs_entity(double), &value);
    test_str(str, "10.500000");
    ecs_os_free(str);
    }   

    {
    double value = -10.5;
    char *str = ecs_ptr_to_str(world, ecs_entity(double), &value);
    test_str(str, "-10.500000");
    ecs_os_free(str);
    }            

    ecs_fini(world);
}

void Primitive_string() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(ecs_string_t) = ecs_lookup_fullpath(world, "flecs.core.ecs_string_t");
    test_assert(ecs_entity(ecs_string_t) != 0);

    {
    ecs_string_t value = "Hello World";
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_string_t), &value);
    test_str(str, "\"Hello World\"");
    ecs_os_free(str);
    }

    {
    ecs_string_t value = NULL;
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_string_t), &value);
    test_str(str, "nullptr");
    ecs_os_free(str);
    }    

    {
    ecs_string_t value = "\"";
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_string_t), &value);
    test_str(str, "\"\\\"\"");
    ecs_os_free(str);
    }  

    {
    ecs_string_t value = "\n";
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_string_t), &value);
    test_str(str, "\"\\n\"");
    ecs_os_free(str);
    }        

    ecs_fini(world);
}

void Primitive_entity() {
    ecs_world_t *world = ecs_init();

    ECS_IMPORT(world, FlecsMeta);

    ecs_entity_t ecs_entity(ecs_entity_t) = ecs_lookup_fullpath(world, "flecs.core.ecs_entity_t");
    test_assert(ecs_entity(ecs_entity_t) != 0);

    {
    ecs_entity_t value = 2000;
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_entity_t), &value);
    test_str(str, "2000");
    ecs_os_free(str);
    }

    {
    ecs_entity_t value = ecs_set(world, 10, EcsName, {"Foo"});
    char *str = ecs_ptr_to_str(world, ecs_entity(ecs_entity_t), &value);
    test_str(str, "Foo");
    ecs_os_free(str);
    }
    
    ecs_fini(world);
}
