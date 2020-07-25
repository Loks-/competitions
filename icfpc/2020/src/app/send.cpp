#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "app/send.h"

#include "app/encoder.h"
#include "app/httplib.h"

#include <iostream>
#include <string>

std::string SendRaw(const std::string& s) {
  std::cout << "[RAW SEND] " << s << std::endl;

  std::string server_name = "https://icfpc2020-api.testkontur.ru";
  std::string player_key = "3bd205ec3d2640ac9b73eccecf9d540e";
  std::string post_url = "/aliens/send?apiKey=" + player_key;
  httplib::Client2 client(server_name.c_str());
  const std::shared_ptr<httplib::Response> serverResponse =
      client.Post(post_url.c_str(), s, "text/plain");

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

  std::cout << "[RAW RECEIVED] " << serverResponse->body << std::endl;
  return serverResponse->body;
}

std::string Send(const std::string& s) {
  std::cout << "[SEND] " << s << std::endl;
  auto r = DecodeMessage(SendRaw(EncodeMessage(s)));
  std::cout << "[RECEIVED] " << r << std::endl;
  return r;
}
