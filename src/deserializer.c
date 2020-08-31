#include "flecs_meta.h"

static
ecs_meta_scope_t* get_scope(
    ecs_meta_cursor_t *cursor)
{
    ecs_assert(cursor != NULL, ECS_INVALID_PARAMETER, NULL);
    return &cursor->scope[cursor->depth];
}

static
ecs_type_op_t* get_op(
    ecs_meta_scope_t *scope)
{
    ecs_type_op_t *ops = ecs_vector_first(scope->ops, ecs_type_op_t);
    ecs_assert(ops != NULL, ECS_INVALID_PARAMETER, NULL);
    return &ops[scope->cur_op];
}

static
ecs_type_op_t* get_ptr(
    ecs_meta_scope_t *scope)
{
    ecs_type_op_t *op = get_op(scope);

    if (scope->vector) {
        _ecs_vector_set_min_count(&scope->vector, ECS_VECTOR_U(op->size, op->alignment), scope->cur_elem + 1);
        scope->base = ecs_vector_first_t(scope->vector, op->size, op->alignment);
    }

    return ECS_OFFSET(scope->base, op->offset + op->size * scope->cur_elem);
}

ecs_meta_cursor_t ecs_meta_cursor(
    ecs_world_t *world,
    ecs_entity_t type, 
    void *base)
{
    ecs_assert(world != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(type != 0, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(base != NULL, ECS_INVALID_PARAMETER, NULL);
    
    ecs_meta_cursor_t result;
    ecs_entity_t ecs_entity(EcsMetaTypeSerializer) = 
        ecs_lookup_fullpath(world, "flecs.meta.MetaTypeSerializer");
    ecs_assert(ecs_entity(EcsMetaTypeSerializer) != 0, ECS_INVALID_PARAMETER, NULL);

    const EcsMetaTypeSerializer *ser = ecs_get(world, type, EcsMetaTypeSerializer);
    ecs_assert(ser != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_type_op_t *ops = ecs_vector_first(ser->ops, ecs_type_op_t);
    ecs_assert(ops != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(ops[0].kind == EcsOpHeader, ECS_INVALID_PARAMETER, NULL);

    result.world = world;
    result.depth = 0;
    result.scope[0].type = type;
    result.scope[0].ops = ser->ops;
    result.scope[0].start = 1;
    result.scope[0].cur_op = 1;
    result.scope[0].cur_elem = 0;
    result.scope[0].base = base;
    result.scope[0].is_collection = false;
    result.scope[0].count = 0;
    result.scope[0].vector = NULL;

    return result;
}

void* ecs_meta_get_ptr(
    ecs_meta_cursor_t *cursor)
{
    return get_ptr(cursor->scope);
}

int ecs_meta_next(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    int32_t ops_count = ecs_vector_count(scope->ops);

    if (scope->count) {
        if (scope->cur_op >= scope->count) {
            return -1;
        }
    } else {
        if (scope->cur_op >= ops_count) {
            return -1;
        }
    }

    if (scope->is_collection) {
        scope->cur_op = 1;
        scope->cur_elem ++;

        if (scope->count) {
            if (scope->cur_elem >= scope->count) {
                return -1;
            }
        }        
    } else {
        scope->cur_op ++;
    }

    return 0;
}

int ecs_meta_move(
    ecs_meta_cursor_t *cursor,
    int32_t pos)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    int32_t ops_count = ecs_vector_count(scope->ops);

    if (pos >= ops_count) {
        return -1;
    }

    scope->cur_op = pos;

    return 0;
}

int ecs_meta_move_name(
    ecs_meta_cursor_t *cursor,
    const char *name)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *ops = ecs_vector_first(scope->ops, ecs_type_op_t);
    int32_t i, ops_count = ecs_vector_count(scope->ops);
    int32_t depth = 1;

    for (i = scope->start; i < ops_count; i ++) {
        ecs_type_op_t *op = &ops[i];

        if (depth <= 1) {
            if (op->name && !strcmp(op->name, name)) {
                scope->cur_op = i;
                return 0;
            }
        }

        if (op->kind == EcsOpPush) {
            depth ++;
        }

        if (op->kind == EcsOpPop) {
            depth --;
            if (depth < 0) {
                return -1;
            }
        }
    }

    return -1;
}

int ecs_meta_push(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    if (scope->vector) {
        /* This makes sure the vector has enough space for the pushed element */
        get_ptr(scope);
    }
    
    scope->cur_op ++;
    cursor->depth ++;
    ecs_meta_scope_t *child_scope = get_scope(cursor);
    child_scope->cur_elem = 0;

    switch(op->kind) {
    case EcsOpPush: {
        child_scope->base = ECS_OFFSET(scope->base, op->size * scope->cur_elem);
        child_scope->start = scope->cur_op;
        child_scope->cur_op = scope->cur_op;
        child_scope->ops = scope->ops;
        child_scope->is_collection = false;
        child_scope->count = 0;
        child_scope->vector = NULL;
        break;
    }
    case EcsOpArray:
    case EcsOpVector: {
        void *ptr = ECS_OFFSET(scope->base, op->offset);
        const EcsMetaTypeSerializer *ser = ecs_get_ref_w_entity(cursor->world, 
            &op->is.collection, 0, 0);
        ecs_assert(ser != NULL, ECS_INTERNAL_ERROR, NULL);

        ecs_vector_t *ops = ser->ops;

        if (op->kind == EcsOpArray) {
            child_scope->base = ptr;
            child_scope->count = op->count;
            child_scope->vector = NULL;
        } else {
            ecs_vector_t *v = *(ecs_vector_t**)ptr;
            if (!v) {
                v = ecs_vector_new_t(op->size, op->alignment, 2);
            } else {
                ecs_vector_set_count_t(&v, op->size, op->alignment, 0);
            }
            
            child_scope->base = ecs_vector_first_t(v, op->size, op->alignment);
            child_scope->count = 0;
            child_scope->vector = v;
        }
        child_scope->start = 1;
        child_scope->cur_op = 1;
        child_scope->ops = ops;
        child_scope->is_collection = true;
#ifndef NDEBUG
        ecs_type_op_t *hdr = ecs_vector_first(child_scope->ops, ecs_type_op_t);
        ecs_assert(hdr->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);
#endif
        }
        break;
    default:
        return -1;
    }

    return 0;
}

int ecs_meta_pop(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *ops = ecs_vector_first(scope->ops, ecs_type_op_t);
    int32_t i, ops_count = ecs_vector_count(scope->ops);

    if (scope->is_collection) {
        cursor->depth --;
        if (scope->vector) {
            /* Vector ptr may have changed, so reassign vector field */
            ecs_meta_scope_t *parent_scope = get_scope(cursor);
            parent_scope->cur_op --;
            void *ptr = get_ptr(parent_scope);
            *(ecs_vector_t**)ptr = scope->vector;
            parent_scope->cur_op ++;
        }         
        return 0;
    } else {
        for (i = scope->cur_op; i < ops_count; i ++) {
            ecs_type_op_t *op = &ops[i];
            if (op->kind == EcsOpPop) {
                cursor->depth -- ;                
                ecs_meta_scope_t *parent_scope = get_scope(cursor);
                if (parent_scope->is_collection) {
                    parent_scope->cur_op = 1;
                    parent_scope->cur_elem ++;
                } else {
                    parent_scope->cur_op = i;                   
                }
                return 0;
            }
        }
    }

    return -1;
}

int ecs_meta_set_bool(
    ecs_meta_cursor_t *cursor,
    bool value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive || op->is.primitive != EcsBool) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);
        *(bool*)ptr = value;
        return 0;
    }
}

int ecs_meta_set_char(
    ecs_meta_cursor_t *cursor,
    char value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive || op->is.primitive != EcsChar) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);
        *(char*)ptr = value;
        return 0;
    }
}

int ecs_meta_set_int(
    ecs_meta_cursor_t *cursor,
    int64_t value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsI8:
            if (value > INT8_MAX) {
                return -1;
            }
            *(int8_t*)ptr = (int8_t)value;
            break;
        case EcsI16:
            if (value > INT16_MAX) {
                return -1;
            }
            *(int16_t*)ptr = (int16_t)value;
            break;
        case EcsI32:
            if (value > INT32_MAX) {
                return -1;
            }
            *(int32_t*)ptr = (int32_t)value;
            break;
        case EcsI64:
            if (value > INT64_MAX) {
                return -1;
            }
            *(int64_t*)ptr = (int64_t)value;
            break;
        case EcsIPtr:
            if (value > INTPTR_MAX) {
                return -1;
            }
            *(intptr_t*)ptr = (intptr_t)value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_uint(
    ecs_meta_cursor_t *cursor,
    uint64_t value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsU8:
        case EcsByte:
            if (value > UINT8_MAX) {
                return -1;
            }
            *(uint8_t*)ptr = (uint8_t)value;
            break;
        case EcsU16:
            if (value > UINT16_MAX) {
                return -1;
            }
            *(uint16_t*)ptr = (uint16_t)value;
            break;
        case EcsU32:
            if (value > UINT32_MAX) {
                return -1;
            }
            *(uint32_t*)ptr = (uint32_t)value;
            break;
        case EcsU64:
            if (value > UINT64_MAX) {
                return -1;
            }
            *(uint64_t*)ptr = (uint64_t)value;
            break;
        case EcsUPtr:
            if (value > UINTPTR_MAX) {
                return -1;
            }
            *(uintptr_t*)ptr = (uintptr_t)value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_float(
    ecs_meta_cursor_t *cursor,
    double value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsF32:
            *(float*)ptr = (float)value;
            break;
        case EcsF64:
            *(double*)ptr = value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_string(
    ecs_meta_cursor_t *cursor,
    const char *value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsString:
            if (*(char**)ptr) {
                ecs_os_free(*(char**)ptr);
            }
            if (value) {
                *(char**)ptr = ecs_os_strdup(value);
            } else {
                *(char**)ptr = NULL;
            }
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_entity(
    ecs_meta_cursor_t *cursor,
    ecs_entity_t value)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsEntity:
            *(ecs_entity_t*)ptr = value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_null(
    ecs_meta_cursor_t *cursor)
{
    ecs_meta_scope_t *scope = get_scope(cursor);
    ecs_type_op_t *op = get_op(scope);

    switch(op->kind) {
    case EcsOpPrimitive: {
        if (op->is.primitive != EcsString) {
            return -1;
        }

        void *ptr = get_ptr(scope);
        char *str = *(char**)ptr;
        if (str) {
            ecs_os_free(str);
        }

        *(char**)ptr = NULL;
        break;
    }

    case EcsOpVector: {
        void *ptr = get_ptr(scope);
        ecs_vector_t *vec = *(ecs_vector_t**)ptr;
        if (vec) {
            ecs_vector_free(vec);
        }

        *(ecs_vector_t**)ptr = NULL;
        break;
    }

    default:
        return -1;
        break;
    }

    return 0;
}
