#ifndef FLECS_COMPONENTS_META_PARSER_H
#define FLECS_COMPONENTS_META_PARSER_H

#include "flecs_components_meta.h"

#define ECS_META_IDENTIFIER_LENGTH (256)

typedef struct ecs_meta_parse_ctx_t {
    const char *name;
    const char *decl;
} ecs_meta_parse_ctx_t;

typedef struct ecs_def_token_t {
    char type[ECS_META_IDENTIFIER_LENGTH];
    char name[ECS_META_IDENTIFIER_LENGTH];
    bool is_const;
    bool is_ptr;
} ecs_def_token_t;

const char* ecs_meta_parse_enum(
    const char *ptr,
    ecs_def_token_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

const char* ecs_meta_parse_struct(
    const char *ptr,
    ecs_def_token_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

void ecs_meta_parse_err(
    const char *decl,
    const char *ptr,
    const char *fmt,
    ...);

#endif
