#ifndef FLECS_COMPONENTS_META_H
#define FLECS_COMPONENTS_META_H

/* This generated file contains includes for project dependencies */
#include "bake_config.h"

typedef enum EcsMetaTypeKind {
    EcsPrimitive,
    EcsBitmask,
    EcsEnum,
    EcsStruct,
    EcsArray,
    EcsVector,
    EcsBuffer,
    EcsMap
} EcsMetaTypeKind;

typedef struct EcsMetaType {
    EcsMetaTypeKind kind;
} EcsMetaType;

typedef enum EcsMetaPrimitiveKind {
    EcsBool,
    EcsChar,
    EcsU8,
    EcsU16,
    EcsU32,
    EcsU64,
    EcsI8,
    EcsI16,
    EcsI32,
    EcsI64,
    EcsF32,
    EcsF64,
    EcsString
} EcsMetaPrimitiveKind;

typedef struct EcsMetaPrimitive {
    EcsMetaPrimitiveKind kind;
} EcsMetaPrimitive;

typedef struct EcsMetaEnumConstant {
    const char *name;
    int32_t value;
} EcsMetaEnumConstant;

typedef struct EcsMetaEnum {
    const EcsMetaEnumConstant *constants;
    uint32_t constants_count;
} EcsMetaEnum;

typedef struct EcsMetaBitmaskConstant {
    const char *name;
    uint64_t value;
} EcsMetaBitmaskConstant;

typedef struct EcsMetaBitmask {
    const EcsMetaBitmaskConstant *constants;
    uint32_t constants_count;
} EcsMetaBitmask;

typedef struct EcsMetaMember {
    const char *name;
    ecs_entity_t type;
    uint32_t offset;
} EcsMetaMember;

typedef struct EcsMetaStruct {
    const EcsMetaMember *members;
    uint32_t members_count;
} EcsMetaStruct;

typedef struct EcsMetaArray {
    ecs_entity_t element_type;
    uint32_t size;
} EcsMetaArray;

typedef struct EcsMetaVector {
    ecs_entity_t element_type;
    uint32_t max_size;
} EcsMetaVector;

typedef struct EcsMetaBuffer {
    ecs_entity_t element_type;
} EcsMetaBuffer;

typedef struct EcsMetaMap {
    ecs_entity_t key_type;
    ecs_entity_t element_type;
    uint32_t max_size;
} EcsMetaMap;

typedef struct EcsComponentsMetaHandles {
    ECS_DECLARE_COMPONENT(EcsMetaType);
    ECS_DECLARE_COMPONENT(EcsMetaPrimitive);
    ECS_DECLARE_COMPONENT(EcsMetaEnum);
    ECS_DECLARE_COMPONENT(EcsMetaBitmask);
    ECS_DECLARE_COMPONENT(EcsMetaStruct);
    ECS_DECLARE_COMPONENT(EcsMetaArray);
    ECS_DECLARE_COMPONENT(EcsMetaVector);
    ECS_DECLARE_COMPONENT(EcsMetaBuffer);
    ECS_DECLARE_COMPONENT(EcsMetaMap);
} EcsComponentsMetaHandles;

void EcsComponentsMeta(
    ecs_world_t *world,
    int flags,
    void *handles_out);

#define EcsComponentsMeta_ImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsMetaType);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaPrimitive);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaEnum);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaBitmask);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaStruct);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaArray);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaVector);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaBuffer);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaMap);

#endif
