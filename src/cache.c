#include <include/meta.h>

ecs_vector_params_t cache_op_param = {.element_size = sizeof(ecs_meta_cache_op_t)};

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t component,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles);

static
bool is_complex(ecs_meta_cache_op_kind_t kind) {
    if (kind == EcsOpPrimitive || 
        kind == EcsOpEnum ||
        kind == EcsOpBitmask)
    {
        return false;
    } else {
        return true;
    }
}

static
bool is_inline(ecs_meta_cache_op_kind_t kind) {
    if (!is_complex(kind) || kind == EcsOpPush)
    {
        return true;
    } else {
        return false;
    }
}

static
size_t component_sizeof(
    ecs_world_t *world, 
    ecs_entity_t entity)
{
    EcsComponent *comp = ecs_get_ptr(world, entity, EcsComponent);
    return comp->size;
}

static
ecs_vector_t* serialize_primitive(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaPrimitive *type = ecs_get_ptr(world, entity, EcsMetaPrimitive);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpPrimitive, 
        .count = 1,
        .size = component_sizeof(world, entity),
        .data.primitive_kind = type->kind
    };

    return ops;
}

static
ecs_vector_t* serialize_enum(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaEnum *type = ecs_get_ptr(world, entity, EcsMetaEnum);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpEnum, 
        .count = 1,
        .size = sizeof(int32_t),
        .data.enum_constants = type->constants
    };

    return ops;
}

static
ecs_vector_t* serialize_bitmask(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaBitmask *type = ecs_get_ptr(world, entity, EcsMetaBitmask);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpBitmask,
        .count = 1,
        .size = sizeof(uint64_t),
        .data.enum_constants = type->constants
    };

    return ops;
}

static
ecs_vector_t* serialize_struct(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaStruct *type = ecs_get_ptr(world, entity, EcsMetaStruct);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpPush,
        .count = 1,
        .size = component_sizeof(world, entity)
    };

    EcsMetaMember *members = ecs_vector_first(type->members);
    int32_t i, count = ecs_vector_count(type->members);

    for (i = 0; i < count; i ++) {
        ops = serialize_type(world, members[i].type, ops, handles);
        ecs_meta_cache_op_t *last_op = ecs_vector_last(ops, &cache_op_param);
        last_op->name = members[i].name;
        last_op->offset = members[i].offset;
    }

    op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpPop
    };
    
    return ops;
}

static
ecs_vector_t* serialize_array(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaArray *type = ecs_get_ptr(world, entity, EcsMetaArray);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    EcsMetaCache *element_cache = ecs_get_ptr(world, type->element_type, EcsMetaCache);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *elem_op = ecs_vector_first(element_cache->ops);

    /* If element is inlined, don't add indirection to other cache */
    if (ecs_vector_count(element_cache->ops) == 1 && is_inline(elem_op->kind))
    {
        /* Serialize element op inline, and set count on first inserted op */
        uint32_t el_start = ecs_vector_count(ops);
        serialize_type(world, type->element_type, ops, handles);
        ecs_meta_cache_op_t *start = ecs_vector_get(ops, &cache_op_param, el_start);
        ecs_assert(start != NULL, ECS_INTERNAL_ERROR, NULL);

        start->count = type->size;
        start->size = component_sizeof(world, type->element_type);
    } else {
        ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
        *op = (ecs_meta_cache_op_t){
            .kind = EcsOpArray, 
            .count = type->size,
            .data.element_ops = element_cache->ops,
            .size = component_sizeof(world, entity)
        };
    }

    return ops;
}

static
ecs_vector_t* serialize_vector(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaVector *type = ecs_get_ptr(world, entity, EcsMetaVector);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    EcsMetaCache *element_cache = ecs_get_ptr(world, type->element_type, EcsMetaCache);
    ecs_assert(element_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpVector,
        .count = 1,
        .size = sizeof(ecs_vector_t*),
        .data.element_ops = element_cache->ops
    };

    return ops;
}

static
ecs_vector_t* serialize_map(
    ecs_world_t *world,
    ecs_entity_t entity,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaMap *type = ecs_get_ptr(world, entity, EcsMetaMap);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    EcsMetaCache *key_cache = ecs_get_ptr(world, type->key_type, EcsMetaCache);
    ecs_assert(key_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    EcsMetaCache *value_cache = ecs_get_ptr(world, type->value_type, EcsMetaCache);
    ecs_assert(value_cache != NULL, ECS_INTERNAL_ERROR, NULL);

    /* Keys cannot be composite types */
    if (ecs_vector_count(key_cache->ops) != 1) {
        ecs_abort(ECS_INTERNAL_ERROR, NULL);
    }

    ecs_meta_cache_op_t *key_op = ecs_vector_first(key_cache->ops);

    /* Key types may not be complex */
    ecs_assert(!is_complex(key_op->kind), ECS_INTERNAL_ERROR, NULL);

    ecs_meta_cache_op_t *op = ecs_vector_add(&ops, &cache_op_param);
    *op = (ecs_meta_cache_op_t){
        .kind = EcsOpVector, 
        .count = 1,
        .size = sizeof(ecs_map_t*),
        .data.map = {
            .key_op = key_op,
            .value_ops = value_cache->ops,
        }
    };

    return ops;
}

static
ecs_vector_t* serialize_type(
    ecs_world_t *world,
    ecs_entity_t component,
    ecs_vector_t *ops,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    EcsMetaType *type = ecs_get_ptr(world, component, EcsMetaType);

    switch(type->kind) {
    case EcsPrimitive:
        ops = serialize_primitive(world, component, ops, handles);
        break;
    case EcsEnum:
        ops = serialize_enum(world, component, ops, handles);
        break;
    case EcsBitmask:
        ops = serialize_bitmask(world, component, ops, handles);
        break;
    case EcsStruct:
        ops = serialize_struct(world, component, ops, handles);
        break;
    case EcsArray:
        ops = serialize_array(world, component, ops, handles);
        break;
    case EcsVector:
        ops = serialize_vector(world, component, ops, handles);
        break;
    case EcsMap:
        ops = serialize_map(world, component, ops, handles);
        break;
    }

    return ops;
}

static
void serialize_component(
    ecs_world_t *world, 
    ecs_entity_t component,
    EcsComponentsMetaHandles *handles)
{
    EcsComponentsMeta_ImportHandles(*handles);

    ecs_vector_t *ops = ecs_vector_new(&cache_op_param, 1);
    ops = serialize_type(world, component, ops, handles);

    ecs_set(world, component, EcsMetaCache, {
        .ops = ops
    });
}

void InitCache(ecs_rows_t *rows) {
    ECS_IMPORT_COLUMN(rows, EcsComponentsMeta, 2);

    int i;
    for (i = 0; i < rows->count; i ++) {
        serialize_component(
            rows->world, rows->entities[i], &ecs_module(EcsComponentsMeta));
    }
}

void* ecs_meta_get_ptr(
    void *base, 
    ecs_meta_cache_op_t *op)
{
    return  ECS_OFFSET(base, op->offset);
}

bool ecs_meta_get_bool(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    ecs_assert(op->data.primitive_kind == EcsBool, ECS_INVALID_PARAMETERS, NULL);
    return *(bool*)ecs_meta_get_ptr(base, op);
}

char ecs_meta_get_char(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    ecs_assert(op->data.primitive_kind == EcsChar, ECS_INVALID_PARAMETERS, NULL);
    return *(char*)ecs_meta_get_ptr(base, op);    
}

int64_t ecs_meta_get_int(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);

    int64_t result;
    void *value = ecs_meta_get_ptr(base, op);

    switch(op->data.primitive_kind) {
    case EcsI8:
        result = *(int8_t*)value;
        break;
    case EcsI16:
        result = *(int16_t*)value;
        break;
    case EcsI32:
        result = *(int32_t*)value;
        break;
    case EcsI64:
        result = *(int64_t*)value;
        break;
    default:
        ecs_abort(ECS_INVALID_PARAMETERS, NULL);
    }

    return result;
}

uint64_t ecs_meta_get_uint(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    
    uint64_t result;
    void *value = ecs_meta_get_ptr(base, op);

    switch(op->data.primitive_kind) {
    case EcsU8:
        result = *(uint8_t*)value;
        break;
    case EcsU16:
        result = *(uint16_t*)value;
        break;
    case EcsU32:
        result = *(uint32_t*)value;
        break;
    case EcsU64:
        result = *(uint64_t*)value;
        break;
    default:
        ecs_abort(ECS_INVALID_PARAMETERS, NULL);
    }

    return result; 
}

int32_t ecs_meta_get_enum(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpEnum, ECS_INVALID_PARAMETERS, NULL);
    return *(int32_t*)ecs_meta_get_ptr(base, op);
}

uint64_t ecs_meta_get_bitmask(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpBitmask, ECS_INVALID_PARAMETERS, NULL);
    return *(uint64_t*)ecs_meta_get_ptr(base, op);
}

double ecs_meta_get_float(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    
    uint64_t result;
    void *value = ecs_meta_get_ptr(base, op);

    switch(op->data.primitive_kind) {
    case EcsF32:
        result = *(float*)value;
        break;
    case EcsF64:
        result = *(double*)value;
        break;
    default:
        ecs_abort(ECS_INVALID_PARAMETERS, NULL);
    }

    return result;   
}

uintptr_t ecs_meta_get_word(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    ecs_assert(op->data.primitive_kind == EcsWord, ECS_INVALID_PARAMETERS, NULL);
    return *(uintptr_t*)ecs_meta_get_ptr(base, op); 
}

char* ecs_meta_get_string(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    ecs_assert(op->data.primitive_kind == EcsString, ECS_INVALID_PARAMETERS, NULL);
    return *(char**)ecs_meta_get_ptr(base, op);    
}

ecs_entity_t ecs_meta_get_entity(
    void *base,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpPrimitive, ECS_INVALID_PARAMETERS, NULL);
    ecs_assert(op->data.primitive_kind == EcsEntity, ECS_INVALID_PARAMETERS, NULL);
    return *(ecs_entity_t*)ecs_meta_get_ptr(base, op);      
}

const char* ecs_meta_enum_to_string(
    int32_t value,
    ecs_meta_cache_op_t *op)
{
    ecs_assert(op->kind == EcsOpEnum, ECS_INVALID_PARAMETERS, NULL);
    EcsMetaEnumConstant *constants = ecs_vector_first(op->data.enum_constants);
    uint32_t count = ecs_vector_count(op->data.enum_constants);

    int i;
    for (i = 0; i < count; i ++) {
        if (constants[i].value == value) {
            return constants[i].name;
        }
    }

    ecs_abort(ECS_INVALID_PARAMETERS, NULL);

    return NULL;
}

void ecs_meta_bitmask_to_string(
    uint64_t value,
    ecs_meta_cache_op_t *op,
    ut_strbuf *buf)
{
    ecs_assert(op->kind == EcsOpBitmask, ECS_INVALID_PARAMETERS, NULL);
    EcsMetaBitmaskConstant *constants = ecs_vector_first(op->data.bitmask_constants);
    uint32_t count = ecs_vector_count(op->data.bitmask_constants);

    if (!value) {
        ut_strbuf_appendstr(buf, "0");
    } else {
        int i, found = 0;
        for (i = 0; i < count; i ++) {
            if (constants[i].value & value) {
                if (i) {
                    ut_strbuf_appendstr(buf, "|");
                }
                ut_strbuf_appendstr(buf, constants[i].name);
                found ++;
            }
        }

        ecs_assert(found != 0, ECS_INVALID_PARAMETERS, NULL);
    }
}