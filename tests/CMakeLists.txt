# ebnfparser/tests/CMakeLists.txt

# simple integration tests with ctest

add_test(NAME int_000 COMMAND ebnfparser -h)
set_tests_properties(int_000 PROPERTIES LABELS "ebnfparser;integration;commands;cmds")

add_test(NAME int_001 COMMAND ebnfparse -h)
set_tests_properties(int_001 PROPERTIES LABELS "ebnfparse;integration;commands;cmds")

add_test(NAME int_002 COMMAND ebnfparse -x)
set_tests_properties(int_002 PROPERTIES WILL_FAIL TRUE LABELS "ebnfparse;integration;commands;cmds")

add_test(NAME int_003 COMMAND ebnfparse --print --stats ${CMAKE_SOURCE_DIR}/data/ebnf_gql_variant.txt)
set_tests_properties(int_003 PROPERTIES LABELS "ebnfparse;integration;commands;cmds")

add_test(NAME int_004 COMMAND ebnfparse --convert --print --stats ${CMAKE_SOURCE_DIR}/data/ebnf_gql_variant.txt)
set_tests_properties(int_004 PROPERTIES LABELS "ebnfparse;integration;commands;cmds")


enable_testing()
