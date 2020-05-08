#include <flecs_components_meta.h>
#include "parser.h"
#include "serializer.h"
#include "type.h"

static
void ecs_set_primitive(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_entity_t ecs_entity(EcsPrimitive) = ecs_lookup(world, "EcsPrimitive");
    ecs_assert(ecs_entity(EcsPrimitive) != 0, ECS_INTERNAL_ERROR, NULL);

    const char *descr = type->descriptor;

    if (!strcmp(descr, "bool")) {
        ecs_set(world, e, EcsPrimitive, {EcsBool});
    } else
    if (!strcmp(descr, "char")) {
        ecs_set(world, e, EcsPrimitive, {EcsChar});
    } else    
    if (!strcmp(descr, "u8")) {
        ecs_set(world, e, EcsPrimitive, {EcsU8});
    } else
    if (!strcmp(descr, "u8")) {
        ecs_set(world, e, EcsPrimitive, {EcsU8});
    } else
    if (!strcmp(descr, "u16")) {
        ecs_set(world, e, EcsPrimitive, {EcsU16});
    } else
    if (!strcmp(descr, "u32")) {
        ecs_set(world, e, EcsPrimitive, {EcsU32});
    } else
    if (!strcmp(descr, "u64")) {
        ecs_set(world, e, EcsPrimitive, {EcsU64});
    } else
    if (!strcmp(descr, "i8")) {
        ecs_set(world, e, EcsPrimitive, {EcsI8});
    } else
    if (!strcmp(descr, "i16")) {
        ecs_set(world, e, EcsPrimitive, {EcsI16});
    } else
    if (!strcmp(descr, "i32")) {
        ecs_set(world, e, EcsPrimitive, {EcsI32});
    } else
    if (!strcmp(descr, "f32")) {
        ecs_set(world, e, EcsPrimitive, {EcsF32});
    } else
    if (!strcmp(descr, "f64")) {
        ecs_set(world, e, EcsPrimitive, {EcsF64});
    } else
    if (!strcmp(descr, "iptr")) {
        ecs_set(world, e, EcsPrimitive, {EcsIPtr});
    } else    
    if (!strcmp(descr, "uptr")) {
        ecs_set(world, e, EcsPrimitive, {EcsUPtr});
    } else
    if (!strcmp(descr, "string")) {
        ecs_set(world, e, EcsPrimitive, {EcsString});
    } else
    if (!strcmp(descr, "entity")) {
        ecs_set(world, e, EcsPrimitive, {EcsEntity});
    }
}

static
void ecs_set_constants(
    ecs_world_t *world, 
    ecs_entity_t e, 
    ecs_entity_t comp,
    bool is_bitmask,
    EcsType *type) 
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_id(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_map_t *constants = ecs_map_new(char*, 1);
    ecs_meta_constant_t token;
    int32_t last_value = 0;

    while ((ptr = ecs_meta_parse_constant(ptr, &token, &ctx))) {
        if (token.is_value_set) {
            last_value = token.value;
        } else if (is_bitmask) {
            ecs_meta_error(&ctx, ptr, 
                "bitmask requires explicit value assignment");
        }

        char *name = ecs_os_strdup(token.name);
        ecs_map_set(constants, last_value, &name);

        last_value ++;
    }

    _ecs_set_ptr(world, e, comp, sizeof(EcsEnum), &(EcsEnum){
        .constants = constants
    });
}

static
void ecs_set_bitmask(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_entity_t comp = ecs_lookup(world, "EcsBitmask");
    ecs_assert(comp != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_set_constants(world, e, comp, true, type);
}

static
void ecs_set_enum(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_entity_t comp = ecs_lookup(world, "EcsEnum");
    ecs_assert(comp != 0, ECS_INTERNAL_ERROR, NULL);    
    ecs_set_constants(world, e, comp, false, type);
}

static
void ecs_set_struct(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_id(world, e);
    bool is_partial = false;

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_vector_t *members = NULL;
    ecs_meta_member_t token;

    while ((ptr = ecs_meta_parse_member(ptr, &token, &ctx))) {
        EcsMember *m = ecs_vector_add(&members, EcsMember);
        m->name = ecs_os_strdup(token.name);
        m->type = ecs_meta_lookup(world, &token.type, ptr, token.count, &ctx);
        ecs_assert(type != 0, ECS_INTERNAL_ERROR, NULL);
    }

    is_partial = token.is_partial;

    ecs_entity_t ecs_entity(EcsStruct) = ecs_lookup(world, "EcsStruct");
    ecs_assert(ecs_entity(EcsStruct) != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_set(world, e, EcsStruct, {members, is_partial});
}

static
void ecs_set_array(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_id(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_array(world, e, type->descriptor, &ctx);
}

static
void ecs_set_vector(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_id(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_vector(world, e, type->descriptor, &ctx);
}

static
void ecs_set_map(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_id(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_map(world, e, type->descriptor, &ctx);
}

static
void EcsSetType(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsType, type, 1);

    ecs_world_t *world = rows->world;

    int i;
    for(i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];

        /* If type does not contain a descriptor, application will have to
         * manually initialize type specific data */
        if (!type[i].descriptor) {

            /* For some types we can set the size and alignment automatically */
            if (!type[i].size || !type[i].alignment) {
                switch(type[i].kind) {
                case EcsBitmaskType:
                case EcsEnumType:
                    type[i].size = sizeof(int32_t);
                    type[i].alignment = ECS_ALIGNOF(int32_t);
                    break;
                case EcsVectorType:
                    type[i].size = sizeof(ecs_vector_t*);
                    type[i].alignment = ECS_ALIGNOF(ecs_vector_t*);
                    break;
                case EcsMapType:
                    type[i].size = sizeof(ecs_map_t*);
                    type[i].alignment = ECS_ALIGNOF(ecs_map_t*);
                    break;                    
                default:
                    break;
                }
            }

            continue;
        }

        switch(type[i].kind) {
        case EcsPrimitiveType:
            ecs_set_primitive(world, e, type);
            break;
        case EcsBitmaskType:
            ecs_set_bitmask(world, e, type);
            break;
        case EcsEnumType:
            ecs_set_enum(world, e, type);
            break;
        case EcsStructType:
            ecs_set_struct(world, e, type);
            break;
        case EcsArrayType:
            ecs_set_array(world, e, type);
            break;
        case EcsVectorType:
            ecs_set_vector(world, e, type);
            break;
        case EcsMapType:
            ecs_set_map(world, e, type);
            break;
        }
    }
}

/* Utility macro to insert meta data for type with meta descriptor */
#define ECS_COMPONENT_TYPE(world, type)\
    ecs_set_ptr(world, ecs_entity(type), EcsType, &__##type##__)

/* Utility macro to insert metadata for primitive type */
#define ECS_COMPONENT_PRIMITIVE(world, type, kind)\
    ECS_COMPONENT(world, type);\
    ecs_set(world, ecs_entity(type), EcsType, {EcsPrimitiveType});\
    ecs_set(world, ecs_entity(type), EcsPrimitive, {kind})

void FlecsComponentsMetaImport(
    ecs_world_t *world,
    int flags)
{
    ECS_MODULE(world, FlecsComponentsMeta);

    ECS_COMPONENT(world, EcsPrimitive);
    ECS_COMPONENT(world, EcsEnum);
    ECS_COMPONENT(world, EcsBitmask);
    ECS_COMPONENT(world, EcsMember);
    ECS_COMPONENT(world, EcsStruct);
    ECS_COMPONENT(world, EcsArray);
    ECS_COMPONENT(world, EcsVector);
    ECS_COMPONENT(world, EcsMap);
    ECS_COMPONENT(world, EcsType);
    ECS_COMPONENT(world, ecs_type_op_kind_t);
    ECS_COMPONENT(world, ecs_type_op_t);
    ECS_COMPONENT(world, EcsTypeSerializer);

    ECS_SYSTEM(world, EcsSetType, EcsOnSet, EcsType);

    ECS_SYSTEM(world, EcsAddStruct, EcsOnAdd, EcsStruct);

    ECS_SYSTEM(world, EcsSetPrimitive, EcsOnSet, EcsPrimitive, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetEnum, EcsOnSet, EcsEnum, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetBitmask, EcsOnSet, EcsBitmask, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetStruct, EcsOnSet, EcsStruct, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetArray, EcsOnSet, EcsArray, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetVector, EcsOnSet, EcsVector, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetMap, EcsOnSet, EcsMap, $.FlecsComponentsMeta);

    ECS_EXPORT_COMPONENT(EcsPrimitive);
    ECS_EXPORT_COMPONENT(EcsEnum);
    ECS_EXPORT_COMPONENT(EcsBitmask);
    ECS_EXPORT_COMPONENT(EcsStruct);
    ECS_EXPORT_COMPONENT(EcsArray);
    ECS_EXPORT_COMPONENT(EcsVector);
    ECS_EXPORT_COMPONENT(EcsMap);
    ECS_EXPORT_COMPONENT(EcsType);
    ECS_EXPORT_COMPONENT(EcsTypeSerializer);  

    /* -- Initialize builtin primitive types -- */
    ECS_COMPONENT_PRIMITIVE(world, bool, EcsBool);
    ECS_COMPONENT_PRIMITIVE(world, char, EcsChar);
    ECS_COMPONENT_PRIMITIVE(world, ecs_byte_t, EcsByte);
    ECS_COMPONENT_PRIMITIVE(world, uint8_t, EcsU8);
    ECS_COMPONENT_PRIMITIVE(world, uint16_t, EcsU16);
    ECS_COMPONENT_PRIMITIVE(world, uint32_t, EcsU32);
    ECS_COMPONENT_PRIMITIVE(world, uint64_t, EcsU64);
    ECS_COMPONENT_PRIMITIVE(world, uintptr_t, EcsUPtr);
    ECS_COMPONENT_PRIMITIVE(world, int8_t, EcsI8);
    ECS_COMPONENT_PRIMITIVE(world, int16_t, EcsI16);
    ECS_COMPONENT_PRIMITIVE(world, int32_t, EcsI32);
    ECS_COMPONENT_PRIMITIVE(world, int64_t, EcsI64);
    ECS_COMPONENT_PRIMITIVE(world, intptr_t, EcsIPtr);
    ECS_COMPONENT_PRIMITIVE(world, size_t, EcsUPtr);
    ECS_COMPONENT_PRIMITIVE(world, float, EcsF32);
    ECS_COMPONENT_PRIMITIVE(world, double, EcsF64);
    ECS_COMPONENT_PRIMITIVE(world, ecs_string_t, EcsString);
    ECS_COMPONENT_PRIMITIVE(world, ecs_entity_t, EcsEntity);  

    /* -- Initialize builtin meta components -- */
    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"ecs_primitive_kind_t"}),
        EcsType, &__ecs_primitive_kind_t__);

    ecs_set(world, ecs_set(world, 0,
        EcsId, {"ecs_type_kind_t"}),
        EcsType, {
            EcsEnumType, 
            sizeof(ecs_type_kind_t), 
            ECS_ALIGNOF(ecs_type_kind_t), 
            __ecs_type_kind_t__
        });


    /* Insert meta definitions for other types */
    ECS_COMPONENT_TYPE(world, EcsPrimitive);
    ECS_COMPONENT_TYPE(world, EcsEnum);
    ECS_COMPONENT_TYPE(world, EcsBitmask);
    ECS_COMPONENT_TYPE(world, EcsMember);
    ECS_COMPONENT_TYPE(world, EcsStruct);
    ECS_COMPONENT_TYPE(world, EcsArray);
    ECS_COMPONENT_TYPE(world, EcsVector);
    ECS_COMPONENT_TYPE(world, EcsMap);
    ECS_COMPONENT_TYPE(world, EcsType);
    ECS_COMPONENT_TYPE(world, ecs_type_op_kind_t);
    ECS_COMPONENT_TYPE(world, ecs_type_op_t);
    ECS_COMPONENT_TYPE(world, EcsTypeSerializer);

    /* -- Initialize metadata for public Flecs core components -- */
    ecs_set(world, ecs_set(world, ecs_entity(EcsId),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsString});

    ecs_set(world, ecs_entity(EcsComponent), EcsType, {
        .kind = EcsStructType, 
        .size = sizeof(EcsComponent),
        .alignment = ECS_ALIGNOF(EcsComponent),
        .descriptor = "{size_t size;}"
    });

    /* Export components to public handles */
    ECS_EXPORT_COMPONENT(bool);
    ECS_EXPORT_COMPONENT(char);
    ECS_EXPORT_COMPONENT(ecs_byte_t);
    ECS_EXPORT_COMPONENT(uint8_t);
    ECS_EXPORT_COMPONENT(uint16_t);
    ECS_EXPORT_COMPONENT(uint32_t);
    ECS_EXPORT_COMPONENT(uint64_t);
    ECS_EXPORT_COMPONENT(uintptr_t);
    ECS_EXPORT_COMPONENT(int8_t);
    ECS_EXPORT_COMPONENT(int16_t);
    ECS_EXPORT_COMPONENT(int32_t);
    ECS_EXPORT_COMPONENT(int64_t);
    ECS_EXPORT_COMPONENT(intptr_t);
    ECS_EXPORT_COMPONENT(size_t);
    ECS_EXPORT_COMPONENT(float);
    ECS_EXPORT_COMPONENT(double);
    ECS_EXPORT_COMPONENT(ecs_string_t);
    ECS_EXPORT_COMPONENT(ecs_entity_t);
}
