
/* A friendly warning from bake.test
 * ----------------------------------------------------------------------------
 * This file is generated. To add/remove testcases modify the 'project.json' of
 * the test project. ANY CHANGE TO THIS FILE IS LOST AFTER (RE)BUILDING!
 * ----------------------------------------------------------------------------
 */

#include <test.h>

// Testsuite 'Struct'
void Struct_struct(void);
void Struct_nested_struct(void);
void Struct_struct_by_name(void);
void Struct_nested_struct_by_name(void);
void Struct_struct_w_array(void);
void Struct_struct_w_array_by_name(void);
void Struct_struct_w_array_nested_struct(void);
void Struct_struct_w_array_nested_struct_by_name(void);

static bake_test_suite suites[] = {
    {
        .id = "Struct",
        .testcase_count = 8,
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
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ut_init(argv[0]);
    return bake_test_run("test", argc, argv, suites, 1);
}
