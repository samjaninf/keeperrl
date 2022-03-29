#pragma once

#include "furniture_type.h"
#include "attr_type.h"

struct WorkshopInfo {
  FurnitureType SERIAL(furniture);
  string SERIAL(name);
  string SERIAL(verb) = "produces";
  AttrType SERIAL(attr);
  SERIALIZE_ALL(NAMED(furniture), NAMED(name), OPTION(verb), NAMED(attr))
};
