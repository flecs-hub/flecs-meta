#ifndef FLECS_META_PARSER_H
#define FLECS_META_PARSER_H

#include "flecs_meta.h"

#define ECS_META_IDENTIFIER_LENGTH (256)

#define ecs_meta_error(ctx, ptr, ...)\
    ecs_parser_error((ctx)->name, (ctx)->decl, ptr - (ctx)->decl, __VA_ARGS__)

typedef char ecs_meta_token_t[ECS_META_IDENTIFIER_LENGTH];

typedef struct ecs_meta_parse_ctx_t {
    const char *name;
    const char *decl;
} ecs_meta_parse_ctx_t;

typedef struct ecs_meta_type_t {
    ecs_meta_token_t type;
    ecs_meta_token_t params;
    bool is_const;
    bool is_ptr;
} ecs_meta_type_t;

typedef struct ecs_meta_member_t {
    ecs_meta_type_t type;
    ecs_meta_token_t name;
    int64_t count;
    bool is_partial;
} ecs_meta_member_t;

typedef struct ecs_meta_constant_t {
    ecs_meta_token_t name;
    int64_t value;
    bool is_value_set;
} ecs_meta_constant_t;

typedef struct ecs_meta_params_t {
    ecs_meta_type_t key_type;
    ecs_meta_type_t type;
    int64_t count;
    bool is_key_value;
    bool is_fixed_size;
} ecs_meta_params_t;

const char* ecs_meta_parse_constant(
    const char *ptr,
    ecs_meta_constant_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

const char* ecs_meta_parse_member(
    const char *ptr,
    ecs_meta_member_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

void ecs_meta_parse_params(
    const char *ptr,
    ecs_meta_params_t *token_out,
    ecs_meta_parse_ctx_t *ctx);

#endif
