cmake_minimum_required(VERSION 3.10)
project(test_server)

# 添加静态库
add_library(my_httpserver_lib STATIC HttpContext.cpp HttpRequest.cpp HttpResponse.cpp HttpServer.cpp Utils.cpp ../source/Acceptor.cpp ../source/Buffer.cpp 
../source/Channel.cpp ../source/Connection.cpp ../source/EventLoop.cpp ../source/Poller.cpp ../source/Socket.cpp ../source/TcpServer.cpp 
../source/TimeWheel.cpp )


add_compile_options(-g)  # 对所有编译器生效
# 生成可执行文件并链接库
add_executable(test_server main.cpp)
target_link_libraries(test_server my_httpserver_lib)