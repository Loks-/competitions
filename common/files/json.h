#pragma once

#include "common/base.h"
#include "common/files/file_to_string.h"

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace files {
class JSON {
 public:
  enum ValueType {
    INTEGER,
    FLOATING,
    STRING,
    BOOLEAN,
    ARRAY,
    DICTIONARY,
    EMPTY,
    END
  };

 protected:
  ValueType type;

  bool value_boolean;
  int value_integer;
  double value_floating;
  std::string value_string;
  std::vector<JSON> value_array;
  std::vector<std::string> value_dictionary_keys;
  std::unordered_map<std::string, unsigned> value_dictionary;

 public:
  JSON()
      : type(EMPTY),
        value_boolean(false),
        value_integer(0),
        value_floating(0.) {}

  explicit JSON(int value) : JSON() {
    type = INTEGER;
    value_integer = value;
  }

  explicit JSON(double value) : JSON() {
    type = FLOATING;
    value_floating = value;
  }

  explicit JSON(const std::string& value) : JSON() {
    type = STRING;
    value_string = value;
  }

  explicit JSON(bool value) : JSON() {
    type = BOOLEAN;
    value_boolean = value;
  }

  explicit JSON(const std::vector<JSON>& value) : JSON() {
    type = ARRAY;
    value_array = value;
  }

  unsigned Size() const {
    switch (type) {
      case INTEGER:
      case FLOATING:
      case STRING:
      case BOOLEAN:
        return 1;
      case ARRAY:
      case DICTIONARY:
        return value_array.size();
      case EMPTY:
        return 0;
      default:
        assert(false);
    }
    return 0;
  }

  ValueType Type() const { return type; }

  int GetInteger() const {
    assert(type == INTEGER);
    return value_integer;
  }

  int GetInteger(unsigned index) const { return GetValue(index).GetInteger(); }

  int GetInteger(const std::string& key) const {
    return GetValue(key).GetInteger();
  }

  double GetFloating() const {
    if (type == INTEGER) return value_integer;
    if (type == FLOATING) return value_floating;
    assert(false);
    return 0;
  }

  const std::string& GetString() const {
    assert(type == STRING);
    return value_string;
  }

  const std::string& GetString(unsigned index) const {
    return GetValue(index).GetString();
  }

  const std::string& GetString(const std::string& key) const {
    return GetValue(key).GetString();
  }

  bool GetBoolean() const {
    assert(type == BOOLEAN);
    return value_boolean;
  }

  JSON& GetValue(unsigned index) {
    assert((type == ARRAY) || (type == DICTIONARY));
    assert(index < value_array.size());
    return value_array[index];
  }

  const JSON& GetValue(unsigned index) const {
    assert((type == ARRAY) || (type == DICTIONARY));
    assert(index < value_array.size());
    return value_array[index];
  }

  bool HasKey(const std::string& key) const {
    return (type == DICTIONARY)
               ? (value_dictionary.find(key) != value_dictionary.end())
               : false;
  }

  const std::string& GetKey(unsigned index) const {
    assert(type == DICTIONARY);
    assert(index < value_dictionary_keys.size());
    return value_dictionary_keys[index];
  }

  JSON& GetValue(const std::string& key) {
    assert(type == DICTIONARY);
    auto it = value_dictionary.find(key);
    assert(it != value_dictionary.end());
    return value_array[it->second];
  }

  const JSON& GetValue(const std::string& key) const {
    assert(type == DICTIONARY);
    auto it = value_dictionary.find(key);
    assert(it != value_dictionary.end());
    return value_array[it->second];
  }

  void SetInteger(int new_value) {
    type = INTEGER;
    value_integer = new_value;
  }

  void SetFloating(double new_value) {
    type = FLOATING;
    value_floating = new_value;
  }

  void SetString(const std::string& new_value) {
    type = STRING;
    value_string = new_value;
  }

  void SetBoolean(bool new_value) {
    type = BOOLEAN;
    value_boolean = new_value;
  }

  void SetArray() {
    type = ARRAY;
    value_array.clear();
  }

  void SetArray(const std::vector<JSON>& new_value) {
    type = ARRAY;
    value_array = new_value;
  }

  void SetDictionary() {
    type = DICTIONARY;
    value_array.clear();
    value_dictionary_keys.clear();
    value_dictionary.clear();
  }

  void SetEmpty() { type = EMPTY; }

  void Add(const JSON& value) {
    assert(type == ARRAY);
    value_array.push_back(value);
  }

  void Add(const std::string& key, const JSON& value) {
    assert(type == DICTIONARY);
    assert(!HasKey(key));
    value_dictionary.insert({key, value_array.size()});
    value_dictionary_keys.push_back(key);
    value_array.push_back(value);
  }

 protected:
  bool ContextAssert(const std::string& s, unsigned pos, char expected) {
    if (pos >= s.size()) {
      std::cerr << "File ended earlier that expected." << std::endl;
      assert(false);
      return false;
    } else if (s[pos] != expected) {
      std::cerr << "Check Failed @ pos = " << pos << std::endl;
      std::cerr << "Expect " << expected << " got " << s[pos] << std::endl;
      std::cerr << "Text before:" << std::endl;
      std::cerr << s.substr(std::max(pos, 200u) - 200, std::min(pos, 200u))
                << std::endl;
      std::cerr << "Text after:" << std::endl;
      std::cerr << s.substr(pos, 100) << std::endl;
      assert(false);
      return false;
    }
    return true;
  }

  void SkipSpaces(const std::string& s, unsigned& index) {
    for (; (index < s.size()) && (isspace(s[index]));) ++index;
  }

  bool ParseI(const std::string& s, unsigned& index) {
    type = END;
    SkipSpaces(s, index);
    assert(index < s.size());
    char f = s[index];
    switch (f) {
      case '{': {
        type = DICTIONARY;
        ++index;
        SkipSpaces(s, index);
        for (;;) {
          if (s[index] == '}') break;
          if (!ContextAssert(s, index, '"')) return false;
          auto npos = s.find('"', index + 1);
          assert(npos != std::string::npos);
          std::string key = s.substr(index + 1, npos - index - 1);
          npos = s.find(':', npos + 1);
          assert(npos != std::string::npos);
          index = npos + 1;
          value_dictionary.insert({key, value_array.size()});
          value_dictionary_keys.push_back(key);
          value_array.push_back({});
          if (!value_array.back().ParseI(s, index)) return false;
          SkipSpaces(s, index);
          if ((index >= s.size()) || (s[index] != ',')) break;
          ++index;
          SkipSpaces(s, index);
        }
        if (!ContextAssert(s, index, '}')) return false;
        ++index;
      } break;
      case '[': {
        type = ARRAY;
        ++index;
        SkipSpaces(s, index);
        for (;;) {
          value_array.push_back({});
          if (!value_array.back().ParseI(s, index)) return false;
          SkipSpaces(s, index);
          if ((index >= s.size()) || (s[index] != ',')) break;
          ++index;
          SkipSpaces(s, index);
        }
        if (!ContextAssert(s, index, ']')) return false;
        ++index;
      } break;
      case '"': {
        type = STRING;
        auto npos = s.find('"', index + 1);
        assert(npos != std::string::npos);
        value_string = s.substr(index + 1, npos - index - 1);
        index = npos + 1;
      } break;
      default: {
        auto npos = std::min(std::min(s.find('\n', index), s.find(',', index)),
                             std::min(s.find(']', index), s.find('}', index)));
        assert(npos != std::string::npos);
        std::string ss = s.substr(index, npos - index);
        if (ss == "true") {
          type = BOOLEAN;
          value_boolean = true;
        } else if (ss == "false") {
          type = BOOLEAN;
          value_boolean = false;
        } else if (ss == "null") {
          type = EMPTY;
        } else {
          char* p = nullptr;
          long ltemp = strtol(ss.c_str(), &p, 10);
          if (!*p) {
            type = INTEGER;
            value_integer = ltemp;
          } else {
            p = nullptr;
            double dtemp = strtod(ss.c_str(), &p);
            if (!*p) {
              type = FLOATING;
              value_floating = dtemp;
            } else {
              std::cerr << "Unknown value: [" << ss << "]" << std::endl;
              if (!ContextAssert(s, npos, '~')) return false;
            }
          }
        }
        index = npos;
      }
    }
    return true;
  }

 public:
  bool Parse(const std::string& full_json) {
    unsigned index = 0;
    return ParseI(full_json, index);
  }

  bool Load(const std::string& filename) {
    std::string sjson = FileToString(filename);
    if (sjson.empty()) return false;
    return Parse(sjson);
  }

  void ToOStream(std::ostream& o) const {
    switch (type) {
      case INTEGER:
        o << value_integer;
        break;
      case FLOATING:
        o << value_floating;
        break;
      case STRING:
        o << "\"" << value_string << "\"";
        break;
      case BOOLEAN:
        o << (value_boolean ? "true" : "false");
        break;
      case ARRAY:
        o << "[";
        if (value_array.size() > 0) {
          value_array[0].ToOStream(o);
          for (unsigned i = 1; i < Size(); ++i) {
            o << ", ";
            value_array[i].ToOStream(o);
          }
        }
        o << "]";
        break;
      case DICTIONARY:
        o << "{";
        if (value_array.size() > 0) {
          o << "\"" << value_dictionary_keys[0] << "\": ";
          value_array[0].ToOStream(o);
          for (unsigned i = 1; i < Size(); ++i) {
            o << ", \"" << value_dictionary_keys[i] << "\": ";
            value_array[i].ToOStream(o);
          }
        }
        o << "}";
        break;
      case EMPTY:
        o << "null";
        break;
      default:
        assert(false);
        break;
    }
  }

  std::string ToString() const {
    std::ostringstream oss;
    ToOStream(oss);
    return oss.str();
  }

  void Save(const std::string& filename) const {
    std::ofstream f(filename);
    ToOStream(f);
  }
};
}  // namespace files
