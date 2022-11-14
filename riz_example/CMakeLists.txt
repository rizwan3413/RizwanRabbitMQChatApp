
###################################
# Libev
###################################
add_definitions("-std=c++17")

add_executable(user1 user1.cpp init.cpp)

add_dependencies(user1 amqpcpp)

target_link_libraries(user1 amqpcpp ev pthread dl ssl)

add_executable(user2 user2.cpp init.cpp)

add_dependencies(user2 amqpcpp)

target_link_libraries(user2 amqpcpp ev pthread dl ssl)

