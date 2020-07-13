#include <vector.h>

ECS_STRUCT(Vert2D, {
    float x;
    float y;
});

ECS_STRUCT(Mesh, {
    ecs_vector(Vert2D) vertices;
});

int main(int argc, char *argv[]) {
    ecs_world_t *world = ecs_init_w_args(argc, argv);

    /* Import meta module */
    ECS_IMPORT(world, FlecsMeta);

    /* Insert the meta definitions for Vert2D and Mesh. Make sure to define Vert2D
     * before Mesh as Mesh uses Vert2D */
    ECS_META(world, Vert2D);
    ECS_META(world, Mesh);

    /* Create an instance of the Mesh type */
    Mesh m = { NULL };
    Vert2D *v = ecs_vector_add(&m.vertices, Vert2D);
    v->x = 10;
    v->y = 20;

    v = ecs_vector_add(&m.vertices, Vert2D);
    v->x = 30;
    v->y = 40;

    v = ecs_vector_add(&m.vertices, Vert2D);
    v->x = 50;
    v->y = 60;        

    /* Pretty print the value */
    char *str = ecs_ptr_to_str(world, ecs_entity(Mesh), &m);
    printf("%s\n", str);
    free(str);

    return ecs_fini(world);
}
