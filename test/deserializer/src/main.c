
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

bake_test_case Struct_testcases[] = {
    {
        "struct",
        Struct_struct
    },
    {
        "struct_primitives",
        Struct_struct_primitives
    },
    {
        "struct_reassign_string",
        Struct_struct_reassign_string
    },
    {
        "struct_reassign_null",
        Struct_struct_reassign_null
    },
    {
        "nested_struct",
        Struct_nested_struct
    },
    {
        "struct_by_name",
        Struct_struct_by_name
    },
    {
        "nested_struct_by_name",
        Struct_nested_struct_by_name
    },
    {
        "struct_w_array",
        Struct_struct_w_array
    },
    {
        "struct_w_array_by_name",
        Struct_struct_w_array_by_name
    },
    {
        "struct_w_array_nested_struct",
        Struct_struct_w_array_nested_struct
    },
    {
        "struct_w_array_nested_struct_by_name",
        Struct_struct_w_array_nested_struct_by_name
    },
    {
        "struct_w_vector",
        Struct_struct_w_vector
    },
    {
        "struct_w_vector_by_name",
        Struct_struct_w_vector_by_name
    },
    {
        "struct_w_vector_nested_struct",
        Struct_struct_w_vector_nested_struct
    },
    {
        "struct_w_vector_nested_struct_by_name",
        Struct_struct_w_vector_nested_struct_by_name
    },
    {
        "struct_reassign_vector",
        Struct_struct_reassign_vector
    },
    {
        "struct_reassign_smaller_vector",
        Struct_struct_reassign_smaller_vector
    },
    {
        "struct_reassign_larger_vector",
        Struct_struct_reassign_larger_vector
    },
    {
        "struct_reassign_vector_null",
        Struct_struct_reassign_vector_null
    }
};

static bake_test_suite suites[] = {
    {
        "Struct",
        NULL,
        NULL,
        19,
        Struct_testcases
    }
};

int main(int argc, char *argv[]) {
    ut_init(argv[0]);
    return bake_test_run("test", argc, argv, suites, 1);
}
