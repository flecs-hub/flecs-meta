#include "flecs_components_meta.h"

static
ecs_meta_scope_t* get_scope(
    ecs_meta_iter_t *iter)
{
    ecs_assert(iter != NULL, ECS_INVALID_PARAMETER, NULL);
    return &iter->scope[iter->depth];
}

static
ecs_type_op_t* get_op(
    ecs_meta_scope_t *scope)
{
    ecs_type_op_t *ops = ecs_vector_first(scope->ops);
    ecs_assert(ops != NULL, ECS_INVALID_PARAMETER, NULL);
    return &ops[scope->cur_op];
}

static
ecs_type_op_t* get_ptr(
    ecs_meta_scope_t *scope)
{
    ecs_type_op_t *op = get_op(scope);
    return ECS_OFFSET(scope->base, op->offset + op->size * scope->cur_elem);
}

ecs_meta_iter_t ecs_meta_iter(
    ecs_world_t *world,
    ecs_entity_t type, 
    void *base)
{
    ecs_assert(world != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(type != 0, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(base != NULL, ECS_INVALID_PARAMETER, NULL);
    
    ecs_meta_iter_t result;
    ecs_entity_t ecs_entity(EcsTypeSerializer) = 
        ecs_lookup(world, "EcsTypeSerializer");
    ecs_assert(ecs_entity(EcsTypeSerializer) != 0, ECS_INVALID_PARAMETER, NULL);

    EcsTypeSerializer *ser = ecs_get_ptr(world, type, EcsTypeSerializer);
    ecs_assert(ser != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_type_op_t *ops = ecs_vector_first(ser->ops);
    ecs_assert(ops != NULL, ECS_INVALID_PARAMETER, NULL);
    ecs_assert(ops[0].kind == EcsOpHeader, ECS_INVALID_PARAMETER, NULL);

    result.world = world;
    result.depth = 0;
    result.scope[0].type = type;
    result.scope[0].ops = ser->ops;
    result.scope[0].start = 1;
    result.scope[0].cur_op = 1;
    result.scope[0].base = base;
    result.scope[0].is_collection = false;
    result.scope[0].count = 0;

    return result;
}

void* ecs_meta_get_ptr(
    ecs_meta_iter_t *iter)
{
    return get_ptr(iter->scope);
}

int ecs_meta_next(
    ecs_meta_iter_t *iter)
{
    ecs_meta_scope_t *scope = get_scope(iter);
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
    } else {
        scope->cur_op ++;
    }

    return 0;
}

int ecs_meta_move_cursor(
    ecs_meta_iter_t *iter,
    int32_t cursor)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    int32_t ops_count = ecs_vector_count(scope->ops);

    if (cursor >= ops_count) {
        return -1;
    }

    scope->cur_op = cursor;

    return 0;
}

int ecs_meta_move_cursor_name(
    ecs_meta_iter_t *iter,
    const char *name)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    ecs_type_op_t *ops = ecs_vector_first(scope->ops);
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
    ecs_meta_iter_t *iter)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    ecs_type_op_t *op = get_op(scope);
    
    scope->cur_op ++;
    iter->depth ++;
    ecs_meta_scope_t *child_scope = get_scope(iter);
    child_scope->cur_elem = 0;

    switch(op->kind) {
    case EcsOpPush: {
        child_scope->base = scope->base;
        child_scope->start = scope->cur_op;
        child_scope->cur_op = scope->cur_op;
        child_scope->ops = scope->ops;
        child_scope->is_collection = false;
        child_scope->count = 0;
        break;
    }
    case EcsOpArray:
        child_scope->base = ECS_OFFSET(scope->base, op->offset);
        child_scope->start = 1;
        child_scope->cur_op = 1;
        child_scope->ops = op->is.collection;
        child_scope->is_collection = true;
        child_scope->count = op->count;
#ifndef NDEBUG
        ecs_type_op_t *op = ecs_vector_first(child_scope->ops);
        ecs_assert(op->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);
#endif
        break;
    default:
        return -1;
    }

    return 0;
}

int ecs_meta_pop(
    ecs_meta_iter_t *iter)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    ecs_type_op_t *ops = ecs_vector_first(scope->ops);
    int32_t i, ops_count = ecs_vector_count(scope->ops);

    if (scope->is_collection) {
        iter->depth --;
        return 0;
    } else {
        for (i = scope->cur_op; i < ops_count; i ++) {
            ecs_type_op_t *op = &ops[i];
            if (op->kind == EcsOpPop) {
                iter->depth -- ;
                ecs_meta_scope_t *parent_scope = get_scope(iter);
                parent_scope->cur_op = i;
                return 0;
            }
        }
    }

    return -1;
}

int ecs_meta_set_bool(
    ecs_meta_iter_t *iter,
    bool value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
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
    ecs_meta_iter_t *iter,
    char value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
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
    ecs_meta_iter_t *iter,
    int64_t value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsI8:
            *(int8_t*)ptr = value;
            break;
        case EcsI16:
            *(int16_t*)ptr = value;
            break;
        case EcsI32:
            *(int32_t*)ptr = value;
            break;
        case EcsI64:
            *(int64_t*)ptr = value;
            break;
        case EcsIPtr:
            *(intptr_t*)ptr = value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_uint(
    ecs_meta_iter_t *iter,
    uint64_t value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsU8:
            *(uint8_t*)ptr = value;
            break;
        case EcsU16:
            *(uint16_t*)ptr = value;
            break;
        case EcsU32:
            *(uint32_t*)ptr = value;
            break;
        case EcsU64:
            *(uint64_t*)ptr = value;
            break;
        case EcsUPtr:
            *(uintptr_t*)ptr = value;
            break;
        default:
            return -1;
            break;
        }

        return 0;
    }
}

int ecs_meta_set_float(
    ecs_meta_iter_t *iter,
    double value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
    ecs_type_op_t *op = get_op(scope);
    
    if (op->kind != EcsOpPrimitive) {
        return -1;
    } else {
        void *ptr = get_ptr(scope);

        switch(op->is.primitive) {
        case EcsF32:
            *(float*)ptr = value;
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
    ecs_meta_iter_t *iter,
    const char *value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
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
    ecs_meta_iter_t *iter,
    ecs_entity_t value)
{
    ecs_meta_scope_t *scope = get_scope(iter);
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
