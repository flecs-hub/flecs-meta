#include <flecs_components_meta.h>
#include "parser.h"

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module);

static
size_t ecs_get_primitive_size(
    ecs_primitive_kind_t kind) 
{
    switch(kind) {
    case EcsBool: return sizeof(bool);
    case EcsChar: return sizeof(char);
    case EcsByte: return sizeof(char);
    case EcsU8: return sizeof(uint8_t);
    case EcsU16: return sizeof(uint16_t);
    case EcsU32: return sizeof(uint32_t);
    case EcsU64: return sizeof(uint64_t);
    case EcsI8: return sizeof(int8_t);
    case EcsI16: return sizeof(int16_t);
    case EcsI32: return sizeof(int32_t);
    case EcsI64: return sizeof(int64_t);
    case EcsF32: return sizeof(float);
    case EcsF64: return sizeof(double);
    case EcsIPtr: return sizeof(intptr_t);
    case EcsUPtr: return sizeof(uintptr_t);
    case EcsString: return sizeof(char*);
    case EcsEntity: return sizeof(ecs_entity_t);
    default:
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }
}

static
size_t ecs_get_primitive_alignment(
    ecs_primitive_kind_t kind) 
{
    switch(kind) {
    case EcsBool: return ECS_ALIGNOF(bool);
    case EcsChar: return ECS_ALIGNOF(char);
    case EcsByte: return ECS_ALIGNOF(char);
    case EcsU8: return ECS_ALIGNOF(uint8_t);
    case EcsU16: return ECS_ALIGNOF(uint16_t);
    case EcsU32: return ECS_ALIGNOF(uint32_t);
    case EcsU64: return ECS_ALIGNOF(uint64_t);
    case EcsI8: return ECS_ALIGNOF(int8_t);
    case EcsI16: return ECS_ALIGNOF(int16_t);
    case EcsI32: return ECS_ALIGNOF(int32_t);
    case EcsI64: return ECS_ALIGNOF(int64_t);
    case EcsF32: return ECS_ALIGNOF(float);
    case EcsF64: return ECS_ALIGNOF(double);
    case EcsIPtr: return ECS_ALIGNOF(intptr_t);
    case EcsUPtr: return ECS_ALIGNOF(uintptr_t);
    case EcsString: return ECS_ALIGNOF(char*);
    case EcsEntity: return ECS_ALIGNOF(ecs_entity_t);
    default:
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }
}

static
ecs_vector_t* serialize_primitive(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsPrimitive *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{
    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = ecs_get_primitive_size(type->kind),
            .alignment = ecs_get_primitive_alignment(type->kind)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    *op = (ecs_type_op_t) {
        .kind = EcsOpPrimitive,
        .size = ecs_get_primitive_size(type->kind),
        .alignment = ecs_get_primitive_alignment(type->kind),
        .count = 1,
        .is.primitive = type->kind
    };

    return ops;
}

static
ecs_vector_t* serialize_enum(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsEnum *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{    
    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(int32_t),
            .alignment = ECS_ALIGNOF(int32_t)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    *op = (ecs_type_op_t) {
        .kind = EcsOpEnum,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = type->constants
    };

    return ops;
}

static
ecs_vector_t* serialize_bitmask(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsBitmask *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{    
    ecs_type_op_t *op;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(int32_t),
            .alignment = ECS_ALIGNOF(int32_t)
        };
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);

    *op = (ecs_type_op_t) {
        .kind = EcsOpEnum,
        .size = sizeof(int32_t),
        .alignment = ECS_ALIGNOF(int32_t),
        .count = 1,
        .is.constant = type->constants
    };

    return ops;
}

static
ecs_vector_t* serialize_struct(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsStruct *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{
    FlecsComponentsMetaImportHandles(*module);

    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPush,
    };

    size_t size = 0;
    size_t alignment = 0;

    EcsMember *members = ecs_vector_first(type->members);
    int32_t i, count = ecs_vector_count(type->members);

    for (i = 0; i < count; i ++) {
        /* Add type operations of member to struct ops */
        int32_t prev_count = ecs_vector_count(ops);
        ops = serialize_type(world, members[i].type, ops, module);
        int32_t count = ecs_vector_count(ops);

        /* At least one op should be added */
        ecs_assert(prev_count != count, ECS_INTERNAL_ERROR, NULL);
        ecs_assert(ops != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_type_op_t *op = ecs_vector_get(ops, ecs_type_op_t, prev_count);
        op->name = members[i].name;

        /* If only one operation was added, we can use the size and alignment of
         * this operation. Otherwise we have to get it from the type itself */
        size_t member_size;
        uint8_t member_alignment;

        if (count - prev_count == 1) {
            member_size = op->size;
            member_alignment = op->alignment;
        } else {
            EcsType *type = ecs_get_ptr(world, members[i].type, EcsType);
            member_size = type->size;
            member_alignment = type->alignment;
        }

        ecs_assert(member_size != 0, ECS_INTERNAL_ERROR, op->name);
        ecs_assert(member_alignment != 0, ECS_INTERNAL_ERROR, op->name);

        size = ECS_ALIGN(size, member_alignment);
        op->offset = size;

        size += member_size;      

        if (member_alignment > alignment) {
            alignment = member_alignment;
        }
    }

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t) {
        .kind = EcsOpPop,
    };

    if (op_header) {
        op_header = ecs_vector_first(ops); /* Might have reallocd */
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = size,
            .alignment = alignment
        };
    }

    return ops;
}

static
ecs_vector_t* serialize_array(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsArray *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *handles)
{
    FlecsComponentsMetaImportHandles(*handles);

    EcsTypeSerializer *element_cache = ecs_get_ptr(world, type->element_type, EcsTypeSerializer);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_type_op_t *op_header = NULL;
    if (!ops) {
        op_header = ecs_vector_add(&ops, ecs_type_op_t);
    }

    /* If element is inlined, don't add indirection to other cache */
    EcsType *element_type = ecs_get_ptr(world, type->element_type, EcsType);
    ecs_assert(element_type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_type_op_t *op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpArray, 
        .count = type->count,
        .size = element_type->size,
        .alignment = element_type->alignment,
        .is.collection = element_cache->ops
    };

    if (op_header) {
        op_header = ecs_vector_first(ops);
        *op_header = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = op->size,
            .alignment = op->alignment
        };            
    }        

    return ops;
}

static
ecs_vector_t* serialize_vector(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsVector *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *handles)
{
    FlecsComponentsMetaImportHandles(*handles);

    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_vector_t*),
            .alignment = ECS_ALIGNOF(ecs_vector_t*)
        };         
    }

    EcsTypeSerializer *element_cache = ecs_get_ptr(world, type->element_type, EcsTypeSerializer);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpVector, 
        .count = 1,
        .size = sizeof(ecs_vector_t*),
        .alignment = ECS_ALIGNOF(ecs_vector_t*),
        .is.collection = element_cache->ops
    };

    return ops;
}

static
ecs_vector_t* serialize_map(
    ecs_world_t *world,
    ecs_entity_t entity,
    EcsMap *type,
    ecs_vector_t *ops,
    FlecsComponentsMeta *handles)
{
    FlecsComponentsMetaImportHandles(*handles);

    ecs_type_op_t *op = NULL;
    if (!ops) {
        op = ecs_vector_add(&ops, ecs_type_op_t);
        *op = (ecs_type_op_t) {
            .kind = EcsOpHeader,
            .size = sizeof(ecs_map_t*),
            .alignment = ECS_ALIGNOF(ecs_map_t*)
        };         
    }

    EcsTypeSerializer *key_cache = ecs_get_ptr(world, type->key_type, EcsTypeSerializer);
    ecs_assert(key_cache != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(key_cache->ops != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ecs_vector_count(key_cache->ops) != 0, ECS_INTERNAL_ERROR, NULL);

    /* Make sure first op is the header */
    ecs_type_op_t *key_op = ecs_vector_first(key_cache->ops);
    ecs_assert(key_op->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);

    if (ecs_vector_count(key_cache->ops) != 2) {
        EcsType *ptr = ecs_get_ptr(world, entity, EcsType);
        ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_meta_parse_ctx_t ctx = {
            .name = ecs_get_id(world, entity),
            .decl = ptr->descriptor
        };

        ecs_meta_error( &ctx, ctx.decl, 
            "invalid key type '%s' for map", ecs_get_id(world, type->key_type));
    }

    key_op = ecs_vector_get(key_cache->ops, ecs_type_op_t, 1);
    ecs_assert(key_op != NULL, ECS_INTERNAL_ERROR, NULL);

    if (key_op->count != 1) {
        EcsType *ptr = ecs_get_ptr(world, entity, EcsType);
        ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_meta_parse_ctx_t ctx = {
            .name = ecs_get_id(world, entity),
            .decl = ptr->descriptor
        };        
        ecs_meta_error( &ctx, ctx.decl, "array type invalid for key type");
    }

    EcsTypeSerializer *element_cache = ecs_get_ptr(world, type->element_type, EcsTypeSerializer);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    op = ecs_vector_add(&ops, ecs_type_op_t);
    *op = (ecs_type_op_t){
        .kind = EcsOpMap, 
        .count = 1,
        .size = sizeof(ecs_map_t*),
        .alignment = ECS_ALIGNOF(ecs_map_t*),
        .is.map = {
            .key_op = key_op,
            element_cache->ops
        }
    };

    return ops;
}

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    FlecsComponentsMeta *module)
{
    FlecsComponentsMetaImportHandles(*module);

    EcsType *type = ecs_get_ptr(world, entity, EcsType);
    ecs_assert(type != NULL, ECS_INVALID_PARAMETER, NULL);

    switch(type->kind) {
    case EcsPrimitiveType: {
        EcsPrimitive *t = ecs_get_ptr(world, entity, EcsPrimitive);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_primitive(world, entity, t, ops, module);
    }

    case EcsEnumType: {
        EcsEnum *t = ecs_get_ptr(world, entity, EcsEnum);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_enum(world, entity, t, ops, module);
    }

    case EcsBitmaskType: {
        EcsBitmask *t = ecs_get_ptr(world, entity, EcsBitmask);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_bitmask(world, entity, t, ops, module);
    }    

    case EcsStructType: {
        EcsStruct *t = ecs_get_ptr(world, entity, EcsStruct);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_struct(world, entity, t, ops, module);
    }    

    case EcsArrayType: {
        EcsArray *t = ecs_get_ptr(world, entity, EcsArray);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_array(world, entity, t, ops, module);
    }

    case EcsVectorType: {
        EcsVector *t = ecs_get_ptr(world, entity, EcsVector);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_vector(world, entity, t, ops, module);
    }    

    case EcsMapType: {
        EcsMap *t = ecs_get_ptr(world, entity, EcsMap);
        ecs_assert(t != NULL, ECS_INTERNAL_ERROR, NULL);
        return serialize_map(world, entity, t, ops, module);
    }

    default:
        break;
    }

    return NULL;
}

void EcsSetPrimitive(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsPrimitive, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(world, e, EcsTypeSerializer, {
            serialize_primitive(
                world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetEnum(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsEnum, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_enum(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetBitmask(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsBitmask, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_bitmask(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetStruct(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsStruct, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_struct(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetArray(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsArray, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_array(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetVector(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsVector, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_vector(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}

void EcsSetMap(ecs_rows_t *rows) {
    ECS_COLUMN(rows, EcsMap, type, 1);
    ECS_IMPORT_COLUMN(rows, FlecsComponentsMeta, 2);

    ecs_world_t *world = rows->world;

    int i;
    for (i = 0; i < rows->count; i ++) {
        ecs_entity_t e = rows->entities[i];
        ecs_set(rows->world, rows->entities[i], EcsTypeSerializer, { 
            serialize_map(world, e, &type[i], NULL, &ecs_module(FlecsComponentsMeta))
        });
    }
}
