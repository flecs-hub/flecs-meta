#include <include/meta.h>

ecs_vector_params_t EcsMetaEnumConstantArray = {.element_size = sizeof(EcsMetaEnumConstant)};
ecs_vector_params_t EcsMetaBitmaskConstantArray = {.element_size = sizeof(EcsMetaBitmaskConstant)};
ecs_vector_params_t EcsMetaMemberArray = {.element_size = sizeof(EcsMetaMember)};

static
void load_reflection(
    ecs_world_t *world,
    EcsComponentsMetaHandles handles) 
{
    EcsComponentsMeta_ImportHandles(handles);

    /* EcsMetaTypeKind */
    ECS_COMPONENT(world, EcsMetaTypeKind);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsPrimitive);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsBitmask);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsEnum);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsStruct);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsArray);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsVector);
    ECS_ENUM_CONSTANT(world, EcsMetaTypeKind, EcsMap);

    /* EcsMetaType */
    ECS_STRUCT_MEMBER(world, EcsMetaType, kind, EcsMetaTypeKind);

    /* EcsMetaPrimitiveKind */
    ECS_COMPONENT(world, EcsMetaPrimitiveKind);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsBool);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsChar);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsU8);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsU16);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsU32);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsU64);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsI8);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsI16);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsI32);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsI64);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsF32);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsF64);
    ECS_ENUM_CONSTANT(world, EcsMetaPrimitiveKind, EcsString);

    /* EcsMetaPrimitive */
    ECS_STRUCT_MEMBER(world, EcsMetaPrimitive, kind, EcsMetaPrimitiveKind);
}

void EcsComponentsMeta(
    ecs_world_t *world,
    int flags,
    void *handles_out)
{
    EcsComponentsMetaHandles *handles = handles_out;

    ECS_COMPONENT(world, EcsMetaType);
    ECS_COMPONENT(world, EcsMetaPrimitive);
    ECS_COMPONENT(world, EcsMetaEnum);
    ECS_COMPONENT(world, EcsMetaBitmask);
    ECS_COMPONENT(world, EcsMetaStruct);
    ECS_COMPONENT(world, EcsMetaArray);
    ECS_COMPONENT(world, EcsMetaVector);
    ECS_COMPONENT(world, EcsMetaMap);

    ECS_SET_COMPONENT(handles, EcsMetaType);
    ECS_SET_COMPONENT(handles, EcsMetaPrimitive);
    ECS_SET_COMPONENT(handles, EcsMetaEnum);
    ECS_SET_COMPONENT(handles, EcsMetaBitmask);
    ECS_SET_COMPONENT(handles, EcsMetaStruct);
    ECS_SET_COMPONENT(handles, EcsMetaArray);
    ECS_SET_COMPONENT(handles, EcsMetaVector);
    ECS_SET_COMPONENT(handles, EcsMetaMap);

    load_reflection(world, *handles);
}
