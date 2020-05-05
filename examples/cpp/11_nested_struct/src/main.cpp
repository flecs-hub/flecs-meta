#include <nested_struct.h>

ECS_STRUCT(Point, {
    int32_t x;
    int32_t y;
});

ECS_STRUCT(Line, {
    Point start;
    Point stop;
});

int main(int argc, char *argv[]) {
    flecs::world world(argc, argv);

    /* Import meta module */
    flecs::import<flecs::components::meta>(world);

    /* Insert the meta definitions for Position. This will also register the
     * Position type as a component */
    flecs::meta<Point>(world);
    flecs::meta<Line>(world);

    /* Create an instance of the Position type */
    Line l = {{10, 20}, {30, 40}};

    /* Pretty print the value */
    std::cout << flecs::pretty_print(world, l) << std::endl;
}
