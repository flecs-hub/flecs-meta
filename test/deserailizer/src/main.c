
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

static bake_test_suite suites[] = {
    {
        .id = "Struct",
        .testcase_count = 2,
        .testcases = (bake_test_case[]){
            {
                .id = "struct",
                .function = Struct_struct
            },
            {
                .id = "nested_struct",
                .function = Struct_nested_struct
            }
        }
    }
};

int main(int argc, char *argv[]) {
    ut_init(argv[0]);
    return bake_test_run("test", argc, argv, suites, 1);
}
