#ifndef FLECS_COMPONENTS_META_H
#define FLECS_COMPONENTS_META_H

/* This generated file contains includes for project dependencies */
#include "flecs-components-meta/bake_config.h"

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////
//// Macro's that capture type definitions
////////////////////////////////////////////////////////////////////////////////

#define ECS_UNUSED __attribute__((unused))

#ifdef __cplusplus
#define ECS_ALIGNOF(T) alignof(T)
#else
#define ECS_ALIGNOF(T) ((size_t)&((struct { char c; T d; } *)0)->d)
#endif

#define ECS_ALIGN(size, alignment) (((((size) - 1) / (alignment)) + 1) * (alignment))

#define ECS_STRUCT(name, ...)\
typedef struct name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsType __##name##__ = {EcsStructType, sizeof(name), ECS_ALIGNOF(name), #__VA_ARGS__};\

#define ECS_ENUM(name, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsType __##name##__ = {EcsEnumType, sizeof(name), ECS_ALIGNOF(name), #__VA_ARGS__};

#define ECS_BITMASK(name, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static EcsType __##name##__ = {EcsBitmaskType, sizeof(name), ECS_ALIGNOF(name), #__VA_ARGS__};

// For the ecs_type_kind_t enumeration, which is defined before EcsType
#define ECS_ENUM_BOOTSTRAP(name, ...)\
typedef enum name __VA_ARGS__ name;\
ECS_UNUSED \
static const char * __##name##__ = #__VA_ARGS__;

#define ECS_NON_SERIALIZABLE

#define ecs_vector(T) ecs_vector_t*
#define ecs_map(K, T) ecs_map_t*

////////////////////////////////////////////////////////////////////////////////
//// Meta description
////////////////////////////////////////////////////////////////////////////////

/* Explicit string type */
typedef char* ecs_string_t;

ECS_ENUM_BOOTSTRAP( ecs_type_kind_t, {
    EcsPrimitiveType,
    EcsBitmaskType,
    EcsEnumType,
    EcsStructType,
    EcsArrayType,
    EcsVectorType,
    EcsMapType
});

ECS_STRUCT( EcsType, {
    ecs_type_kind_t kind;
    size_t size;
    int8_t alignment;
    const char *descriptor;
});

ECS_ENUM( ecs_primitive_kind_t, {
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
    EcsUPtr,
    EcsIPtr,
    EcsString,
    EcsEntity
});

ECS_STRUCT( EcsPrimitive, {
    ecs_primitive_kind_t kind;
});

ECS_STRUCT( EcsBitmask, {
    ecs_map(int32_t, ecs_string_t) constants;
});

ECS_STRUCT( EcsEnum, {
    ecs_map(int32_t, ecs_string_t) constants;
});

ECS_STRUCT( EcsMember, {
    const char *name;
    ecs_entity_t type;
});

ECS_STRUCT( EcsStruct, {
    ecs_vector(EcsMember) members;
});

ECS_STRUCT( EcsArray, {
    ecs_entity_t element_type;
    int32_t count;
});

ECS_STRUCT( EcsVector, {
    ecs_entity_t element_type;
});

ECS_STRUCT( EcsMap, {
    ecs_entity_t key_type;
    ecs_entity_t element_type;
});


////////////////////////////////////////////////////////////////////////////////
//// Type serializer
////////////////////////////////////////////////////////////////////////////////

ECS_ENUM( ecs_type_op_kind_t, {
    EcsOpHeader,
    EcsOpPrimitive,
    EcsOpEnum,
    EcsOpBitmask,
    EcsOpPush,
    EcsOpPop,
    EcsOpArray,
    EcsOpVector,
    EcsOpMap
});

typedef ecs_vector_t ecs_type_op_vector_t;
typedef ecs_vector_t ecs_constant_vector_t;

ECS_STRUCT( ecs_type_op_t, {
    ecs_type_op_kind_t kind;
    uint16_t size;
    uint16_t count;
    const char *name;

    uint32_t offset;
    uint8_t alignment;

ECS_NON_SERIALIZABLE

    union {
        ecs_vector(ecs_type_op_t) collection;
        ecs_primitive_kind_t primitive;
        ecs_map(int32_t, ecs_string_t) constant;
        struct {
            struct ecs_type_op_t *key_op;
            ecs_vector(ecs_type_op_t) element_ops;
        } map;
    } is;
});

ECS_STRUCT( EcsTypeSerializer, {
    ecs_vector(ecs_type_op_t) ops;
});


////////////////////////////////////////////////////////////////////////////////
//// Module implementation
////////////////////////////////////////////////////////////////////////////////

typedef struct FlecsComponentsMeta {
    ECS_DECLARE_COMPONENT(EcsPrimitive);
    ECS_DECLARE_COMPONENT(EcsEnum);
    ECS_DECLARE_COMPONENT(EcsBitmask);
    ECS_DECLARE_COMPONENT(EcsStruct);
    ECS_DECLARE_COMPONENT(EcsArray);
    ECS_DECLARE_COMPONENT(EcsVector);
    ECS_DECLARE_COMPONENT(EcsMap);
    ECS_DECLARE_COMPONENT(EcsType);
    ECS_DECLARE_COMPONENT(EcsTypeSerializer);
} FlecsComponentsMeta;

FLECS_COMPONENTS_META_EXPORT
void FlecsComponentsMetaImport(
    ecs_world_t *world,
    int flags);

#define FlecsComponentsMetaImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsPrimitive);\
    ECS_IMPORT_COMPONENT(handles, EcsEnum);\
    ECS_IMPORT_COMPONENT(handles, EcsBitmask);\
    ECS_IMPORT_COMPONENT(handles, EcsStruct);\
    ECS_IMPORT_COMPONENT(handles, EcsArray);\
    ECS_IMPORT_COMPONENT(handles, EcsVector);\
    ECS_IMPORT_COMPONENT(handles, EcsMap);\
    ECS_IMPORT_COMPONENT(handles, EcsType);\
    ECS_IMPORT_COMPONENT(handles, EcsTypeSerializer);


////////////////////////////////////////////////////////////////////////////////
//// C++ Module implementation
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {
namespace components {

class meta {
public:
    using Primitive = EcsPrimitive;
    using Enum = EcsEnum;
    using Bitmask = EcsBitmask;
    using Struct = EcsStruct;
    using Array = EcsArray;
    using Vector = EcsVector;
    using Type = EcsType;
    using TypeSerializer = EcsTypeSerializer;

    meta(flecs::world& world, int flags) {
        FlecsComponentsMetaImport(world.c_ptr(), flags);

        flecs::module<flecs::components::meta>(world, "flecs::components::meta");

        flecs::component<Primitive>(world, "EcsPrimitive");
        flecs::component<Enum>(world, "EcsEnum");
        flecs::component<Bitmask>(world, "EcsBitmask");
        flecs::component<Struct>(world, "EcsStruct");
        flecs::component<Array>(world, "EcsArray");
        flecs::component<Vector>(world, "EcsVector");
        flecs::component<Type>(world, "EcsType");
        flecs::component<TypeSerializer>(world, "EcsTypeSerializer");
    }
};

} 
}

#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
