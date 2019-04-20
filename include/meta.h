#ifndef FLECS_COMPONENTS_META_H
#define FLECS_COMPONENTS_META_H

#ifdef __cplusplus
extern "C" {
#endif

/* This generated file contains includes for project dependencies */
#include "bake_config.h"
#include "api.h"

typedef const char* ecs_string_t;

extern ecs_vector_params_t EcsMetaEnumConstantParam;
extern ecs_vector_params_t EcsMetaBitmaskConstantParam;
extern ecs_vector_params_t EcsMetaMemberParam;

typedef enum EcsMetaTypeKind {
    EcsPrimitive,
    EcsBitmask,
    EcsEnum,
    EcsStruct,
    EcsArray,
    EcsVector,
    EcsMap
} EcsMetaTypeKind;

typedef struct EcsMetaType {
    EcsMetaTypeKind kind;
} EcsMetaType;

typedef enum EcsMetaPrimitiveKind {
    EcsBool,
    EcsChar,
    EcsByte,
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
    EcsWord,
    EcsString,
    EcsEntity
} EcsMetaPrimitiveKind;

typedef struct EcsMetaPrimitive {
    EcsMetaPrimitiveKind kind;
} EcsMetaPrimitive;

typedef struct EcsMetaEnumConstant {
    const char *name;
    int32_t value;
} EcsMetaEnumConstant;

typedef ecs_vector_t* EcsMetaEnumConstantVector;

typedef struct EcsMetaEnum {
    EcsMetaEnumConstantVector constants;
} EcsMetaEnum;

typedef struct EcsMetaBitmaskConstant {
    const char *name;
    uint64_t value;
} EcsMetaBitmaskConstant;

typedef ecs_vector_t* EcsMetaBitmaskConstantVector;

typedef struct EcsMetaBitmask {
    EcsMetaBitmaskConstantVector constants;
} EcsMetaBitmask;

typedef struct EcsMetaMember {
    const char *name;
    ecs_entity_t type;
    uint32_t offset;
} EcsMetaMember;

typedef ecs_vector_t* EcsMetaMemberVector;

typedef struct EcsMetaStruct {
    EcsMetaMemberVector members;
} EcsMetaStruct;

typedef struct EcsMetaArray {
    ecs_entity_t element_type;
    uint16_t size;
} EcsMetaArray;

typedef struct EcsMetaVector {
    ecs_entity_t element_type;
    uint32_t max_size;
} EcsMetaVector;

typedef struct EcsMetaMap {
    ecs_entity_t key_type;
    ecs_entity_t value_type;
    uint32_t max_size;
} EcsMetaMap;

#include "cache.h"

typedef struct EcsComponentsMetaHandles {
    /* Meta components */
    ECS_DECLARE_COMPONENT(EcsMetaType);
    ECS_DECLARE_COMPONENT(EcsMetaPrimitive);
    ECS_DECLARE_COMPONENT(EcsMetaEnum);
    ECS_DECLARE_COMPONENT(EcsMetaBitmask);
    ECS_DECLARE_COMPONENT(EcsMetaStruct);
    ECS_DECLARE_COMPONENT(EcsMetaArray);
    ECS_DECLARE_COMPONENT(EcsMetaVector);
    ECS_DECLARE_COMPONENT(EcsMetaMap);
    
    /* Meta framework components */
    ECS_DECLARE_ENTITY(EcsMetaDefined);
    ECS_DECLARE_COMPONENT(EcsMetaCache);

    /* Components for primitive types */
    ECS_DECLARE_COMPONENT(bool);
    ECS_DECLARE_COMPONENT(char);
    ECS_DECLARE_COMPONENT(uint8_t);
    ECS_DECLARE_COMPONENT(uint16_t);
    ECS_DECLARE_COMPONENT(uint32_t);
    ECS_DECLARE_COMPONENT(uint64_t);
    ECS_DECLARE_COMPONENT(int8_t);
    ECS_DECLARE_COMPONENT(int16_t);
    ECS_DECLARE_COMPONENT(int32_t);
    ECS_DECLARE_COMPONENT(int64_t);
    ECS_DECLARE_COMPONENT(intptr_t);
    ECS_DECLARE_COMPONENT(float);
    ECS_DECLARE_COMPONENT(double);
    ECS_DECLARE_COMPONENT(ecs_string_t);
    ECS_DECLARE_COMPONENT(ecs_entity_t);    
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
    ECS_IMPORT_COMPONENT(handles, EcsMetaMap);\
    ECS_IMPORT_ENTITY(handles, EcsMetaDefined);\
    ECS_IMPORT_COMPONENT(handles, EcsMetaCache);\
    ECS_IMPORT_COMPONENT(handles, bool);\
    ECS_IMPORT_COMPONENT(handles, char);\
    ECS_IMPORT_COMPONENT(handles, uint8_t);\
    ECS_IMPORT_COMPONENT(handles, uint16_t);\
    ECS_IMPORT_COMPONENT(handles, uint32_t);\
    ECS_IMPORT_COMPONENT(handles, uint64_t);\
    ECS_IMPORT_COMPONENT(handles, int8_t);\
    ECS_IMPORT_COMPONENT(handles, int16_t);\
    ECS_IMPORT_COMPONENT(handles, int32_t);\
    ECS_IMPORT_COMPONENT(handles, int64_t);\
    ECS_IMPORT_COMPONENT(handles, intptr_t);\
    ECS_IMPORT_COMPONENT(handles, float);\
    ECS_IMPORT_COMPONENT(handles, double);\
    ECS_IMPORT_COMPONENT(handles, ecs_string_t);\
    ECS_IMPORT_COMPONENT(handles, ecs_entity_t);

#ifdef __cplusplus
}
#endif

#endif
