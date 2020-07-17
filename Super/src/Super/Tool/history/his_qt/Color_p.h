#ifndef QCOLOR_PRIVATE_H
#define QCOLOR_PRIVATE_H


#include "../Basic/global.h"
#include "RGB.h"
#include  <string>
#include  <vector>


uint get_rgb_value(const char *name);
bool get_named_rgb(const char *, Rgb_t*);
bool get_named_rgb(const char *, int len, Rgb_t*);
bool get_hex_rgb(const char *, Rgb_t *);
bool get_hex_rgb(const char *, int len, Rgb_t *);
std::vector<std::string> get_colornames();

#endif

