#include <flecs_meta.h>
#include "parser.h"
#include "serializer.h"
#include "type.h"

ECS_COMPONENT_DECLARE(EcsPrimitive);
ECS_COMPONENT_DECLARE(EcsEnum);
ECS_COMPONENT_DECLARE(EcsBitmask);
ECS_COMPONENT_DECLARE(EcsMember);
ECS_COMPONENT_DECLARE(EcsStruct);
ECS_COMPONENT_DECLARE(EcsArray);
ECS_COMPONENT_DECLARE(EcsVector);
ECS_COMPONENT_DECLARE(EcsMap);
ECS_COMPONENT_DECLARE(EcsMetaType);
ECS_COMPONENT_DECLARE(ecs_type_op_kind_t);
ECS_COMPONENT_DECLARE(ecs_type_op_t);
ECS_COMPONENT_DECLARE(EcsMetaTypeSerializer);

static ECS_CTOR(EcsMetaType, ptr, {
    ptr->descriptor = NULL;
    ptr->alias = NULL;
})

static ECS_DTOR(EcsMetaType, ptr, {
    ecs_os_free((char*)ptr->descriptor);
    ptr->descriptor = NULL;
    ptr->alias = NULL;
})

static ECS_COPY(EcsMetaType, dst, src, {
    ecs_os_free((char*)dst->descriptor);

    dst->descriptor = ecs_os_strdup(src->descriptor);

    dst->kind = src->kind;
    dst->size = src->size;
    dst->alignment = src->alignment;
    dst->alias = src->alias;
})

static ECS_MOVE(EcsMetaType, dst, src, {
    ecs_os_free((char*)dst->descriptor);

    dst->kind = src->kind;
    dst->size = src->size;
    dst->alignment = src->alignment;
    dst->descriptor = src->descriptor;
    dst->alias = src->alias;

    src->descriptor = NULL;
    src->alias = NULL;
})

static ECS_CTOR(EcsStruct, ptr, {
    ptr->members = NULL;
    ptr->is_partial = false;
})

static ECS_DTOR(EcsStruct, ptr, {
    ecs_vector_each(ptr->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(ptr->members);
})

static ECS_COPY(EcsStruct, dst, src, {
    ecs_vector_each(dst->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(dst->members);

    dst->members = ecs_vector_copy(src->members, EcsMember);
    EcsMember *src_struct = ecs_vector_first(src->members, EcsMember);

    ecs_vector_each(dst->members, EcsMember, m, {
        m->name = ecs_os_strdup(src_struct->name);
        src_struct++;
    });
    dst->is_partial = src->is_partial;
})

static ECS_MOVE(EcsStruct, dst, src, {
    ecs_vector_each(dst->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(dst->members);

    dst->members = src->members;
    dst->is_partial = src->is_partial;

    src->members = NULL;
})

static ECS_CTOR(EcsEnum, ptr, {
    ptr->constants = NULL;
})

static ECS_DTOR(EcsEnum, ptr, {
    ecs_map_each(ptr->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(ptr->constants);

    ptr->constants = NULL;
})

static ECS_COPY(EcsEnum, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    int32_t count = ecs_map_count(src->constants);
    dst->constants = ecs_map_new(char*, count);

    ecs_map_each(src->constants, char*, key, c_ptr, {
        char *constant_name = ecs_os_strdup(*c_ptr);
        ecs_map_set(dst->constants, key, &constant_name);
    })
})

static ECS_MOVE(EcsEnum, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    dst->constants = src->constants;
    src->constants = NULL;
})

static ECS_CTOR(EcsBitmask, ptr, {
    ptr->constants = NULL;
})

static ECS_DTOR(EcsBitmask, ptr, {
    ecs_map_each(ptr->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(ptr->constants);

    ptr->constants = NULL;
})

static ECS_COPY(EcsBitmask, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    int32_t count = ecs_map_count(src->constants);
    dst->constants = ecs_map_new(char*, count);

    ecs_map_each(src->constants, char*, key, c_ptr, {
        char *constant_name = ecs_os_strdup(*c_ptr);
        ecs_map_set(dst->constants, key, &constant_name);
    })
})

static ECS_MOVE(EcsBitmask, dst, src, {
    ecs_map_each(dst->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(dst->constants);

    dst->constants = src->constants;
    src->constants = NULL;
})

static ECS_CTOR(EcsMetaTypeSerializer, ptr, {
    ptr->ops = NULL;
})

static ECS_DTOR(EcsMetaTypeSerializer, ptr, {
    ecs_vector_free(ptr->ops);
    ptr->ops = NULL;
})

static ECS_COPY(EcsMetaTypeSerializer, dst, src, {
    ecs_vector_free(dst->ops);
    dst->ops = ecs_vector_copy(src->ops, ecs_type_op_t);
})

static ECS_MOVE(EcsMetaTypeSerializer, dst, src, {
    ecs_vector_free(dst->ops);

    dst->ops = src->ops;
    src->ops = NULL;
})

static
void ecs_set_primitive(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

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
    if (!strcmp(descr, "string_t")) {
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
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_map_t *constants = ecs_map_new(char*, 1);
    ecs_meta_constant_t token;
    int64_t last_value = 0;

    while ((ptr = ecs_meta_parse_constant(ptr, &token, &ctx))) {
        if (token.is_value_set) {
            last_value = token.value;
        } else if (is_bitmask) {
            ecs_meta_error(&ctx, ptr,
                "bitmask requires explicit value assignment");
        }

        char *constant_name = ecs_os_strdup(token.name);
        ecs_map_set(constants, last_value, &constant_name);

        last_value ++;
    }

    ecs_set_id(world, e, comp, sizeof(EcsEnum), &(EcsEnum){
        .constants = constants
    });
}

static
void ecs_set_bitmask(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_set_constants(world, e, ecs_id(EcsBitmask), true, type);
}

static
void ecs_set_enum(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_set_constants(world, e, ecs_id(EcsEnum), false, type);
}

static
void ecs_set_struct(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);
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

    if (!ecs_vector_count(members)) {
        ecs_parser_error(name, type->descriptor, 0, "empty struct declaration");
    }

    is_partial = token.is_partial;

    ecs_set(world, e, EcsStruct, {members, is_partial});
}

static
void ecs_set_array(
    ecs_world_t *world,
    ecs_entity_t e,
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

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
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

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
    EcsMetaType *type)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(e != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(type != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *ptr = type->descriptor;
    const char *name = ecs_get_name(world, e);

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_meta_lookup_map(world, e, type->descriptor, &ctx);
}

static
void EcsSetType(ecs_iter_t *it) {
    EcsMetaType *type = ecs_term(it, EcsMetaType, 1);

    ecs_world_t *world = it->world;

    int i;
    for(i = 0; i < it->count; i ++) {
        ecs_entity_t e = it->entities[i];

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

void ecs_new_meta(
    ecs_world_t *world,
    ecs_entity_t component,
    EcsMetaType *meta_type)
{
    ecs_assert(ecs_id(EcsMetaType) != 0, ECS_MODULE_UNDEFINED, "flecs.meta");

    if (meta_type->alias) {
        EcsMetaType *alias = meta_type->alias;
        meta_type->kind = alias->kind;
        meta_type->descriptor = alias->descriptor;
    }

    ecs_set_ptr(world, component, EcsMetaType, meta_type);
}

/* Utility macro to insert meta data for type with meta descriptor */
#define ECS_COMPONENT_TYPE(world, type)\
    ecs_set_ptr(world, ecs_id(type), EcsMetaType, &ecs_meta(type))

/* Utility macro to insert metadata for primitive type */
#define ECS_COMPONENT_PRIMITIVE(world, type, kind)\
    ECS_COMPONENT(world, type);\
    ecs_set(world, ecs_id(type), EcsMetaType, {EcsPrimitiveType, 0, 0, NULL, NULL});\
    ecs_set(world, ecs_id(type), EcsPrimitive, {kind})

void FlecsMetaImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsMeta);

    ecs_set_name_prefix(world, "Ecs");

    ECS_COMPONENT_DEFINE(world, EcsPrimitive);
    ECS_COMPONENT_DEFINE(world, EcsEnum);
    ECS_COMPONENT_DEFINE(world, EcsBitmask);
    ECS_COMPONENT_DEFINE(world, EcsMember);
    ECS_COMPONENT_DEFINE(world, EcsStruct);
    ECS_COMPONENT_DEFINE(world, EcsArray);
    ECS_COMPONENT_DEFINE(world, EcsVector);
    ECS_COMPONENT_DEFINE(world, EcsMap);
    ECS_COMPONENT_DEFINE(world, EcsMetaType);
    ECS_COMPONENT_DEFINE(world, ecs_type_op_kind_t);
    ECS_COMPONENT_DEFINE(world, ecs_type_op_t);
    ECS_COMPONENT_DEFINE(world, EcsMetaTypeSerializer);

    ECS_OBSERVER(world, EcsSetType, EcsOnSet, EcsMetaType);

    ecs_set_component_actions(world, EcsMetaType, {
        .ctor = ecs_ctor(EcsMetaType),
        .dtor = ecs_dtor(EcsMetaType),
        .copy = ecs_copy(EcsMetaType),
        .move = ecs_move(EcsMetaType)
    });

    ecs_set_component_actions(world, EcsStruct, {
        .ctor = ecs_ctor(EcsStruct),
        .dtor = ecs_dtor(EcsStruct),
        .copy = ecs_copy(EcsStruct),
        .move = ecs_move(EcsStruct)
    });

    ecs_set_component_actions(world, EcsEnum, {
        .ctor = ecs_ctor(EcsEnum),
        .dtor = ecs_dtor(EcsEnum),
        .copy = ecs_copy(EcsEnum),
        .move = ecs_move(EcsEnum)
    });

    ecs_set_component_actions(world, EcsBitmask, {
        .ctor = ecs_ctor(EcsBitmask),
        .dtor = ecs_dtor(EcsBitmask),
        .copy = ecs_copy(EcsBitmask),
        .move = ecs_move(EcsBitmask)
    });

    ecs_set_component_actions(world, EcsMetaTypeSerializer, {
        .ctor = ecs_ctor(EcsMetaTypeSerializer),
        .dtor = ecs_dtor(EcsMetaTypeSerializer),
        .copy = ecs_copy(EcsMetaTypeSerializer),
        .move = ecs_move(EcsMetaTypeSerializer)
    });

    ECS_OBSERVER(world, EcsSetPrimitive, EcsOnSet, Primitive);
    ECS_OBSERVER(world, EcsSetEnum, EcsOnSet, Enum);
    ECS_OBSERVER(world, EcsSetBitmask, EcsOnSet, Bitmask);
    ECS_OBSERVER(world, EcsSetStruct, EcsOnSet, Struct);
    ECS_OBSERVER(world, EcsSetArray, EcsOnSet, Array);
    ECS_OBSERVER(world, EcsSetVector, EcsOnSet, Vector);
    ECS_OBSERVER(world, EcsSetMap, EcsOnSet, Map);

    ECS_EXPORT_COMPONENT(EcsPrimitive);
    ECS_EXPORT_COMPONENT(EcsEnum);
    ECS_EXPORT_COMPONENT(EcsBitmask);
    ECS_EXPORT_COMPONENT(EcsStruct);
    ECS_EXPORT_COMPONENT(EcsArray);
    ECS_EXPORT_COMPONENT(EcsVector);
    ECS_EXPORT_COMPONENT(EcsMap);
    ECS_EXPORT_COMPONENT(EcsMetaType);
    ECS_EXPORT_COMPONENT(EcsMetaTypeSerializer);

    /* -- Initialize builtin primitive types -- */
    ecs_entity_t old_scope = ecs_set_scope(world, EcsFlecsCore);
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
    ECS_COMPONENT_PRIMITIVE(world, ecs_size_t, EcsI32);
    ECS_COMPONENT_PRIMITIVE(world, ecs_string_t, EcsString);
    ECS_COMPONENT_PRIMITIVE(world, ecs_entity_t, EcsEntity);

    /* If stdbool is included, the above bool declaration will have been
     * registered with the name _Bool. To make sure meta also knows the type by
     * its regular name, check and register if necessary */
    if (!ecs_lookup(world, "bool")) {
        ecs_entity_t type = ecs_component_init(world, &(ecs_component_desc_t) {
            .entity.name = "bool",
            .size = sizeof(bool),
            .alignment = ECS_ALIGNOF(bool)
        });

        ecs_set(world, type, EcsMetaType, {
            EcsPrimitiveType, 0, 0, NULL, NULL});
        ecs_set(world, type, EcsPrimitive, {EcsBool});
    }

    ecs_set_scope(world, old_scope);

    /* -- Initialize builtin meta components -- */

    ecs_set_ptr(world, ecs_set_name(world, 0, "ecs_primitive_kind_t"),
        EcsMetaType, &ecs_meta(ecs_primitive_kind_t));

    ecs_set(world, ecs_set_name(world, 0, "ecs_type_kind_t"),
        EcsMetaType, {
            EcsEnumType,
            sizeof(ecs_type_kind_t),
            ECS_ALIGNOF(ecs_type_kind_t),
            ecs_meta(ecs_type_kind_t),
            NULL
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
    ECS_COMPONENT_TYPE(world, EcsMetaType);
    ECS_COMPONENT_TYPE(world, ecs_type_op_kind_t);
    ECS_COMPONENT_TYPE(world, ecs_type_op_t);
    ECS_COMPONENT_TYPE(world, EcsMetaTypeSerializer);

    /* -- Initialize metadata for public Flecs core components -- */
    ecs_set(world, ecs_id(EcsIdentifier), EcsMetaType, {
        .kind = EcsStructType,
        .size = sizeof(EcsIdentifier),
        .alignment = ECS_ALIGNOF(EcsIdentifier),
        .descriptor = "{ char *value; ECS_PRIVATE }",
        .alias = NULL
    });

    ecs_set(world, ecs_id(EcsComponent), EcsMetaType, {
        .kind = EcsStructType,
        .size = sizeof(EcsComponent),
        .alignment = ECS_ALIGNOF(EcsComponent),
        .descriptor = "{int32_t size; int32_t alignment;}",
        .alias = NULL
    });
}
