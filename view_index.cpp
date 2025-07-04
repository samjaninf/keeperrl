/* Copyright (C) 2013-2014 Michal Brzozowski (rusolis@poczta.fm)

   This file is part of KeeperRL.

   KeeperRL is free software; you can redistribute it and/or modify it under the terms of the
   GNU General Public License as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   KeeperRL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
   even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with this program.
   If not, see http://www.gnu.org/licenses/ . */

#include "stdafx.h"

#include "view_index.h"
#include "view_object.h"

optional<TStringId> getDescription(HighlightType type) {
  switch (type) {
    case HighlightType::STORAGE_EQUIPMENT:
      return TStringId("EQUIPMENT_STORAGE_HIGHLIGHT");
    case HighlightType::STORAGE_RESOURCES:
      return TStringId("RESOURCE_STORAGE_HIGHLIGHT");
     case HighlightType::LEISURE:
       return TStringId("MINION_LEISURE_ZONE_HIGHLIGHT");
     case HighlightType::FETCH_ITEMS:
       return TStringId("FETCHING_ITEMS_HIGHLIGHT");
     case HighlightType::PERMANENT_FETCH_ITEMS:
       return TStringId("PERSISTENT_ITEM_FETCHING_ZONE_HIGHLIGHT");
     case HighlightType::GUARD_ZONE1:
       return TStringId("GUARD_ZONE1_HIGHLIGHT");
     case HighlightType::GUARD_ZONE2:
       return TStringId("GUARD_ZONE2_HIGHLIGHT");
     case HighlightType::GUARD_ZONE3:
       return TStringId("GUARD_ZONE3_HIGHLIGHT");
     case HighlightType::FORBIDDEN_ZONE:
       return TStringId("FORBIDDEN_ZONE_HIGHLIGHT");
     case HighlightType::ALLIED_TOTEM:
       return TStringId("ALLIED_MAGICAL_FIELD_HIGHLIGHT");
     case HighlightType::HOSTILE_TOTEM:
       return TStringId("HOSTILE_MAGICAL_FIELD_HIGHLIGHT");
     case HighlightType::CUT_TREE:
       return TStringId("TREE_CUTTING_TASK_HIGHLIGHT");
     case HighlightType::DIG:
       return TStringId("DIGGING_TASK_HIGHLIGHT");
     case HighlightType::INSUFFICIENT_LIGHT:
       return TStringId("INSUFFICIENT_LIGHT_HIGHLIGHT");
     case HighlightType::PRISON_NOT_CLOSED:
       return TStringId("PRISON_MUST_BE_SEPARATE_HIGHLIGHT");
     case HighlightType::PIGSTY_NOT_CLOSED:
       return TStringId("ANIMAL_PEN_MUST_BE_SEPARATE_HIGHLIGHT");
     case HighlightType::TORTURE_UNAVAILABLE:
       return TStringId("TORTURE_UNAVAILABLE_HIGHLIGHT");
     case HighlightType::PRIORITY_TASK:
       return TStringId("PRIORITY_TASK_HIGHLIGHT");
    default:
      return none;
  }
}

ViewId getViewId(HighlightType id, bool active) {
  switch (id) {
   case HighlightType::QUARTERS:
      return ViewId("quarters", Color(255, 20, 147));
    case HighlightType::LEISURE:
      return ViewId("quarters", Color(50, 50, 200));
    case HighlightType::FETCH_ITEMS:
    case HighlightType::PERMANENT_FETCH_ITEMS:
      return ViewId("fetch_icon");
    case HighlightType::STORAGE_EQUIPMENT:
      return ViewId("storage_equipment");
    case HighlightType::STORAGE_RESOURCES:
      return ViewId("storage_resources");
    case HighlightType::GUARD_ZONE1:
      return ViewId("guard_zone");
    case HighlightType::GUARD_ZONE2:
      return ViewId("guard_zone", Color::PURPLE);
    case HighlightType::GUARD_ZONE3:
      return ViewId("guard_zone", Color::SKY_BLUE);
    case HighlightType::FORBIDDEN_ZONE:
      return ViewId("dig_mark", Color(255, 0, 0, 120));
    case HighlightType::ALLIED_TOTEM:
      return ViewId("magic_field", Color::GREEN);
    case HighlightType::HOSTILE_TOTEM:
      return ViewId("magic_field", Color::RED);
    case HighlightType::CLICKABLE_FURNITURE:
      return ViewId("dig_mark", Color(255, 255, 0, 120));
    case HighlightType::CLICKED_FURNITURE:
      return ViewId("dig_mark", Color(255, 255, 0));
    case HighlightType::CREATURE_DROP:
      return ViewId("dig_mark", Color(0, 255, 0));
    case HighlightType::CUT_TREE:
      return ViewId("dig_mark", Color::YELLOW.transparency(200));
    case HighlightType::DIG:
      return ViewId("dig_mark", Color::YELLOW.transparency(100));
    case HighlightType::INDOORS:
      return ViewId("dig_mark2", Color(0, 0, 255, active ? 40 : 0));
    case HighlightType::INSUFFICIENT_LIGHT:
      return ViewId("dig_mark2", Color(255, 0, 0, active ? 200 : 0));
    case HighlightType::MEMORY:
      return ViewId("tile_below", Color(0, 0, 0, 80));
    case HighlightType::PRISON_NOT_CLOSED:
    case HighlightType::PIGSTY_NOT_CLOSED:
      return ViewId("dig_mark", Color(255, 0, 0));
    case HighlightType::RECT_SELECTION:
      return ViewId("dig_mark", Color::YELLOW.transparency(100));
    case HighlightType::RECT_DESELECTION:
      return ViewId("dig_mark", Color::RED.transparency(100));
    case HighlightType::PRIORITY_TASK:
      return ViewId("dig_mark", Color(0, 255, 0, 200));
    case HighlightType::TILE_BELOW:
      return ViewId("tile_below", Color(0, 0, 0, 120));
    case HighlightType::UNAVAILABLE:
      return ViewId("dig_mark2", Color(0, 0, 0, 120));
    case HighlightType::TORTURE_UNAVAILABLE:
      return ViewId("dig_mark", Color(255, 0, 0));
  }
}

SERIALIZE_DEF(ViewIndex, objIndex, highlights, tileGas, objects, anyHighlight, itemCounts, nightAmount)

ViewIndex::ViewIndex() {
  for (auto& elem : objIndex)
    elem = 100;
}

ViewIndex::~ViewIndex() {
}

void ViewIndex::insert(ViewObject obj) {
  PROFILE;
  int ind = objIndex[int(obj.layer())];
  if (ind < 100)
    objects[ind] = std::move(obj);
  else {
    objIndex[int(obj.layer())] = objects.size();
    objects.push_back(std::move(obj));
  }
}

bool ViewIndex::hasObject(ViewLayer l) const {
  return objIndex[int(l)] < 100;
}

void ViewIndex::removeObject(ViewLayer l) {
  objIndex[int(l)] = 100;
}

bool ViewIndex::isEmpty() const {
  return objects.empty() && !anyHighlight;
}

bool ViewIndex::hasAnyHighlight() const {
  return anyHighlight || !tileGas.empty();
}

bool ViewIndex::noObjects() const {
  PROFILE;
  return objects.empty();
}

const ViewObject& ViewIndex::getObject(ViewLayer l) const {
  int ind = objIndex[int(l)];
  CHECK(ind >= 0 && ind < objects.size()) << "No object on layer " << int(l) << " " << ind;
  return objects[ind];
}

ViewObject& ViewIndex::getObject(ViewLayer l) {
  int ind = objIndex[int(l)];
  CHECK(ind < 100) << "No object on layer " << int(l);
  return objects[ind];
}

const ViewObject* ViewIndex::getTopObject(const vector<ViewLayer>& layers) const {
  for (int i = layers.size() - 1; i >= 0; --i)
    if (hasObject(layers[i]))
      return &getObject(layers[i]);
  return nullptr;
}
void ViewIndex::setNightAmount(double amount) {
  nightAmount = (std::uint8_t) trunc(amount * 255);
}

double ViewIndex::getNightAmount() const {
  return double(nightAmount) / 255.0;
}

void ViewIndex::addGasAmount(TString name, Color color) {
  tileGas.push_back(TileGasInfo{std::move(color), std::move(name)});
}

const vector<ViewIndex::TileGasInfo>& ViewIndex::getGasAmounts() const {
  return tileGas;
}

const static ItemCounts emptyCounts;

const ItemCounts& ViewIndex::getItemCounts() const {
  if (itemCounts)
    return itemCounts->first;
  else
    return emptyCounts;
}

const ItemCounts& ViewIndex::getEquipmentCounts() const {
  if (itemCounts)
    return itemCounts->second;
  else
    return emptyCounts;
}

ItemCounts& ViewIndex::modItemCounts() {
  if (!itemCounts)
    itemCounts.reset(make_pair(ItemCounts(), ItemCounts()));
  return itemCounts->first;
}

ItemCounts& ViewIndex::modEquipmentCounts() {
  if (!itemCounts)
    itemCounts.reset(make_pair(ItemCounts(), ItemCounts()));
  return itemCounts->second;
}

void ViewIndex::setHighlight(HighlightType h, bool state) {
  if (state)
    anyHighlight = true;
  highlights.set(h, state);
}

bool ViewIndex::isHighlight(HighlightType h) const {
  return highlights.contains(h);
}

optional<ViewId> ViewIndex::getHiddenId() const {
  return hiddenId;
}

void ViewIndex::setHiddenId(ViewId id) {
  hiddenId = id;
}

vector<ViewObject>& ViewIndex::getAllObjects() {
  return objects;
}

const vector<ViewObject>& ViewIndex::getAllObjects() const {
  return objects;
}

void ViewIndex::mergeFromMemory(const ViewIndex& memory) {
  if (isEmpty())
    *this = memory;
  else if (!hasObject(ViewLayer::FLOOR) && !hasObject(ViewLayer::FLOOR_BACKGROUND) && !isEmpty()) {
    // special case when monster or item is visible but floor is only in memory
    if (memory.hasObject(ViewLayer::FLOOR))
      insert(memory.getObject(ViewLayer::FLOOR));
    if (memory.hasObject(ViewLayer::FLOOR_BACKGROUND))
      insert(memory.getObject(ViewLayer::FLOOR_BACKGROUND));
  }
}

void ViewIndex::mergeGroundBelow(const ViewIndex& memory, int height) {
  if (isEmpty()) {
    *this = memory;
    removeObject(ViewLayer::TORCH1);
    removeObject(ViewLayer::TORCH2);
    this->height = height;
    setHighlight(HighlightType::TILE_BELOW);
  }
}
