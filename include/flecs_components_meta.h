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

#define ECS_STRUCT(name, ...)\
typedef struct name __VA_ARGS__ name;\
static EcsType __##name##__ = {EcsStructType, #__VA_ARGS__};

#define ECS_ENUM(name, ...)\
typedef enum name __VA_ARGS__ name;\
static EcsType __##name##__ = {EcsEnumType, #__VA_ARGS__};

// For the ecs_type_kind_t enumeration, which is defined before EcsType
#define ECS_ENUM_BOOTSTRAP(name, ...)\
typedef enum name __VA_ARGS__ name;\
static const char * __##name##__ = #__VA_ARGS__;

#define ECS_PROPERTY


////////////////////////////////////////////////////////////////////////////////
//// Meta description
////////////////////////////////////////////////////////////////////////////////

ECS_ENUM_BOOTSTRAP( ecs_type_kind_t, {
    EcsPrimitiveType,
    EcsBitmaskType,
    EcsEnumType,
    EcsStructType,
    EcsArrayType,
    EcsVectorType
});

ECS_STRUCT( EcsType, {
    ecs_type_kind_t kind;
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
    EcsWord,
    EcsString,
    EcsEntity
});

ECS_STRUCT( EcsPrimitive, {
    ecs_primitive_kind_t kind;
});

ECS_STRUCT( EcsBitmask, {
    ecs_vector_t *constants;
});

ECS_STRUCT( EcsEnum, {
    ecs_vector_t *constants;
});

ECS_STRUCT( EcsMember, {
    const char *name;
    ecs_entity_t type;
});

ECS_STRUCT( EcsStruct, {
    ecs_vector_t *members;
});

ECS_STRUCT( EcsArray, {
    ecs_entity_t element_type;
});

ECS_STRUCT( EcsVector, {
    ecs_entity_t element_type;
});


////////////////////////////////////////////////////////////////////////////////
//// Type serializer
////////////////////////////////////////////////////////////////////////////////

typedef enum ecs_type_op_kind_t {
    EcsOpNothing,
    EcsOpPrimitive,
    EcsOpEnum,
    EcsOpBitmask,
    EcsOpPush,
    EcsOpPop,
    EcsOpArray,
    EcsOpVector
} ecs_type_op_kind_t;

typedef ecs_vector_t ecs_type_op_vector_t;
typedef ecs_vector_t ecs_constant_vector_t;

typedef struct ecs_type_op_t {
    ecs_type_op_kind_t kind;
    uint32_t offset;
    uint16_t size;
    uint16_t count;
    const char *name;

    union {
        ecs_vector_t *collection;
        ecs_primitive_kind_t primitive;
        ecs_vector_t *constant;
    } is;
} ecs_type_op_t;

ECS_STRUCT( EcsTypeSerializer, {
    ecs_vector_t *ops;
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
    ECS_DECLARE_COMPONENT(EcsType);
    ECS_DECLARE_COMPONENT(EcsTypeSerializer);
} FlecsComponentsMeta;

FLECS_COMPONENTS_META_EXPORT
void FlecsComponentsMetaImport(
    ecs_world_t *world,
    int flags);

#define FlecsComponentsMetaImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, EcsType);


////////////////////////////////////////////////////////////////////////////////
//// C++ Module implementation
////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
#ifndef FLECS_NO_CPP

namespace flecs {
namespace components {

class meta {
public:
    meta(flecs::world& world, int flags) {
        FlecsComponentsMetaImport(world.c_ptr(), flags);

        flecs::module<flecs::components::meta>(world, "FlecsComponentsMeta");

        flecs::component<EcsPrimitive>(world, "EcsPrimitive");
        flecs::component<EcsEnum>(world, "EcsEnum");
        flecs::component<EcsBitmask>(world, "EcsBitmask");
        flecs::component<EcsStruct>(world, "EcsStruct");
        flecs::component<EcsArray>(world, "EcsArray");
        flecs::component<EcsVector>(world, "EcsVector");
        flecs::component<EcsType>(world, "EcsType");
        flecs::component<EcsTypeSerializer>(world, "EcsTypeSerializer");
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
