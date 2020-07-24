#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "app/send.h"

#include "app/httplib.h"
#include "galaxy/linear_encoded_form.h"
#include "galaxy/message_decoder.h"
#include "galaxy/node.h"

#include <iostream>
#include <string>

std::string Send(const std::string& s) {
  std::string server_name = "https://icfpc2020-api.testkontur.ru";
  std::string player_key = "3bd205ec3d2640ac9b73eccecf9d540e";
  std::string post_url = "/aliens/send?apiKey=" + player_key;

  std::cout << "[SEND] " << s << std::endl;
  MessageDecoder md;
  auto e = md.DecodeExpression(s);
  auto lef = LEFEncodeNode(e.MakeRoot());
  auto post_message = LEFToString(lef);
  // std::cout << "Message = " << post_message << std::endl;

  httplib::Client2 client(server_name.c_str());
  const std::shared_ptr<httplib::Response> serverResponse =
      client.Post(post_url.c_str(), post_message, "text/plain");

  if (!serverResponse) {
    std::cout << "Unexpected server response:\nNo response from server"
              << std::endl;
    return "";
  }

  if (serverResponse->status != 200) {
    std::cout << "Unexpected server response:\nHTTP code: "
              << serverResponse->status
              << "\nResponse body: " << serverResponse->body << std::endl;
    return "";
  }

  // std::cout << "Server response: " << serverResponse->body << std::endl;
  auto lef2 = StringToLEF(serverResponse->body);
  Node* node = LEFDecodeExpression(lef2);
  std::cout << "[RECEIVED] ";
  Print(node);
  std::cout << std::endl;
  return "";
}
