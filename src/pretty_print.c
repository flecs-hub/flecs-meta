#include <flecs_components_meta.h>

/* Simple serializer to turn values into strings */
static
void str_ser_type(
    ecs_vector_t *ser, 
    void *base, 
    ecs_strbuf_t *str);

static
void str_ser_type_op(
    ecs_type_op_t *op, 
    void *base,
    ecs_strbuf_t *str);

static
void str_ser_primitive(
    ecs_type_op_t *op, 
    void *base, 
    ecs_strbuf_t *str) 
{
    const char *bool_str[] = { "false", "true" };

    switch(op->is.primitive) {
    case EcsBool:
        ecs_strbuf_appendstr(str, bool_str[(int)*(bool*)base]);
        break;
    case EcsChar:
        ecs_strbuf_appendstrn(str, "\"", 1);
        ecs_strbuf_appendstrn(str, (char*)base, 1);
        ecs_strbuf_appendstrn(str, "\"", 1);
        break;
    case EcsByte:
        ecs_strbuf_append(str, "%u", *(uint8_t*)base);
        break;
    case EcsU8:
        ecs_strbuf_append(str, "%u", *(uint8_t*)base);
        break;
    case EcsU16:
        ecs_strbuf_append(str, "%u", *(uint16_t*)base);
        break;
    case EcsU32:
        ecs_strbuf_append(str, "%u", *(uint32_t*)base);
        break;
    case EcsU64:
        ecs_strbuf_append(str, "%u", *(uint64_t*)base);
        break;
    case EcsI8:
        ecs_strbuf_append(str, "%d", *(int8_t*)base);
        break;
    case EcsI16:
        ecs_strbuf_append(str, "%d", *(int16_t*)base);
        break;
    case EcsI32:
        ecs_strbuf_append(str, "%d", *(int32_t*)base);
        break;
    case EcsI64:
        ecs_strbuf_append(str, "%d", *(int64_t*)base);
        break;
    case EcsF32:
        ecs_strbuf_append(str, "%f", *(float*)base);
        break;
    case EcsF64:
        ecs_strbuf_append(str, "%f", *(double*)base);
        break;
    case EcsIPtr:
        ecs_strbuf_appendstrn(str, "%i", *(intptr_t*)base);
        break;
    case EcsUPtr:
        ecs_strbuf_appendstrn(str, "%u", *(uintptr_t*)base);
        break;
    case EcsString: {
        char *value = *(char**)base;
        if (value) {
            ecs_strbuf_appendstrn(str, "\"", 1);
            ecs_strbuf_appendstr(str, value);
            ecs_strbuf_appendstrn(str, "\"", 1);
        } else {
            ecs_strbuf_appendstr(str, "nullptr");
        }
        break;
    }
    case EcsEntity:
        ecs_strbuf_appendstrn(str, "%u", *(intptr_t*)base);
        break;
    }
}

static
void str_ser_enum(
    ecs_type_op_t *op, 
    void *base, 
    ecs_strbuf_t *str) 
{
    ecs_assert(op->is.constant != NULL, ECS_INVALID_PARAMETER, NULL);

    int32_t value = *(int32_t*)base;
    
    char **constant = ecs_map_get(op->is.constant, char*, value);
    ecs_assert(constant != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_strbuf_appendstr(str, *constant);
}

static
void str_ser_bitmask(
    ecs_type_op_t *op, 
    void *base, 
    ecs_strbuf_t *str) 
{
    ecs_assert(op->is.constant != NULL, ECS_INVALID_PARAMETER, NULL);

    int32_t value = *(int32_t*)base;
    ecs_map_key_t key, count = 0;
    char **constant;

    ecs_map_iter_t it = ecs_map_iter(op->is.constant);
    while ((constant = ecs_map_next(&it, char*, &key))) {
        if ((value & key) == key) {
            if (count) {
                ecs_strbuf_appendstrn(str, " | ", 1);
            }
            ecs_strbuf_appendstr(str, *constant);
            count ++;
        }
    }
}


static
void str_ser_elements(
    ecs_vector_t *elem_ops, 
    void *base, 
    int32_t elem_count, 
    int32_t elem_size,
    ecs_strbuf_t *str)
{
    ecs_strbuf_appendstrn(str, "[", 1);

    void *ptr = base;

    int i;
    for (i = 0; i < elem_count; i ++) {
        if (i) {
            ecs_strbuf_appendstr(str, ", ");
        }
        
        str_ser_type(elem_ops, ptr, str);
        ptr = ECS_OFFSET(ptr, elem_size);
    }

    ecs_strbuf_appendstrn(str, "]", 1);
}

static
void str_ser_array(
    ecs_type_op_t *op, 
    void *base, 
    ecs_strbuf_t *str) 
{
    str_ser_elements(op->is.collection, base, op->count, op->size, str);
}

static
void str_ser_vector(
    ecs_type_op_t *op, 
    void *base, 
    ecs_strbuf_t *str) 
{
    ecs_vector_t *value = *(ecs_vector_t**)base;
    
    int32_t count = ecs_vector_count(value);
    void *array = ecs_vector_first(value);
    ecs_vector_t *elem_ops = op->is.collection;
    
    ecs_type_op_t *elem_op_hdr = (ecs_type_op_t*)ecs_vector_first(elem_ops);
    ecs_assert(elem_op_hdr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(elem_op_hdr->kind == EcsOpHeader, ECS_INTERNAL_ERROR, NULL);
    size_t elem_size = elem_op_hdr->size;

    str_ser_elements(elem_ops, array, count, elem_size, str);
}

static
void str_ser_map(
    ecs_type_op_t *op, 
    void *base, 
    ecs_strbuf_t *str) 
{
    ecs_map_t *value = *(ecs_map_t**)base;

    ecs_map_iter_t it = ecs_map_iter(value);  
    ecs_map_key_t key; 
    void *ptr;
    int32_t count = 0;

    ecs_strbuf_appendstrn(str, "{", 1);

    while ((ptr = _ecs_map_next(&it, 0, &key))) {
        if (count) {
            ecs_strbuf_appendstr(str, ", ");
        }

        str_ser_type_op(op->is.map.key_op, (void*)&key, str);
        ecs_strbuf_appendstr(str, " = ");
        str_ser_type(op->is.map.element_ops, ptr, str);
        count ++;

        key = 0;
    }

    ecs_strbuf_appendstrn(str, "}", 1);
}

static
void str_ser_type_op(
    ecs_type_op_t *op, 
    void *base,
    ecs_strbuf_t *str) 
{
    switch(op->kind) {
    case EcsOpHeader:
    case EcsOpPush:
    case EcsOpPop:
        /* Should not be parsed as single op */
        ecs_abort(ECS_INVALID_PARAMETER, NULL);
        break;
    case EcsOpPrimitive:
        str_ser_primitive(op, ECS_OFFSET(base, op->offset), str);
        break;
    case EcsOpEnum:
        str_ser_enum(op, ECS_OFFSET(base, op->offset), str);
        break;
    case EcsOpBitmask:
        str_ser_bitmask(op, ECS_OFFSET(base, op->offset), str);
        break;
    case EcsOpArray:
        str_ser_array(op, ECS_OFFSET(base, op->offset), str);
        break;
    case EcsOpVector:
        str_ser_vector(op, ECS_OFFSET(base, op->offset), str);
        break;
    case EcsOpMap:
        str_ser_map(op, ECS_OFFSET(base, op->offset), str);
        break;
    }
}

static
void str_ser_type(ecs_vector_t *ser, void *base, ecs_strbuf_t *str) {
    ecs_type_op_t *ops = (ecs_type_op_t*)ecs_vector_first(ser);
    int32_t count = ecs_vector_count(ser);

    int elem_count[64] = {0};
    int sp = 0;

    for (int i = 0; i < count; i ++) {
        ecs_type_op_t *op = &ops[i];

        if (sp && elem_count[sp] && op->kind != EcsOpPop) {
            ecs_strbuf_appendstr(str, ", ");
        }

        elem_count[sp] ++;

        if (op->name) {
            ecs_strbuf_append(str, "%s = ", op->name);
        }

        switch(op->kind) {
        case EcsOpHeader:
            break;
        case EcsOpPush:
            ecs_strbuf_appendstrn(str, "{", 1);
            sp ++;
            elem_count[sp] = 0;
            break;
        case EcsOpPop:
            ecs_strbuf_appendstrn(str, "}", 1);
            sp --;
            break;
        default:
            str_ser_type_op(op, base, str);
            break;
        }
    }
}

char* ecs_pretty_print_ptr(
    ecs_world_t *world, 
    ecs_entity_t type, 
    void* ptr)
{
    ecs_entity_t ecs_entity(EcsTypeSerializer) = ecs_lookup(world, "EcsTypeSerializer");
    EcsTypeSerializer *ser = ecs_get_ptr(world, type, EcsTypeSerializer);
    ecs_assert(ser != NULL, ECS_INVALID_PARAMETER, NULL);

    ecs_strbuf_t str = ECS_STRBUF_INIT;
    str_ser_type(ser->ops, ptr, &str);
    return ecs_strbuf_get(&str);
}

char* ecs_pretty_print_entity(
    ecs_world_t *world, 
    ecs_entity_t entity)
{
    ecs_type_t type = ecs_get_type(world, entity);
    ecs_entity_t *ids = (ecs_entity_t*)ecs_vector_first(type);
    int32_t count = ecs_vector_count(type);
    
    ecs_entity_t EEcsTypeSerializer = ecs_lookup(world, "EcsTypeSerializer");
    ecs_strbuf_t str = ECS_STRBUF_INIT;

    const char *name = ecs_get_id(world, entity);
    if (name) {
        ecs_strbuf_append(&str, "%s: ", name);
    }

    ecs_strbuf_appendstr(&str, "{\n");

    int i, comps_serialized = 0;
    for (i = 0; i < count; i ++) {
        EcsTypeSerializer *ser = ecs_get_ptr(world, ids[i], EcsTypeSerializer);
        if (ser) {
            void *ptr = _ecs_get_ptr(world, entity, ids[i]);
            ecs_strbuf_append(&str, "    %s: ", ecs_get_id(world, ids[i]));
            str_ser_type(ser->ops, ptr, &str);
            ecs_strbuf_appendstr(&str, "\n");
            comps_serialized ++;
        }
    }

    ecs_strbuf_appendstr(&str, "}");

    return ecs_strbuf_get(&str);  
}
