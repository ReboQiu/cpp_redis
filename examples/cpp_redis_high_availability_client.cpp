// The MIT License (MIT)
//
// Copyright (c) 2015-2017 Simon Ninon <simon.ninon@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// 编译命令: 
// g++ -I/home/rebo/td2 -I /home/rebo/td2/common/thirdpart/cpp_redis/includes/ -I /home/rebo/td2/common/thirdpart/cpp_redis/tacopie/includes/  -g -std=c++11 -D_SPD_LOG -c -o cpp_redis_high_availability_client.o cpp_redis_high_availability_client.cpp; g++ cpp_redis_high_availability_client.o /home/rebo/td2/common/lib/libcpp_redis.a /home/rebo/td2/common/lib/libutil.a -lpthread -o cpp_redis_high_availability_client
#include <cpp_redis/cpp_redis>

#include <iostream>
#include <../../../util/Properties.h>

#ifdef _WIN32
#include <Winsock2.h>
#endif /* _WIN32 */

int
main(int argc, char **argv) {
#ifdef _WIN32
  //! Windows netword DLL init
  WORD version = MAKEWORD(2, 2);
  WSADATA data;

  if (WSAStartup(version, &data) != 0) {
    std::cerr << "WSAStartup() failure" << std::endl;
    return -1;
  }
#endif /* _WIN32 */

  //! Enable logging
  // cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

  std::string conf_path = CommonUtil::Properties::getPropertyPath();
  if (argc >= 2)
  {
	  conf_path = std::string(argv[1]);
  }
  CommonUtil::PropertiesPtr ptrProperties;
  ptrProperties = new CommonUtil::Properties();
  ptrProperties->load(conf_path);

  INIT_LOG(ptrProperties->getPropertyWithDefault("td2_risk_online_main.log_level", "info"), ptrProperties);

  //! High availability requires at least 2 io service workers
  cpp_redis::network::set_default_nb_workers(2);

  cpp_redis::client client;

  //! Add your sentinels by IP/Host & Port
  client.add_sentinel("10.8.17.42", 26379, 1000);

  //! Call connect with optional timeout
  //! Can put a loop around this until is_connected() returns true.
  client.connect("mymaster", [](const std::string& host, std::size_t port, cpp_redis::connect_state status) {
    if (status == cpp_redis::connect_state::dropped) {
      std::cout << "client disconnected from " << host << ":" << port << std::endl;
    }
  }, 1000, -1, 5000);
  client.auth("test", [](cpp_redis::reply& reply) {
    std::cout << "auth:" << reply << std::endl;
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  LOGTRACE("---------------start: ");
  // same as client.send({ "SET", "hello", "42" }, ...)
  // client.get("hello", [](cpp_redis::reply& reply) {
  client.set("hello", "44", [](cpp_redis::reply& reply) {
          std::cout << "get hello : " << std::endl;
          LOGTRACE("----------------------end: ");
    // if (reply.is_string())
    //   do_something_with_string(reply.as_string())
  });
  client.sync_commit();

  int index = 0;
  while (true) {
  //   LOGTRACE("-------------------");
  //     /*
  //   // same as client.send({ "DECRBY", "hello", 12 }, ...)
  //   LOGTRACE("-------------------");
  //   client.incrby("hello", 12, [](cpp_redis::reply& reply) {
  //     LOGTRACE("incrby hello 12: " << reply);
  //     // std::cout << "incrby hello 12: " << reply << std::endl;
  //     // if (reply.is_integer())
  //     //   do_something_with_integer(reply.as_integer())
  //   });

  //   // same as client.send({ "GET", "hello" }, ...)
  //   client.get("hello", [](cpp_redis::reply& reply) {
  //     LOGTRACE("get hello: " << reply);
  //     // std::cout << "get hello: " << reply << std::endl;
  //     // if (reply.is_string())
  //     //   do_something_with_string(reply.as_string())
  //   });

  //   // commands are pipelined and only sent when client.commit() is called
  //   // client.commit();

  //   // synchronous commit, no timeout
  //   client.sync_commit();
  //   std::cout << std::endl;
  //   std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  //   */

      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      index++;
      LOGTRACE("before index: " << index);
      auto incr    = client.incrby("hello", 1);
      auto get     = client.get("hello");
      client.sync_commit();
      LOGTRACE("incrby get hello: " << incr.get());
      LOGTRACE("get hello: " << get.get());
      LOGTRACE("index: " << index);
      std::cout << std::endl;

  }

#ifdef _WIN32
  WSACleanup();
#endif /* _WIN32 */

  return 0;
}
