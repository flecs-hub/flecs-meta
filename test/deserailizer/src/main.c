
/* A friendly warning from bake.test
 * ----------------------------------------------------------------------------
 * This file is generated. To add/remove testcases modify the 'project.json' of
 * the test project. ANY CHANGE TO THIS FILE IS LOST AFTER (RE)BUILDING!
 * ----------------------------------------------------------------------------
 */

#include <test.h>

// Testsuite 'Struct'
void Struct_struct(void);
void Struct_struct_primitives(void);
void Struct_struct_reassign_string(void);
void Struct_struct_reassign_null(void);
void Struct_nested_struct(void);
void Struct_struct_by_name(void);
void Struct_nested_struct_by_name(void);
void Struct_struct_w_array(void);
void Struct_struct_w_array_by_name(void);
void Struct_struct_w_array_nested_struct(void);
void Struct_struct_w_array_nested_struct_by_name(void);
void Struct_struct_w_vector(void);
void Struct_struct_w_vector_by_name(void);
void Struct_struct_w_vector_nested_struct(void);
void Struct_struct_w_vector_nested_struct_by_name(void);
void Struct_struct_reassign_vector(void);
void Struct_struct_reassign_smaller_vector(void);
void Struct_struct_reassign_larger_vector(void);
void Struct_struct_reassign_vector_null(void);

static bake_test_suite suites[] = {
    {
        .id = "Struct",
        .testcase_count = 19,
        .testcases = (bake_test_case[]){
            {
                .id = "struct",
                .function = Struct_struct
            },
            {
                .id = "struct_primitives",
                .function = Struct_struct_primitives
            },
            {
                .id = "struct_reassign_string",
                .function = Struct_struct_reassign_string
            },
            {
                .id = "struct_reassign_null",
                .function = Struct_struct_reassign_null
            },
            {
                .id = "nested_struct",
                .function = Struct_nested_struct
            },
            {
                .id = "struct_by_name",
                .function = Struct_struct_by_name
            },
            {
                .id = "nested_struct_by_name",
                .function = Struct_nested_struct_by_name
            },
            {
                .id = "struct_w_array",
                .function = Struct_struct_w_array
            },
            {
                .id = "struct_w_array_by_name",
                .function = Struct_struct_w_array_by_name
            },
            {
                .id = "struct_w_array_nested_struct",
                .function = Struct_struct_w_array_nested_struct
            },
            {
                .id = "struct_w_array_nested_struct_by_name",
                .function = Struct_struct_w_array_nested_struct_by_name
            },
            {
                .id = "struct_w_vector",
                .function = Struct_struct_w_vector
            },
            {
                .id = "struct_w_vector_by_name",
                .function = Struct_struct_w_vector_by_name
            },
            {
                .id = "struct_w_vector_nested_struct",
                .function = Struct_struct_w_vector_nested_struct
            },
            {
                .id = "struct_w_vector_nested_struct_by_name",
                .function = Struct_struct_w_vector_nested_struct_by_name
            },
            {
                .id = "struct_reassign_vector",
                .function = Struct_struct_reassign_vector
            },
            {
                .id = "struct_reassign_smaller_vector",
                .function = Struct_struct_reassign_smaller_vector
            },
            {
                .id = "struct_reassign_larger_vector",
                .function = Struct_struct_reassign_larger_vector
            },
            {
                .id = "struct_reassign_vector_null",
                .function = Struct_struct_reassign_vector_null
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ut_init(argv[0]);
    return bake_test_run("test", argc, argv, suites, 1);
}
