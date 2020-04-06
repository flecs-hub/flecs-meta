#include <flecs_components_meta.h>
#include "parser.h"
#include "serializer.h"

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
    const char *component, 
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

    ecs_vector_t *constants = NULL;
    ecs_def_token_t token;

    while ((ptr = ecs_meta_parse_constants(ptr, &token, &ctx))) {
        char **name = ecs_vector_add(&constants, char*);
        *name = ecs_os_strdup(token.name);
    }

    ecs_entity_t comp = ecs_lookup(world, component);
    ecs_assert(comp != 0, ECS_INTERNAL_ERROR, NULL);
    _ecs_set_ptr(world, e, comp, 0, &(EcsEnum){constants});
}

static
void ecs_set_bitmask(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_set_constants(world, e, "EcsBitmask", type);
}

static
void ecs_set_enum(
    ecs_world_t *world, 
    ecs_entity_t e, 
    EcsType *type) 
{
    ecs_set_constants(world, e, "EcsEnum", type);
}

static
ecs_entity_t ecs_meta_lookup_type(
    ecs_world_t *world,
    ecs_def_token_t *token,
    const char *ptr,
    ecs_meta_parse_ctx_t *ctx)
{
    ecs_assert(world != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(token != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ctx != NULL, ECS_INTERNAL_ERROR, NULL);

    const char *name = ctx->name;
    const char *typename = token->type;
    ecs_entity_t type = 0;

    if (!strcmp(typename, "ecs_vector")) {
        ecs_meta_parse_ctx_t param_ctx = {
            .name = name,
            .decl = token->params
        };

        ecs_def_token_t params_token;
        ecs_meta_parse_params(token->params, &params_token, &param_ctx);

        ecs_entity_t element_type = ecs_meta_lookup_type(
            world, &params_token, token->params, &param_ctx);
            
        if (!element_type) {
            ecs_parser_error(name, ctx->decl, ptr - ctx->decl - 1,
                "unknown element type '%s'", params_token.type);
        }
        
        ecs_entity_t ecs_entity(EcsType) = ecs_lookup(world, "EcsType");
        ecs_entity_t ecs_entity(EcsVector) = ecs_lookup(world, "EcsVector");

        type = ecs_set(world, ecs_set(world, 0, 
            EcsType, {EcsVectorType}),
            EcsVector, { element_type });

    } else {
        if (token->is_ptr && !strcmp(typename, "char")) {
            typename = "string";
        }

        type = ecs_lookup(world, typename);
        if (!type) {
            ecs_parser_error(name, ctx->decl, ptr - ctx->decl - 1, 
                "unknown type '%s'", typename);
        }
    }

    return type;
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

    ecs_meta_parse_ctx_t ctx = {
        .name = name,
        .decl = ptr
    };

    ecs_vector_t *members = NULL;
    ecs_def_token_t token;

    while ((ptr = ecs_meta_parse_struct(ptr, &token, &ctx))) {
        ecs_entity_t type = ecs_meta_lookup_type(world, &token, ptr, &ctx);
        ecs_assert(type != 0, ECS_INTERNAL_ERROR, NULL);

        EcsMember *m = ecs_vector_add(&members, EcsMember);
        m->name = ecs_os_strdup(token.name);
        m->type = type;
    }

    ecs_entity_t ecs_entity(EcsStruct) = ecs_lookup(world, "EcsStruct");
    ecs_assert(ecs_entity(EcsStruct) != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_set(world, e, EcsStruct, {members});
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

    ecs_def_token_t token;
    ecs_meta_parse_params(ptr, &token, &ctx);

    if (!token.count) {
        ecs_parser_error(name, ctx.decl, ptr - ctx.decl - 1, 
            "invalid array size");
    }

    ecs_entity_t el_type = ecs_lookup(world, token.type);
    if (!el_type) {
        ecs_parser_error(name, ctx.decl, ptr - ctx.decl - 1, 
            "unknown element type '%s'", token.type);
    }

    ecs_entity_t ecs_entity(EcsArray) = ecs_lookup(world, "EcsArray");
    ecs_assert(ecs_entity(EcsArray) != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_set(world, e, EcsArray, {el_type, token.count});
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

    ecs_def_token_t token;
    ecs_meta_parse_params(ptr, &token, &ctx);

    ecs_entity_t el_type = ecs_lookup(world, token.type);
    if (!el_type) {
        ecs_parser_error(name, ctx.decl, ptr - ctx.decl - 1, 
            "unknown element type '%s'", token.type);
    }

    ecs_entity_t ecs_entity(EcsVector) = ecs_lookup(world, "EcsVector");
    ecs_assert(ecs_entity(EcsVector) != 0, ECS_INTERNAL_ERROR, NULL);
    ecs_set(world, e, EcsVector, {el_type, token.count});
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
        }
    }
}

void FlecsComponentsMetaImport(
    ecs_world_t *world,
    int flags)
{
    ECS_MODULE(world, FlecsComponentsMeta);

    ECS_COMPONENT(world, EcsPrimitive);
    ECS_COMPONENT(world, EcsEnum);
    ECS_COMPONENT(world, EcsBitmask);
    ECS_COMPONENT(world, EcsStruct);
    ECS_COMPONENT(world, EcsArray);
    ECS_COMPONENT(world, EcsVector);
    ECS_COMPONENT(world, EcsType);
    ECS_COMPONENT(world, EcsTypeSerializer);

    ECS_SYSTEM(world, EcsSetType, EcsOnSet, EcsType);

    ECS_SYSTEM(world, EcsSetPrimitive, EcsOnSet, EcsPrimitive, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetEnum, EcsOnSet, EcsEnum, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetBitmask, EcsOnSet, EcsBitmask, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetStruct, EcsOnSet, EcsStruct, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetArray, EcsOnSet, EcsArray, $.FlecsComponentsMeta);
    ECS_SYSTEM(world, EcsSetVector, EcsOnSet, EcsVector, $.FlecsComponentsMeta);

    ECS_EXPORT_COMPONENT(EcsPrimitive);
    ECS_EXPORT_COMPONENT(EcsEnum);
    ECS_EXPORT_COMPONENT(EcsBitmask);
    ECS_EXPORT_COMPONENT(EcsStruct);
    ECS_EXPORT_COMPONENT(EcsArray);
    ECS_EXPORT_COMPONENT(EcsVector);
    ECS_EXPORT_COMPONENT(EcsType);
    ECS_EXPORT_COMPONENT(EcsTypeSerializer);

    /* -- Initialize builtin primitive types -- */
    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"bool"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsBool});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"char"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsChar});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"uint8_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsU8});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"uint16_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsU16});   

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"uint32_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsU32});   

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"uint64_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsU64});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"int8_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsI8});  

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"int16_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsI16});  

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"int32_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsI32});  

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"int64_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsI64});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"intptr_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsIPtr});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"uintptr_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsUPtr});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"size_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsUPtr});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"float"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsF32});   

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"double"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsF64});   

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"string"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsString});

    ecs_set(world, ecs_set(world, ecs_set(world, 0, 
        EcsId, {"ecs_entity_t"}),
        EcsType, {EcsPrimitiveType}), 
        EcsPrimitive, {EcsEntity});

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

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsPrimitive"}),
        EcsType, &__EcsPrimitive__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsBitmask"}),
        EcsType, &__EcsBitmask__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsEnum"}),
        EcsType, &__EcsEnum__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsMember"}),
        EcsType, &__EcsMember__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsStruct"}),
        EcsType, &__EcsStruct__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsArray"}),
        EcsType, &__EcsArray__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsVector"}),
        EcsType, &__EcsVector__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsType"}),
        EcsType, &__EcsType__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"ecs_type_op_kind_t"}),
        EcsType, &__ecs_type_op_kind_t__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"ecs_type_op_t"}),
        EcsType, &__ecs_type_op_t__);

    ecs_set_ptr(world, ecs_set(world, 0,
        EcsId, {"EcsTypeSerializer"}),
        EcsType, &__EcsTypeSerializer__);
}
