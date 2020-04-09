#ifndef FLECS_COMPONENTS_META_SERIALIZER_H
#define FLECS_COMPONENTS_META_SERIALIZER_H

#include "flecs_components_meta.h"

void EcsAddStruct(
    ecs_rows_t *rows);

void EcsSetPrimitive(
    ecs_rows_t *rows);

void EcsSetEnum(
    ecs_rows_t *rows);

void EcsSetBitmask(
    ecs_rows_t *rows);

void EcsSetStruct(
    ecs_rows_t *rows);

void EcsSetArray(
    ecs_rows_t *rows);

void EcsSetVector(
    ecs_rows_t *rows);

void EcsSetMap(
    ecs_rows_t *rows);

#endif
