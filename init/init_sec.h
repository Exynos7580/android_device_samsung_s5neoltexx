#ifndef INIT_SEC_H
#define INIT_SEC_H

#include <string.h>

enum device_variant {
    VARIANT_A310F = 0,
    VARIANT_A310M,
    VARIANT_A310Y,
    VARIANT_A310N0,
    VARIANT_MAX
};

typedef struct {
    std::string model;
    std::string codename;
} variant;

static const variant international_models = {
    .model = "SM-A310F",
    .codename = "a3xeltexx"
};

static const variant latin_america_models = {
    .model = "SM-A310M",
    .codename = "a3xelteub"
};

static const variant oceania_models = {
    .model = "SM-A310Y",
    .codename = "a3xeltedo"
};

static const variant china_models = {
    .model = "SM-A310N0",
    .codename = "a3xeltekx"
};

static const variant *all_variants[VARIANT_MAX] = {
    &international_models,
    &latin_america_models,
    &oceania_models,
    &china_models,
};

#endif // INIT_SEC_H
