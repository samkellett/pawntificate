function(add_unit_test)
  cmake_parse_arguments(UNIT_TEST "GTEST" "NAME" "SOURCES;LIBRARIES" ${ARGN})
  add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})

  if(${UNIT_TEST_GTEST})
    list(APPEND UNIT_TEST_LIBRARIES gmock_main)
    target_include_directories(${UNIT_TEST_NAME} PRIVATE
      ${CMAKE_SOURCE_DIR}/ext/googletest/include
      ${CMAKE_SOURCE_DIR}/ext/googlemock/include)
  endif()

  target_link_libraries(${UNIT_TEST_NAME} ${UNIT_TEST_LIBRARIES})
  add_test(NAME ${UNIT_TEST_NAME} COMMAND ${UNIT_TEST_NAME})
endfunction()

add_unit_test(GTEST NAME test_board SOURCES test_board.cpp LIBRARIES pawntificate)
add_unit_test(GTEST NAME test_evaluate SOURCES test_evaluate.cpp LIBRARIES pawntificate)
add_unit_test(GTEST NAME test_find_legal_moves SOURCES test_find_legal_moves.cpp LIBRARIES pawntificate)
add_unit_test(GTEST NAME test_uci_command SOURCES test_uci_command.cpp LIBRARIES pawntificate)
