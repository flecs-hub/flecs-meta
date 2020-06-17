#ifndef FLECS_META_SERIALIZER_H
#define FLECS_META_SERIALIZER_H

#include "flecs_meta.h"

void EcsAddStruct(
    ecs_iter_t *it);

void EcsSetPrimitive(
    ecs_iter_t *it);

void EcsSetEnum(
    ecs_iter_t *it);

void EcsSetBitmask(
    ecs_iter_t *it);

void EcsSetStruct(
    ecs_iter_t *it);

void EcsSetArray(
    ecs_iter_t *it);

void EcsSetVector(
    ecs_iter_t *it);

void EcsSetMap(
    ecs_iter_t *it);

#endif
