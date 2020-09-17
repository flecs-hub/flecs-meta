#include <flecs_meta.h>
#include "parser.h"
#include "serializer.h"
#include "type.h"

ECS_CTOR(EcsStruct, ptr, {
    ptr->members = NULL;
    ptr->is_partial = false;
})

ECS_DTOR(EcsStruct, ptr, {
    ecs_vector_each(ptr->members, EcsMember, m, {
        ecs_os_free(m->name);
    });
    ecs_vector_free(ptr->members);
})

ECS_CTOR(EcsEnum, ptr, {
    ptr->constants = NULL;
})

ECS_DTOR(EcsEnum, ptr, {
    ecs_map_each(ptr->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })
    ecs_map_free(ptr->constants);
})

ECS_CTOR(EcsBitmask, ptr, {
    ptr->constants = NULL;
})

ECS_DTOR(EcsBitmask, ptr, {
    ecs_map_each(ptr->constants, char*, key, c_ptr, {
        ecs_os_free(*c_ptr);
    })    
    ecs_map_free(ptr->constants);
})

ECS_CTOR(EcsMetaTypeSerializer, ptr, {
    ptr->ops = NULL;
})

ECS_DTOR(EcsMetaTypeSerializer, ptr, {
    ecs_vector_free(ptr->ops);
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

    ecs_entity_t ecs_entity(EcsPrimitive) = ecs_lookup_fullpath(world, "flecs.meta.Primitive");
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

    ecs_set_ptr_w_entity(world, e, comp, sizeof(EcsEnum), &(EcsEnum){
        .constants = constants
    });
}

static
void ecs_set_bitmask(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsMetaType *type) 
{
    ecs_entity_t comp = ecs_lookup_fullpath(world, "flecs.meta.Bitmask");
    ecs_assert(comp != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_set_constants(world, e, comp, true, type);
}

static
void ecs_set_enum(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsMetaType *type) 
{
    ecs_entity_t comp = ecs_lookup_fullpath(world, "flecs.meta.Enum");
    ecs_assert(comp != 0, ECS_INTERNAL_ERROR, NULL);    
    ecs_set_constants(world, e, comp, false, type);
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

    is_partial = token.is_partial;

    ecs_entity_t ecs_entity(EcsStruct) = ecs_lookup_fullpath(world, "flecs.meta.Struct");
    ecs_assert(ecs_entity(EcsStruct) != 0, ECS_INTERNAL_ERROR, NULL);
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
    ECS_COLUMN(it, EcsMetaType, type, 1);

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

static
void ctor_initialize_0(
    ecs_world_t *world,
    ecs_entity_t component,
    const ecs_entity_t *entities,
    void *ptr,
    size_t size,
    int32_t count,
    void *ctx)
{
    (void)world;
    (void)component;
    (void)entities;
    (void)ctx;
    memset(ptr, 0, size * (size_t)count);
}

void ecs_new_meta(
    ecs_world_t *world,
    ecs_entity_t component,
    EcsMetaType *meta_type)
{
    ecs_entity_t ecs_entity(EcsMetaType) = 
        ecs_lookup_fullpath(world, "flecs.meta.MetaType");
    ecs_assert(ecs_entity(EcsMetaType) != 0, ECS_MODULE_UNDEFINED, "flecs.meta");

    ecs_set_ptr(world, component, EcsMetaType, meta_type);\
    ecs_set_component_actions_w_entity(world, component, &(EcsComponentLifecycle){
        .ctor = ctor_initialize_0
    });
}

/* Utility macro to insert meta data for type with meta descriptor */
#define ECS_COMPONENT_TYPE(world, type)\
    ecs_set_ptr(world, ecs_entity(type), EcsMetaType, &__##type##__)

/* Utility macro to insert metadata for primitive type */
#define ECS_COMPONENT_PRIMITIVE(world, type, kind)\
    ECS_COMPONENT(world, type);\
    ecs_set(world, ecs_entity(type), EcsMetaType, {EcsPrimitiveType, 0, 0, NULL});\
    ecs_set(world, ecs_entity(type), EcsPrimitive, {kind})

void FlecsMetaImport(
    ecs_world_t *world)
{
    ECS_MODULE(world, FlecsMeta);

    ecs_set_name_prefix(world, "Ecs");

    ECS_COMPONENT(world, EcsPrimitive);
    ECS_COMPONENT(world, EcsEnum);
    ECS_COMPONENT(world, EcsBitmask);
    ECS_COMPONENT(world, EcsMember);
    ECS_COMPONENT(world, EcsStruct);
    ECS_COMPONENT(world, EcsArray);
    ECS_COMPONENT(world, EcsVector);
    ECS_COMPONENT(world, EcsMap);
    ECS_COMPONENT(world, EcsMetaType);
    ECS_COMPONENT(world, ecs_type_op_kind_t);
    ECS_COMPONENT(world, ecs_type_op_t);
    ECS_COMPONENT(world, EcsMetaTypeSerializer);

    ECS_SYSTEM(world, EcsSetType, EcsOnSet, EcsMetaType);

    ecs_set_component_actions(world, EcsStruct, {
        .ctor = ecs_ctor(EcsStruct),
        .dtor = ecs_dtor(EcsStruct)
    });

    ecs_set_component_actions(world, EcsEnum, {
        .ctor = ecs_ctor(EcsEnum),
        .dtor = ecs_dtor(EcsEnum)
    });    

    ecs_set_component_actions(world, EcsBitmask, {
        .ctor = ecs_ctor(EcsBitmask),
        .dtor = ecs_dtor(EcsBitmask)
    });

    ecs_set_component_actions(world, EcsMetaTypeSerializer, {
        .ctor = ecs_ctor(EcsMetaTypeSerializer),
        .dtor = ecs_dtor(EcsMetaTypeSerializer)
    });    

    ECS_SYSTEM(world, EcsSetPrimitive, EcsOnSet, Primitive, flecs.meta:flecs.meta);
    ECS_SYSTEM(world, EcsSetEnum, EcsOnSet, Enum, flecs.meta:flecs.meta);
    ECS_SYSTEM(world, EcsSetBitmask, EcsOnSet, Bitmask, flecs.meta:flecs.meta);
    ECS_SYSTEM(world, EcsSetStruct, EcsOnSet, Struct, flecs.meta:flecs.meta);
    ECS_SYSTEM(world, EcsSetArray, EcsOnSet, Array, flecs.meta:flecs.meta);
    ECS_SYSTEM(world, EcsSetVector, EcsOnSet, Vector, flecs.meta:flecs.meta);
    ECS_SYSTEM(world, EcsSetMap, EcsOnSet, Map, flecs.meta:flecs.meta);

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
        ecs_entity_t type = ecs_new_component(
            world, 0, "bool", sizeof(bool), ECS_ALIGNOF(bool));
        ecs_set(world, type, EcsMetaType, {
            EcsPrimitiveType, 0, 0, NULL});
        ecs_set(world, type, EcsPrimitive, {EcsBool});
    }

    ecs_set_scope(world, old_scope);

    /* -- Initialize builtin meta components -- */
    ecs_set_ptr(world, ecs_set(world, 0,
        EcsName, {"ecs_primitive_kind_t", NULL, NULL}),
        EcsMetaType, &__ecs_primitive_kind_t__);

    ecs_set(world, ecs_set(world, 0,
        EcsName, {"ecs_type_kind_t", NULL, NULL}),
        EcsMetaType, {
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
    ECS_COMPONENT_TYPE(world, EcsMetaType);
    ECS_COMPONENT_TYPE(world, ecs_type_op_kind_t);
    ECS_COMPONENT_TYPE(world, ecs_type_op_t);
    ECS_COMPONENT_TYPE(world, EcsMetaTypeSerializer);

    /* -- Initialize metadata for public Flecs core components -- */
    ecs_set(world, ecs_entity(EcsName), EcsMetaType, {
        .kind = EcsStructType, 
        .size = sizeof(EcsName),
        .alignment = ECS_ALIGNOF(EcsName),
        .descriptor = "{ecs_string_t value; ECS_PRIVATE; }"
    });

    ecs_set(world, ecs_entity(EcsComponent), EcsMetaType, {
        .kind = EcsStructType, 
        .size = sizeof(EcsComponent),
        .alignment = ECS_ALIGNOF(EcsComponent),
        .descriptor = "{int32_t size; int32_t alignment;}"
    });

    ecs_set(world, ecs_entity(EcsSignatureExpr), EcsMetaType, {
        .kind = EcsStructType, 
        .size = sizeof(EcsSignatureExpr),
        .alignment = ECS_ALIGNOF(EcsSignatureExpr),
        .descriptor = "{const char *expr;}"
    });
}
