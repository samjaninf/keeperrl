#pragma once

#include "util.h"
#include "directory_path.h"
#include "pretty_printing.h"
#include "file_path.h"

enum class GameConfigId {
  CAMPAIGN_VILLAINS,
  PLAYER_CREATURES,
  BUILD_MENU,
  WORKSHOPS_MENU,
  IMMIGRATION,
  TECHNOLOGY
};

class GameConfig {
  public:
  GameConfig(DirectoryPath);
  template<typename T>
  optional<string> readObject(T& object, GameConfigId id) {
    return PrettyPrinting::parseObject<T>(object, path.file(getConfigName(id) + ".txt"_s));
  }

  private:
  static const char* getConfigName(GameConfigId);
  DirectoryPath path;
};
