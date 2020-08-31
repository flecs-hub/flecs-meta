#include "parser.h"
#include <stdio.h>
#include <ctype.h>

static
const char* skip_ws(const char *ptr) {
    while (isspace(*ptr)) {
        ptr ++;
    }

    return ptr;
}

static
const char* skip_scope(const char *ptr, ecs_meta_parse_ctx_t *ctx) {
    /* Keep track of which characters were used to open the scope */
    char stack[256];
    int32_t sp = 0;
    char ch;

    while ((ch = *ptr)) {
        if (ch == '(') {
            stack[sp++] = ch;
        } else if (ch == '<') {
            stack[sp++] = ch;
        } else if (ch == '>') {
            if (stack[--sp] != '<') {
                ecs_meta_error(ctx, ptr, "mismatching < > in type definition");
            }
        } else if (ch == ')') {
            if (stack[--sp] != '(') {
                ecs_meta_error(ctx, ptr, "mismatching ( ) in type definition");
            }            
        }

        ptr ++;

        if (!sp) {
            break;
        }
    }

    return ptr;
}

static
const char* parse_digit(
    const char *ptr,
    int64_t *value_out,
    ecs_meta_parse_ctx_t *ctx)
{
    ptr = skip_ws(ptr);

    if (!isdigit(*ptr) && *ptr != '-') {
        ecs_meta_error(ctx, ptr, "expected number, got %c", *ptr);
    }

    *value_out = strtol(ptr, NULL, 0);

    if (ptr[0] == '-') {
        ptr ++;
    } else 
    if (ptr[0] == '0' && ptr[1] == 'x') {
        ptr += 2;
    }

    while (isdigit(*ptr)) {
        ptr ++;
    }

    return skip_ws(ptr);
}

static
const char* parse_identifier(
    const char *ptr, 
    char *buff,
    char *params,
    ecs_meta_parse_ctx_t *ctx) 
{
    ecs_assert(ptr != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(buff != NULL, ECS_INTERNAL_ERROR, NULL);
    ecs_assert(ctx != NULL, ECS_INTERNAL_ERROR, NULL);

    char *bptr = buff, ch;

    if (params) {
        params[0] = '\0';
    }

    /* Ignore whitespaces */
    ptr = skip_ws(ptr);

    if (!isalpha(*ptr)) {
        ecs_meta_error(ctx, ptr, 
            "invalid identifier (starts with '%c')", *ptr);
    }

    while ((ch = *ptr) && !isspace(ch) && ch != ';' && ch != ',' && ch != ')' && ch != '>') {
        /* Type definitions can contain macro's or templates */
        if (ch == '(' || ch == '<') {
            if (!params) {
                ecs_meta_error(ctx, ptr, "unexpected %c", *ptr);
            }

            const char *end = skip_scope(ptr, ctx);
            ecs_os_strncpy(params, ptr, (ecs_size_t)(end - ptr));
            params[end - ptr] = '\0';

            ptr = end;
        } else {
            *bptr = ch;
            bptr ++;
            ptr ++;
        }
    }

    *bptr = '\0';

    if (!ch) {
        ecs_meta_error(ctx, ptr, "unexpected end of token");
    }

    return ptr;
}

static
const char * ecs_meta_open_scope(
    const char *ptr,
    ecs_meta_parse_ctx_t *ctx)    
{
    /* Skip initial whitespaces */
    ptr = skip_ws(ptr);

    /* Is this the start of the type definition? */
    if (ctx->decl == ptr) {
        if (*ptr != '{') {
            ecs_meta_error(ctx, ptr, "missing '{' in struct definition");     
        }

        ptr ++;
        ptr = skip_ws(ptr);
    }

    /* Is this the end of the type definition? */
    if (!*ptr) {
        ecs_meta_error(ctx, ptr, "missing '}' at end of struct definition");
    }   

    /* Is this the end of the type definition? */
    if (*ptr == '}') {
        ptr = skip_ws(ptr + 1);
        if (*ptr) {
            ecs_meta_error(ctx, ptr, 
                "stray characters after struct definition");
        }
        return NULL;
    }

    return ptr;
}

const char* ecs_meta_parse_constant(
    const char *ptr,
    ecs_meta_constant_t *token,
    ecs_meta_parse_ctx_t *ctx)
{    
    ptr = ecs_meta_open_scope(ptr, ctx);
    if (!ptr) {
        return NULL;
    }

    token->is_value_set = false;

    /* Parse token, constant identifier */
    ptr = parse_identifier(ptr, token->name, NULL, ctx);
    ptr = skip_ws(ptr);

    /* Explicit value assignment */
    if (*ptr == '=') {
        int64_t value = 0;
        ptr = parse_digit(ptr + 1, &value, ctx);
        token->value = value;
        token->is_value_set = true;
    }

    /* Expect a ',' or '}' */
    if (*ptr != ',' && *ptr != '}') {
        ecs_meta_error(ctx, ptr, "missing , after enum constant");
    }

    if (*ptr == ',') {
        return ptr + 1;
    } else {
        return ptr;
    }
}

static
const char* ecs_meta_parse_type(
    const char *ptr,
    ecs_meta_type_t *token,
    ecs_meta_parse_ctx_t *ctx)
{
    token->is_ptr = false;
    token->is_const = false;

    ptr = skip_ws(ptr);

    /* Parse token, expect type identifier or ECS_PROPERTY */
    ptr = parse_identifier(ptr, token->type, token->params, ctx);

    if (!strcmp(token->type, "ECS_PRIVATE")) {
        /* Members from this point are not stored in metadata */
        return NULL;
    }

    /* If token is const, set const flag and continue parsing type */
    if (!strcmp(token->type, "const")) {
        token->is_const = true;

        /* Parse type after const */
        ptr = parse_identifier(ptr + 1, token->type, token->params, ctx);
    }

    /* Check if type is a pointer */
    ptr = skip_ws(ptr);
    if (*ptr == '*') {
        token->is_ptr = true;
        ptr ++;
    }

    return ptr;
}

const char* ecs_meta_parse_member(
    const char *ptr,
    ecs_meta_member_t *token,
    ecs_meta_parse_ctx_t *ctx)
{
    ptr = ecs_meta_open_scope(ptr, ctx);
    if (!ptr) {
        return NULL;
    }

    token->count = 1;
    token->is_partial = false;

    /* Parse member type */
    ptr = ecs_meta_parse_type(ptr, &token->type, ctx);
    if (!ptr) {
        /* If NULL is returned, parsing should stop */
        token->is_partial = true;
        return NULL;
    }

    /* Next token is the identifier */
    ptr = parse_identifier(ptr, token->name, NULL, ctx);

    /* Skip whitespace between member and [ or ; */
    ptr = skip_ws(ptr);

    /* Check if this is an array */
    char *array_start = strchr(token->name, '[');
    if (!array_start) {
        /* If the [ was separated by a space, it will not be parsed as part of
         * the name */
        if (*ptr == '[') {
            array_start = (char*)ptr; /* safe, will not be modified */
        }
    }

    if (array_start) {
        /* Check if the [ matches with a ] */
        char *array_end = strchr(array_start, ']');
        if (!array_end) {
            ecs_meta_error(ctx, ptr, "missing ']'");

        } else if (array_end - array_start == 0) {
            ecs_meta_error(ctx, ptr, "dynamic size arrays are not supported");
        }

        token->count = atoi(array_start + 1);

        if (array_start == ptr) {
            /* If [ was found after name, continue parsing after ] */
            ptr = array_end + 1;
        } else {
            /* If [ was fonud in name, replace it with 0 terminator */
            array_start[0] = '\0';
        }
    }

    /* Expect a ; */
    if (*ptr != ';') {
        ecs_meta_error(ctx, ptr, "missing ; after member declaration");
    }

    return ptr + 1;
}

void ecs_meta_parse_params(
    const char *ptr,
    ecs_meta_params_t *token,
    ecs_meta_parse_ctx_t *ctx)
{
    token->is_key_value = false;
    token->is_fixed_size = false;

    ptr = skip_ws(ptr);
    if (*ptr != '(' && *ptr != '<') {
        ecs_meta_error(ctx, ptr, 
            "expected '(' at start of collection definition");
    }

    ptr ++;

    /* Parse type identifier */
    ptr = ecs_meta_parse_type(ptr, &token->type, ctx);
    ptr = skip_ws(ptr);
    
    /* If next token is a ',' the first type was a key type */
    if (*ptr == ',') {
        ptr = skip_ws(ptr + 1);
        
        if (isdigit(*ptr)) {
            int64_t value;
            ptr = parse_digit(ptr, &value, ctx);
            token->count = value;
            token->is_fixed_size = true;
        } else {
            token->key_type = token->type;

            /* Parse element type */
            ptr = ecs_meta_parse_type(ptr, &token->type, ctx);
            ptr = skip_ws(ptr);

            token->is_key_value = true;
        }
    }

    if (*ptr != ')' && *ptr != '>') {
        ecs_meta_error(ctx, ptr, 
            "expected ')' at end of collection definition");
    }
}
