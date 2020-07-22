#include "galaxy/glyph_decoder.h"
#include "galaxy/linear_encoded_form.h"
#include "galaxy/message.h"
#include "galaxy/message_as_image.h"
#include "galaxy/message_as_text.h"
#include "galaxy/message_decoder.h"

#include "common/stl/base.h"

#include "stb/wrapper.h"

#include <string>

void TestAll(const std::string& filename);
void FindStableOrbits();

namespace {
std::string messages_dir = "../src/mfs/messages/";
}

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

void ProcessGalaxy(const std::string& filename) {
  MessageAsText mt(filename);
  MessageDecoder md;
  Message m = md.Decode(mt);
  m.Process();
}

int main(int argc, char* argv[]) {
  FindStableOrbits();
  ProcessGalaxy(messages_dir + "galaxy.txt");
  TestAll(messages_dir + "test.txt");
  if (argc >= 2) {
    MessageDecoder md;
    Expression e = md.DecodeExpression(argv[1]);
    e.Evaluate();
    e.Print();
    cout << endl;
  }
  // GlyphDecoder gd = GlyphDecoder::GetDecoder();
  //   for (int64_t i = -10; i < 30; ++i) {
  // auto ei = gd.Encode(Glyph(GlyphType::VARIABLE, i));
  //   auto di = gd.Decode(ei);
  //   cout << i << " -> " << ei.mask << " -> " << di.value << endl;
  //     auto ei = LEFEncodeNumber(i);
  //     auto di = LEFDecodeNumber(ei);
  //     cout << i << " -> " << di << endl;
  //   }
  return 0;
}
