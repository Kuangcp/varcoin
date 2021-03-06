add_definitions(-DSTATICLIB)

add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../version)

file(GLOB_RECURSE CORE_TESTS core_tests/*)
file(GLOB_RECURSE CRYPTO_TESTS crypto/*)
file(GLOB_RECURSE FUNC_TESTS functional_tests/*)
file(GLOB_RECURSE PERFORMANCE_TESTS performance_tests/*)
file(GLOB_RECURSE CORE_PROXY core_proxy/*)
#file(GLOB_RECURSE UNIT_TESTS unit_tests/*)

source_group(core_tests FILES ${CORE_TESTS})
source_group(crypto_tests FILES ${CRYPTO_TESTS})
source_group(functional_tests FILES ${FUNC_TESTS})
source_group(performance_tests FILES ${PERFORMANCE_TESTS})
source_group(core_proxy FILES ${CORE_PROXY})
#source_group(unit_tests FILES ${UNIT_TESTS})

# add_subdirectory(daemon_tests)

add_library(coretests_lib ${CORE_TESTS})

add_executable(coretests ${CORE_TESTS})
add_executable(crypto-tests ${CRYPTO_TESTS})
add_executable(difficulty-tests difficulty/difficulty.cpp)
add_executable(hash-tests hash/main.cpp)
add_executable(hash-target-tests hash-target.cpp)
add_executable(functional_tests ${FUNC_TESTS})
add_executable(performance_tests ${PERFORMANCE_TESTS})
add_executable(core_proxy ${CORE_PROXY})
#add_executable(unit_tests ${UNIT_TESTS})
add_executable(net_load_tests_clt net_load_tests/clt.cpp)
add_executable(net_load_tests_srv net_load_tests/srv.cpp)

target_link_libraries(core_proxy VarNote_core common crypto upnpc-static ${Boost_LIBRARIES})
target_link_libraries(coretests VarNote_core common crypto ${Boost_LIBRARIES})
target_link_libraries(difficulty-tests VarNote_core)
target_link_libraries(functional_tests VarNote_core wallet common crypto ${Boost_LIBRARIES})
target_link_libraries(hash-tests crypto)
target_link_libraries(hash-target-tests crypto VarNote_core)
target_link_libraries(performance_tests VarNote_core common crypto ${Boost_LIBRARIES})
#target_link_libraries(unit_tests VarNote_core common crypto wallet coretests_lib gtest_main ${Boost_LIBRARIES})
target_link_libraries(net_load_tests_clt VarNote_core common crypto gtest_main ${Boost_LIBRARIES})
target_link_libraries(net_load_tests_srv VarNote_core common crypto gtest_main ${Boost_LIBRARIES})



file(GLOB_RECURSE NODE_RPC_PROXY_TEST node_rpc_proxy_test/*)
source_group(node_rpc_proxy_test FILES ${NODE_RPC_PROXY_TEST})
add_executable(node_rpc_proxy_test ${NODE_RPC_PROXY_TEST})
target_link_libraries(node_rpc_proxy_test node_rpc_proxy VarNote_core common crypto ${Boost_LIBRARIES})


if(NOT MSVC)
  set_property(TARGET gtest gtest_main net_load_tests_clt net_load_tests_srv APPEND_STRING PROPERTY COMPILE_FLAGS " -Wno-undef -Wno-sign-compare")
endif()

add_custom_target(tests DEPENDS coretests difficulty hash performance_tests core_proxy unit_tests node_rpc_proxy_test)
set_property(TARGET coretests crypto-tests functional_tests difficulty-tests gtest gtest_main hash-tests hash-target-tests performance_tests core_proxy tests net_load_tests_clt net_load_tests_srv node_rpc_proxy_test PROPERTY FOLDER "tests")

add_test(coretests coretests --generate_and_play_test_data)
add_test(crypto crypto-tests ${CMAKE_CURRENT_SOURCE_DIR}/crypto/tests.txt)
add_test(difficulty difficulty-tests ${CMAKE_CURRENT_SOURCE_DIR}/difficulty/data.txt)
foreach(hash IN ITEMS fast slow tree extra-blake extra-groestl extra-jh extra-skein)
  add_test(hash-${hash} hash-tests ${hash} ${CMAKE_CURRENT_SOURCE_DIR}/hash/tests-${hash}.txt)
endforeach(hash)
add_test(hash-target hash-target-tests)
#add_test(unit_tests unit_tests)
set_property(TARGET node_rpc_proxy_test PROPERTY OUTPUT_NAME "node_rpc_proxy_testd")
set_property(TARGET performance_tests PROPERTY OUTPUT_NAME "performance_testsd")
#set_property(TARGET unit_tests PROPERTY OUTPUT_NAME "unit_testsd")
set_property(TARGET functional_tests PROPERTY OUTPUT_NAME "functional_testsd")
set_property(TARGET core_proxy PROPERTY OUTPUT_NAME "core_proxyd")
