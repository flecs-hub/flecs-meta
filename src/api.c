
#include <include/meta.h>

void ecs_enum_push_constant(
    ecs_world_t *world,
    ecs_entity_t component,
    const char *name,
    int32_t value)
{
    ecs_entity_t ecs_to_entity(EcsMetaEnum) = ecs_lookup(world, "EcsMetaEnum");
    ecs_type_t ecs_to_type(EcsMetaEnum) = ecs_type_from_entity(world, ecs_to_entity(EcsMetaEnum));

    EcsMetaEnum *r_enum = ecs_get_ptr(world, component, EcsMetaEnum);
    if (!r_enum) {
        ecs_add(world, component, EcsMetaEnum);
        r_enum = ecs_get_ptr(world, component, EcsMetaEnum);
    }

    EcsMetaEnumConstant *c = ecs_vector_add(&r_enum->constants, &EcsMetaEnumConstantArray);
    c->name = name;
    c->value = value;
}

void ecs_struct_push_member(
    ecs_world_t *world,
    ecs_entity_t component,
    const char *name,
    ecs_entity_t type,
    uint32_t offset)
{
    ecs_entity_t ecs_to_entity(EcsMetaStruct) = ecs_lookup(world, "EcsMetaStruct");
    ecs_type_t ecs_to_type(EcsMetaStruct) = ecs_type_from_entity(world, ecs_to_entity(EcsMetaStruct));

    EcsMetaStruct *r_struct = ecs_get_ptr(world, component, EcsMetaStruct);
    if (!r_struct) {
        ecs_add(world, component, EcsMetaStruct);
        r_struct = ecs_get_ptr(world, component, EcsMetaStruct);
    }

    EcsMetaMember *c = ecs_vector_add(&r_struct->members, &EcsMetaMemberArray);
    c->name = name;
    c->type = type;
    c->offset = offset;
}
