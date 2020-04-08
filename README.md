# flecs.components.meta
This is a reflection library for C/C++ that uses Flecs to store the meta definitions. The library uses a combination of macro and template wizardry to directly parse the C/C++ type defintions. No code generation or APIs to describe the types are needed. A simple example:

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
    std::cout << flecs::to_string(p) << std::endl;
}
```