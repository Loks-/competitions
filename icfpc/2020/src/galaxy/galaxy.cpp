#include "galaxy/galaxy.h"

#include "galaxy/message.h"
#include "galaxy/message_as_image.h"
#include "galaxy/message_as_text.h"
#include "galaxy/message_decoder.h"

#include "common/assert_exception.h"

#include <iostream>
#include <string>

namespace galaxy {
namespace {
std::string path_to_messages = "../icfpc/2020/mfs/";
bool galaxy_loaded = false;
};  // namespace

// void ReadPNG(const std::string& filename) {
//   // MessageAsImage::PrintPNG(filename);
//   MessageAsImage mi(filename);
//   // mi.Print();
//   MessageDecoder md(GlyphDecoder::GetDecoder());
//   Message m = md.Decode(mi);
//   m.Print();
// }

// void ReadText(const std::string& filename) {
//   MessageAsText mt(filename);
//   MessageDecoder md(GlyphDecoder::GetDecoder());
//   Message m = md.Decode(mt);
//   m.Print();
//   cout << endl;
//   m.Compress();
//   m.Print();
// }

void LoadGalaxy() {
  if (galaxy_loaded) return;
  MessageAsText mt(path_to_messages + "galaxy.txt");
  MessageDecoder md;
  Message m = md.Decode(mt);
  m.Process();
  galaxy_loaded = true;
}

int TestGalaxy() {
  LoadGalaxy();
  MessageAsText mt(path_to_messages + "test.txt");
  MessageDecoder md;
  Message m = md.Decode(mt);
  for (auto& l : m.v) {
    // l.Print();
    // std::cout << std::endl;
    Assert(l.v.size() == 3);
    Assert(l.v[1].v.size() == 1);
    Assert(l.v[1].v[0].type == GlyphType::EQUALITY);
    l.v[0].Evaluate();
    l.v[2].Evaluate();
    if (l.v[0] != l.v[2]) {
      std::cout << "Expressions are not match:" << std::endl;
      l.v[0].Print();
      std::cout << std::endl;
      l.v[2].Print();
      std::cout << std::endl;
      return -1;
    }
  }
  return 0;
}
}  // namespace galaxy
