#include <flecs_components_meta.h>
#include "systems.h"

ecs_vector_params_t EcsMetaEnumConstantParam = {.element_size = sizeof(EcsMetaEnumConstant)};
ecs_vector_params_t EcsMetaBitmaskConstantParam = {.element_size = sizeof(EcsMetaBitmaskConstant)};
ecs_vector_params_t EcsMetaMemberParam = {.element_size = sizeof(EcsMetaMember)};

static
void load_reflection(
    ecs_world_t *world,
    FlecsComponentsMeta handles) 
{
    FlecsComponentsMetaImportHandles(handles);

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
    ECS_DEFINE(world, EcsMetaTypeKind);

    /* EcsMetaType */
    ECS_STRUCT_MEMBER(world, EcsMetaType, kind, EcsMetaTypeKind);
    ECS_DEFINE(world, EcsMetaType);

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
    ECS_DEFINE(world, EcsMetaPrimitive);

    
    /* -- EcsMetaEnum -- */

    /* EcsMetaEnumConstant */
    ECS_COMPONENT(world, EcsMetaEnumConstant);
    ECS_STRUCT_MEMBER(world, EcsMetaEnumConstant, name, ecs_string_t);
    ECS_STRUCT_MEMBER(world, EcsMetaEnumConstant, value, int32_t);
    ECS_DEFINE(world, EcsMetaEnumConstant);

    /* vector<EcsMetaEnumConstant> */
    ECS_COMPONENT(world, EcsMetaEnumConstantVector);
    ECS_VECTOR(world, EcsMetaEnumConstantVector, EcsMetaEnumConstant, 0);

    /* EcsMetaEnum */
    ECS_STRUCT_MEMBER(world, EcsMetaEnum, constants, EcsMetaEnumConstantVector);
    ECS_DEFINE(world, EcsMetaEnum);


    /* -- EcsMetaBitmask -- */

    /* EcsMetaBitmaskConstant */
    ECS_COMPONENT(world, EcsMetaBitmaskConstant);
    ECS_STRUCT_MEMBER(world, EcsMetaBitmaskConstant, name, ecs_string_t);
    ECS_STRUCT_MEMBER(world, EcsMetaBitmaskConstant, value, uint64_t);
    ECS_DEFINE(world, EcsMetaBitmaskConstant);

    /* vector<EcsMetaBitmaskConstant> */
    ECS_COMPONENT(world, EcsMetaBitmaskConstantVector);
    ECS_VECTOR(world, EcsMetaBitmaskConstantVector, EcsMetaBitmaskConstant, 0);

    /* EcsMetaBitmask */
    ECS_STRUCT_MEMBER(world, EcsMetaBitmask, constants, EcsMetaBitmaskConstantVector);
    ECS_DEFINE(world, EcsMetaBitmask);


    /* -- EcsMetaStruct -- */

    /* EcsMetaMember */
    ECS_COMPONENT(world, EcsMetaMember);
    ECS_STRUCT_MEMBER(world, EcsMetaMember, name, ecs_string_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMember, type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMember, offset, uint32_t);
    ECS_DEFINE(world, EcsMetaMember);

    /* vector<EcsMetaMember> */
    ECS_COMPONENT(world, EcsMetaMemberVector);
    ECS_VECTOR(world, EcsMetaMemberVector, EcsMetaMember, 0);

    /* EcsStruct */
    ECS_STRUCT_MEMBER(world, EcsMetaStruct, members, EcsMetaMemberVector);
    ECS_DEFINE(world, EcsMetaStruct);


    /* -- Collections -- */
    
    /* EcsMetaArray */
    ECS_STRUCT_MEMBER(world, EcsMetaArray, element_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaArray, size, uint32_t);
    ECS_DEFINE(world, EcsMetaArray);

    /* EcsMetaVector */
    ECS_STRUCT_MEMBER(world, EcsMetaVector, element_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaVector, max_size, uint32_t);
    ECS_DEFINE(world, EcsMetaVector);

    /* EcsMetaMap */
    ECS_STRUCT_MEMBER(world, EcsMetaMap, key_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMap, value_type, ecs_entity_t);
    ECS_STRUCT_MEMBER(world, EcsMetaMap, max_size, uint32_t);
    ECS_DEFINE(world, EcsMetaMap);
}

static
void init_type(ecs_rows_t *rows, ecs_type_t ecs_type(EcsMetaType), EcsMetaTypeKind kind) {
    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_set(rows->world, rows->entities[i], EcsMetaType, {
            .kind = kind
        });
    }
}

static
void InitPrimitive(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsPrimitive);
}

static
void InitEnum(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsEnum);
}

static
void InitBitmask(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsBitmask);
}

static
void InitStruct(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsStruct);
}

static
void InitArray(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsArray);
}

static
void InitVector(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsVector);
}

static
void InitMap(ecs_rows_t *rows) {
    ECS_COLUMN_COMPONENT(rows, EcsMetaType, 2);
    init_type(rows, ecs_type(EcsMetaType), EcsMap);
}

static
void DeinitEnum(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsMetaEnum, meta_enum, 1);
    
    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_vector_free(meta_enum[i].constants);
    }
}

static
void DeinitBitmask(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsMetaBitmask, meta_bitmask, 1);
    
    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_vector_free(meta_bitmask[i].constants);
    }
}

static
void DeinitStruct(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsMetaStruct, meta_struct, 1);

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_vector_free(meta_struct[i].members);
    }
}

void FlecsComponentsMetaImport(
    ecs_world_t *world,
    int flags)
{
    ECS_MODULE(world, FlecsComponentsMeta);

    ECS_COMPONENT(world, EcsMetaType);
    ECS_COMPONENT(world, EcsMetaPrimitive);
    ECS_COMPONENT(world, EcsMetaEnum);
    ECS_COMPONENT(world, EcsMetaBitmask);
    ECS_COMPONENT(world, EcsMetaStruct);
    ECS_COMPONENT(world, EcsMetaArray);
    ECS_COMPONENT(world, EcsMetaVector);
    ECS_COMPONENT(world, EcsMetaMap);
    ECS_TAG(world, EcsMetaDefined);
    ECS_COMPONENT(world, EcsMetaCache);

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
    ECS_SET_COMPONENT(handles, EcsMetaCache);

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

    ECS_SYSTEM(world, InitPrimitive, EcsOnAdd, EcsMetaPrimitive, ID.EcsMetaType);
    ECS_SYSTEM(world, InitEnum, EcsOnAdd, EcsMetaEnum, ID.EcsMetaType);
    ECS_SYSTEM(world, InitBitmask, EcsOnAdd, EcsMetaBitmask, ID.EcsMetaType);
    ECS_SYSTEM(world, InitStruct, EcsOnAdd, EcsMetaStruct, ID.EcsMetaType);
    ECS_SYSTEM(world, InitArray, EcsOnAdd, EcsMetaArray, ID.EcsMetaType);
    ECS_SYSTEM(world, InitVector, EcsOnAdd, EcsMetaVector, ID.EcsMetaType);
    ECS_SYSTEM(world, InitMap, EcsOnAdd, EcsMetaMap, ID.EcsMetaType);

    ECS_SYSTEM(world, InitCache, EcsOnAdd, EcsMetaDefined, $FlecsComponentsMeta);
    ECS_SYSTEM(world, DeinitCache, EcsOnRemove, EcsMetaCache);

    ECS_SYSTEM(world, DeinitEnum, EcsOnRemove, EcsMetaEnum);
    ECS_SYSTEM(world, DeinitBitmask, EcsOnRemove, EcsMetaBitmask);
    ECS_SYSTEM(world, DeinitStruct, EcsOnRemove, EcsMetaStruct);

    load_reflection(world, *handles);
}
