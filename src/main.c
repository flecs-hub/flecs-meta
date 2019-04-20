#include <include/meta.h>

ecs_vector_params_t EcsMetaEnumConstantParam = {.element_size = sizeof(EcsMetaEnumConstant)};
ecs_vector_params_t EcsMetaBitmaskConstantParam = {.element_size = sizeof(EcsMetaBitmaskConstant)};
ecs_vector_params_t EcsMetaMemberParam = {.element_size = sizeof(EcsMetaMember)};

static
void load_reflection(
    ecs_world_t *world,
    EcsComponentsMetaHandles handles) 
{
    EcsComponentsMeta_ImportHandles(handles);

    /* Primitive types */
    ECS_PRIMITIVE(world, bool, EcsBool);
    ECS_PRIMITIVE(world, char, EcsChar);
    ECS_PRIMITIVE(world, uint8_t, EcsU8);
    ECS_PRIMITIVE(world, uint16_t, EcsU16);
    ECS_PRIMITIVE(world, uint32_t, EcsU32);
    ECS_PRIMITIVE(world, uint64_t, EcsU64);
    ECS_PRIMITIVE(world, int8_t, EcsI8);
    ECS_PRIMITIVE(world, int16_t, EcsI16);
    ECS_PRIMITIVE(world, int32_t, EcsI32);
    ECS_PRIMITIVE(world, int64_t, EcsI64);
    ECS_PRIMITIVE(world, intptr_t, EcsWord);
    ECS_PRIMITIVE(world, float, EcsF32);
    ECS_PRIMITIVE(world, double, EcsF64);
    ECS_PRIMITIVE(world, ecs_string_t, EcsString);
    ECS_PRIMITIVE(world, ecs_entity_t, EcsEntity);

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

    
    /* -- EcsMetaEnum -- */

    /* EcsMetaEnumConstant */
    ECS_COMPONENT(world, EcsMetaEnumConstant);
    ECS_STRUCT_MEMBER(world, EcsMetaEnumConstant, name, ecs_string_t);
    ECS_STRUCT_MEMBER(world, EcsMetaEnumConstant, value, int32_t);

    /* vector<EcsMetaEnumConstant> */
    ECS_COMPONENT(world, EcsMetaEnumConstantVector);
    ECS_VECTOR(world, EcsMetaEnumConstantVector, EcsMetaEnumConstant, 0);

    /* EcsMetaEnum */
    ECS_STRUCT_MEMBER(world, EcsMetaEnum, constants, EcsMetaEnumConstantVector);


    /* -- EcsMetaBitmask -- */

    /* EcsMetaBitmaskConstant */
    ECS_COMPONENT(world, EcsMetaBitmaskConstant);
    ECS_STRUCT_MEMBER(world, EcsMetaBitmaskConstant, name, ecs_string_t);
    ECS_STRUCT_MEMBER(world, EcsMetaBitmaskConstant, value, uint64_t);

    /* vector<EcsMetaBitmaskConstant> */
    ECS_COMPONENT(world, EcsMetaBitmaskConstantVector);
    ECS_VECTOR(world, EcsMetaBitmaskConstantVector, EcsMetaBitmaskConstant, 0);

    /* EcsMetaBitmask */
    ECS_STRUCT_MEMBER(world, EcsMetaBitmask, constants, EcsMetaBitmaskConstantVector);


    /* -- EcsMetaStruct -- */

    /* EcsMetaMember */
    ECS_COMPONENT(world, EcsMetaMember);
    ECS_STRUCT_MEMBER(world, EcsMetaMember, name, ecs_string_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMember, type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMember, offset, uint32_t);

    /* vector<EcsMetaMember> */
    ECS_COMPONENT(world, EcsMetaMemberVector);
    ECS_VECTOR(world, EcsMetaMemberVector, EcsMetaMember, 0);

    /* EcsStruct */
    ECS_STRUCT_MEMBER(world, EcsMetaStruct, members, EcsMetaMemberVector);


    /* -- Collections -- */
    
    /* EcsMetaArray */
    ECS_STRUCT_MEMBER(world, EcsMetaArray, element_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaArray, size, uint32_t);

    /* EcsMetaVector */
    ECS_STRUCT_MEMBER(world, EcsMetaVector, element_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaVector, max_size, uint32_t);

    /* EcsMetaMap */
    ECS_STRUCT_MEMBER(world, EcsMetaMap, key_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMap, value_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMap, max_size, uint32_t);
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
    ECS_TAG(world, EcsMetaDefined);

    ECS_COMPONENT(world, bool);
    ECS_COMPONENT(world, char);
    ECS_COMPONENT(world, uint8_t);
    ECS_COMPONENT(world, uint16_t);
    ECS_COMPONENT(world, uint32_t);
    ECS_COMPONENT(world, uint64_t);
    ECS_COMPONENT(world, int8_t);
    ECS_COMPONENT(world, int16_t);
    ECS_COMPONENT(world, int32_t);
    ECS_COMPONENT(world, int64_t);
    ECS_COMPONENT(world, intptr_t);
    ECS_COMPONENT(world, float);
    ECS_COMPONENT(world, double);
    ECS_COMPONENT(world, ecs_string_t);
    ECS_COMPONENT(world, ecs_entity_t);    

    ECS_SET_COMPONENT(handles, EcsMetaType);
    ECS_SET_COMPONENT(handles, EcsMetaPrimitive);
    ECS_SET_COMPONENT(handles, EcsMetaEnum);
    ECS_SET_COMPONENT(handles, EcsMetaBitmask);
    ECS_SET_COMPONENT(handles, EcsMetaStruct);
    ECS_SET_COMPONENT(handles, EcsMetaArray);
    ECS_SET_COMPONENT(handles, EcsMetaVector);
    ECS_SET_COMPONENT(handles, EcsMetaMap);
    ECS_SET_ENTITY(handles, EcsMetaDefined);

    ECS_SET_COMPONENT(handles, bool);
    ECS_SET_COMPONENT(handles, char);
    ECS_SET_COMPONENT(handles, uint8_t);
    ECS_SET_COMPONENT(handles, uint16_t);
    ECS_SET_COMPONENT(handles, uint32_t);
    ECS_SET_COMPONENT(handles, uint64_t);
    ECS_SET_COMPONENT(handles, int8_t);
    ECS_SET_COMPONENT(handles, int16_t);
    ECS_SET_COMPONENT(handles, int32_t);
    ECS_SET_COMPONENT(handles, int64_t);
    ECS_SET_COMPONENT(handles, intptr_t);
    ECS_SET_COMPONENT(handles, float);
    ECS_SET_COMPONENT(handles, double);
    ECS_SET_COMPONENT(handles, ecs_string_t);
    ECS_SET_COMPONENT(handles, ecs_entity_t);    

    load_reflection(world, *handles);
}
