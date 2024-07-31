#include "KeyCommon.h"

#include <stdexcept>

int GetKeySetIndex(KeyType key)
{
  switch (key) {
  case KeyType::LEFT_MOUSE:
    return 0;
  case KeyType::RIGHT_MOUSE:
    return 1;
  case KeyType::KEY_1:
    return 2;
  case KeyType::KEY_2:
    return 3;
  case KeyType::KEY_Q:
    return 4;
  case KeyType::KEY_E:
    return 5;
  case KeyType::KEY_R:
    return 6;
  case KeyType::KEY_T:
    return 7;
  default:
    throw std::invalid_argument("Invalid key type");
  }
}