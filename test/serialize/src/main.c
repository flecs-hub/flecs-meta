
/* A friendly warning from bake.test
 * ----------------------------------------------------------------------------
 * This file is generated. To add/remove testcases modify the 'project.json' of
 * the test project. ANY CHANGE TO THIS FILE IS LOST AFTER (RE)BUILDING!
 * ----------------------------------------------------------------------------
 */

#include <test.h>

// Testsuite 'Primitive'
void Primitive_bool(void);
void Primitive_byte(void);
void Primitive_char(void);
void Primitive_i8(void);
void Primitive_i16(void);
void Primitive_i32(void);
void Primitive_i64(void);
void Primitive_iptr(void);
void Primitive_u8(void);
void Primitive_u16(void);
void Primitive_u32(void);
void Primitive_u64(void);
void Primitive_uptr(void);
void Primitive_float(void);
void Primitive_double(void);
void Primitive_string(void);
void Primitive_entity(void);

// Testsuite 'Struct'
void Struct_struct(void);
void Struct_nested_struct(void);
void Struct_struct_bool_i32(void);
void Struct_struct_i32_bool(void);

// Testsuite 'Enum'
void Enum_enum(void);
void Enum_enum_explicit_values(void);
void Enum_enum_invalid_value(void);

// Testsuite 'Bitmask'
void Bitmask_bitmask_1(void);
void Bitmask_bitmask_2(void);
void Bitmask_bitmask_3(void);
void Bitmask_bitmask_0_value(void);

// Testsuite 'Array'
void Array_array_bool(void);
void Array_array_int(void);
void Array_array_string(void);
void Array_array_entity(void);
void Array_struct_array_int(void);
void Array_array_struct(void);
void Array_array_nested_struct(void);
void Array_array_array_int(void);
void Array_array_array_string(void);
void Array_array_array_struct(void);
void Array_array_array_nested_struct(void);

// Testsuite 'Vector'
void Vector_vector_bool(void);
void Vector_vector_int(void);
void Vector_vector_string(void);
void Vector_vector_entity(void);
void Vector_vector_struct(void);
void Vector_vector_nested_struct(void);
void Vector_vector_vector_int(void);
void Vector_vector_vector_struct(void);
void Vector_vector_vector_nested_struct(void);
void Vector_vector_empty(void);
void Vector_vector_vector_empty(void);
void Vector_vector_null(void);
void Vector_vector_vector_null(void);

// Testsuite 'Map'
void Map_map_bool_bool(void);
void Map_map_int_bool(void);
void Map_map_string_bool(void);
void Map_map_enum_bool(void);
void Map_map_bitmask_bool(void);
void Map_map_int_int(void);
void Map_map_int_string(void);
void Map_map_int_struct(void);
void Map_map_int_nested_struct(void);
void Map_map_int_array_int(void);
void Map_map_int_vector_int(void);
void Map_map_int_map_int_bool(void);

static bake_test_suite suites[] = {
    {
        .id = "Primitive",
        .testcase_count = 17,
        .testcases = (bake_test_case[]){
            {
                .id = "bool",
                .function = Primitive_bool
            },
            {
                .id = "byte",
                .function = Primitive_byte
            },
            {
                .id = "char",
                .function = Primitive_char
            },
            {
                .id = "i8",
                .function = Primitive_i8
            },
            {
                .id = "i16",
                .function = Primitive_i16
            },
            {
                .id = "i32",
                .function = Primitive_i32
            },
            {
                .id = "i64",
                .function = Primitive_i64
            },
            {
                .id = "iptr",
                .function = Primitive_iptr
            },
            {
                .id = "u8",
                .function = Primitive_u8
            },
            {
                .id = "u16",
                .function = Primitive_u16
            },
            {
                .id = "u32",
                .function = Primitive_u32
            },
            {
                .id = "u64",
                .function = Primitive_u64
            },
            {
                .id = "uptr",
                .function = Primitive_uptr
            },
            {
                .id = "float",
                .function = Primitive_float
            },
            {
                .id = "double",
                .function = Primitive_double
            },
            {
                .id = "string",
                .function = Primitive_string
            },
            {
                .id = "entity",
                .function = Primitive_entity
            }
        }
    },
    {
        .id = "Struct",
        .testcase_count = 4,
        .testcases = (bake_test_case[]){
            {
                .id = "struct",
                .function = Struct_struct
            },
            {
                .id = "nested_struct",
                .function = Struct_nested_struct
            },
            {
                .id = "struct_bool_i32",
                .function = Struct_struct_bool_i32
            },
            {
                .id = "struct_i32_bool",
                .function = Struct_struct_i32_bool
            }
        }
    },
    {
        .id = "Enum",
        .testcase_count = 3,
        .testcases = (bake_test_case[]){
            {
                .id = "enum",
                .function = Enum_enum
            },
            {
                .id = "enum_explicit_values",
                .function = Enum_enum_explicit_values
            },
            {
                .id = "enum_invalid_value",
                .function = Enum_enum_invalid_value
            }
        }
    },
    {
        .id = "Bitmask",
        .testcase_count = 4,
        .testcases = (bake_test_case[]){
            {
                .id = "bitmask_1",
                .function = Bitmask_bitmask_1
            },
            {
                .id = "bitmask_2",
                .function = Bitmask_bitmask_2
            },
            {
                .id = "bitmask_3",
                .function = Bitmask_bitmask_3
            },
            {
                .id = "bitmask_0_value",
                .function = Bitmask_bitmask_0_value
            }
        }
    },
    {
        .id = "Array",
        .testcase_count = 11,
        .testcases = (bake_test_case[]){
            {
                .id = "array_bool",
                .function = Array_array_bool
            },
            {
                .id = "array_int",
                .function = Array_array_int
            },
            {
                .id = "array_string",
                .function = Array_array_string
            },
            {
                .id = "array_entity",
                .function = Array_array_entity
            },
            {
                .id = "struct_array_int",
                .function = Array_struct_array_int
            },
            {
                .id = "array_struct",
                .function = Array_array_struct
            },
            {
                .id = "array_nested_struct",
                .function = Array_array_nested_struct
            },
            {
                .id = "array_array_int",
                .function = Array_array_array_int
            },
            {
                .id = "array_array_string",
                .function = Array_array_array_string
            },
            {
                .id = "array_array_struct",
                .function = Array_array_array_struct
            },
            {
                .id = "array_array_nested_struct",
                .function = Array_array_array_nested_struct
            }
        }
    },
    {
        .id = "Vector",
        .testcase_count = 13,
        .testcases = (bake_test_case[]){
            {
                .id = "vector_bool",
                .function = Vector_vector_bool
            },
            {
                .id = "vector_int",
                .function = Vector_vector_int
            },
            {
                .id = "vector_string",
                .function = Vector_vector_string
            },
            {
                .id = "vector_entity",
                .function = Vector_vector_entity
            },
            {
                .id = "vector_struct",
                .function = Vector_vector_struct
            },
            {
                .id = "vector_nested_struct",
                .function = Vector_vector_nested_struct
            },
            {
                .id = "vector_vector_int",
                .function = Vector_vector_vector_int
            },
            {
                .id = "vector_vector_struct",
                .function = Vector_vector_vector_struct
            },
            {
                .id = "vector_vector_nested_struct",
                .function = Vector_vector_vector_nested_struct
            },
            {
                .id = "vector_empty",
                .function = Vector_vector_empty
            },
            {
                .id = "vector_vector_empty",
                .function = Vector_vector_vector_empty
            },
            {
                .id = "vector_null",
                .function = Vector_vector_null
            },
            {
                .id = "vector_vector_null",
                .function = Vector_vector_vector_null
            }
        }
    },
    {
        .id = "Map",
        .testcase_count = 12,
        .testcases = (bake_test_case[]){
            {
                .id = "map_bool_bool",
                .function = Map_map_bool_bool
            },
            {
                .id = "map_int_bool",
                .function = Map_map_int_bool
            },
            {
                .id = "map_string_bool",
                .function = Map_map_string_bool
            },
            {
                .id = "map_enum_bool",
                .function = Map_map_enum_bool
            },
            {
                .id = "map_bitmask_bool",
                .function = Map_map_bitmask_bool
            },
            {
                .id = "map_int_int",
                .function = Map_map_int_int
            },
            {
                .id = "map_int_string",
                .function = Map_map_int_string
            },
            {
                .id = "map_int_struct",
                .function = Map_map_int_struct
            },
            {
                .id = "map_int_nested_struct",
                .function = Map_map_int_nested_struct
            },
            {
                .id = "map_int_array_int",
                .function = Map_map_int_array_int
            },
            {
                .id = "map_int_vector_int",
                .function = Map_map_int_vector_int
            },
            {
                .id = "map_int_map_int_bool",
                .function = Map_map_int_map_int_bool
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ut_init(argv[0]);
    return bake_test_run("test", argc, argv, suites, 7);
}
