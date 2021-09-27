cmake_minimum_required(VERSION 3.10)

project(LFU_Cash)

add_executable(LFU_Cash Cash.cpp)

enable_testing()

add_test(Test1 LFU_Cash ../Tests/Test1.txt ../Tests/Answer1.txt)
add_test(Test2 LFU_Cash ../Tests/Test2.txt ../Tests/Answer2.txt)
add_test(Test3 LFU_Cash ../Tests/Test3.txt ../Tests/Answer3.txt)
