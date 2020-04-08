# flecs.components.meta
**NOTE: This library currently only works with the bleeding_edge branch of Flecs!**

This is a reflection library for C/C++ that uses Flecs to store the meta definitions. The library uses a combination of macro and template wizardry to directly parse the C/C++ type defintions. No code generation or APIs to describe the types are needed. 

The library does not require compile time knowledge of a type. This lets applications define types at runtime, and enables for the creation of tools that are type unaware.

Type definitions are converted to a virtual-machine like opcode format which allows applications to write extremely fast serializers.

The library supports:
 - Lots of primitive types
 - Enumerations
 - Bitmasks
 - Structs
 - Arrays
 - Vectors
 - Maps

A simple example:

```c++
ECS_STRUCT(Position, {
    float x;
    float y;
});

void main() {
    // The world that will store the reflection data
    flecs::world world();

    // Import the reflection module
    flecs::import<flecs::components::meta>(world);

    // Inject reflection data for position
    flecs::meta<Position>(world);

    // Enjoy!
    Position p{10, 20};
    std::cout << flecs::pretty_print(world, p) << std::endl;
}
```

## Examples

### Primitive types
Only fixed-size primitive types are currently supported. For example, use 
`int32_t` instead of `int`, and `uint32_t` instead of `unsigned int`.

```c++
ECS_STRUCT(Primitives, {
    const char *name;
    int32_t value;
    bool is_active;
});
```

Use it like this:

```c++
flecs::meta<Primitives>(world);

Primitives = {"Foobar", 10, true};

std::cout << flecs::pretty_print(world, p) << std::endl;
```

Output:

```
{name = "Foobar", value = 10, is_active = true}
```

### Enumerations
Enumerations can be used in a way that is similar to structs: 

```c++
ECS_ENUM(Color, {
    Red,
    Green,
    Blue
});

ECS_STRUCT(Point, {
    float x;
    float y;
    Color color;
})
```

Use it like this:

```c++
// Make sure to define Color before Point, as Point uses Color
flecs::meta<Color>(world);
flecs::meta<Point>(world);

Point p = {10, 20, Green};

std::cout << flecs::pretty_print(world, p) << std::endl;
```

Output:

```
{x = 10.0000, y = 20.0000, color = Green}
```

### Bitmasks
Bitmasks are just like enumerations, except that a serializer can serialize multiple constants, depending on which flags are set.

```c++
ECS_BITMASK(Toppings, {
    Bacon,
    Lettuce,
    Tomato,
    Egg,
    Ham
});

ECS_STRUCT(Sandwich, {
    const char *name;
    Toppings toppings;
})
```

Use it like this:

```c++
flecs::meta<Toppings>(world);
flecs::meta<Sandwich>(world);

Sandwich s = {"BLT", Bacon | Lettuce | Tomato};

std::cout << flecs::pretty_print(world, p) << std::endl;
```

Output:

```
{name = "BLT", toppings = Bacon | Lettuce | Tomato}
```

### Arrays
The standard C array notation is correctly recognized:

```c++
ECS_STRUCT(Vec3D, {
    float coord[2];
});
```

Use it like this:

```c++
flecs::meta<Vec2D>(world);

Vec2D v = {10, 20};

std::cout << flecs::pretty_print(world, v) << std::endl;
```

Output:

```
{coord = [10.0000, 20.0000]}
```

### Vectors
Vectors are supported through the built-in Flecs vector type. This is a C data structure for which eventually a C++ wrapper will become availble.

```c++
ECS_STRUCT(Vec2D, {
    float x;
    float y;
});

ECS_STRUCT(Mesh, {
    ecs_vector(Vec2D) vertices;
});
```

Use it like this:

```c++
flecs::meta<Vec2D>(world);
flecs::meta<Mesh>(world);

ecs_vector_T *vertices = ecs_vector_new(Vec2D, 2);
Vec2D *v = ecs_vector_add(&vertices, Vec2D);
v->x = 10;
v->y = 20;

v = ecs_vector_add(&vertices, Vec2D);
v->x = 30;
v->y = 40;

Mesh m = {vertices};

std::cout << flecs::pretty_print(world, v) << std::endl;
```

Output:

```
{vertices = [{x = 10.0000, y = 20.0000}, {x = 30.0000, y = 40.0000}]}
```

### Maps
Maps use the built-in Flecs hashmap implementation. This is a C data structure for which eventually a C++ wrapper will become available. The map type can be a bit harder to populate, as its key is hardcoded as a 64bit integer. It is possible to use strings as map keys, but they will have to be casted to the map keytype.

```c++
ECS_STRUCT(Menu, {
    ecs_map(ecs_string_t, int32_t) items;
});
```

Use it like this:

```c++
flecs::meta<Menu>(world);

ecs_map_t *map = ecs_map_new(int32_t, 2);
int cost = 3;
ecs_map_set(m, (intptr_t)"BLT", &cost);
cost = 2;
ecs_map_set(m, (intptr_t)"Bacon and cheese", &cost);

Menu m = {map};

std::cout << flecs::pretty_print(world, m) << std::endl;
```

Output:

```
{items = {"BLT" = 3, "Bacon and cheese" = 2}}
```

