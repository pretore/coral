cmake_minimum_required(VERSION 3.17)
project(coral VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
include(cmake/FetchCMocka.cmake)

# Sources
set(EXPORTED_HEADER_FILES
        include/coral/array.h
        include/coral/autorelease_pool.h
        include/coral/class.h
        include/coral/context.h
        include/coral/error.h
        include/coral/interface.h
        include/coral/lock.h
        include/coral/lock_condition.h
        include/coral/notification.h
        include/coral/object.h
        include/coral/range.h
        include/coral/reference.h
        include/coral/string.h
        include/coral/weak_reference.h
        include/coral.h)
set(SOURCES
        ${EXPORTED_HEADER_FILES}
        src/private/array.h
        src/private/autorelease_pool.h
        src/private/class.h
        src/private/context.h
        src/private/coral.h
        src/private/interface.h
        src/private/lock.h
        src/private/lock_condition.h
        src/private/object.h
        src/private/range.h
        src/private/red_black_tree.h
        src/private/reference.h
        src/private/scope.h
        src/private/string.h
        src/private/tree_map.h
        src/private/tree_set.h
        src/private/weak_reference.h
        src/array.c
        src/autorelease_pool.c
        src/class.c
        src/context.c
        src/coral.c
        src/error.c
        src/interface.c
        src/lock.c
        src/lock_condition.c
        src/notification.c
        src/object.c
        src/range.c
        src/red_black_tree.c
        src/reference.c
        src/scope.c
        src/string.c
        src/tree_map.c
        src/tree_set.c
        src/weak_reference.c)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Static Library
    add_library(coral STATIC "")
    add_compile_definitions(TEST)
    target_sources(coral
            PRIVATE
                ${SOURCES}
                src/test/cmocka.h
                src/test/wrap.c
                src/test/wrap.h)
    target_link_libraries(coral
            PUBLIC
                m
                ${CMAKE_THREAD_LIBS_INIT}
                ${CMOCKA_SHARED_LIBRARY})
    target_include_directories(coral
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    set(LINK_FLAGS "-Wl,--wrap=pthread_rwlock_init,--wrap=pthread_rwlock_destroy,--wrap=pthread_rwlock_rdlock,--wrap=pthread_rwlock_wrlock,--wrap=pthread_rwlock_unlock,--wrap=pthread_mutex_init,--wrap=pthread_mutex_destroy,--wrap=pthread_mutex_lock,--wrap=pthread_mutex_unlock,--wrap=pthread_cond_init,--wrap=pthread_cond_destroy,--wrap=pthread_cond_wait,--wrap=pthread_cond_broadcast,--wrap=pthread_cond_signal,--wrap=nanosleep")
    # Unit Tests
    enable_testing()
    # coral-unit-test
    add_executable(coral-unit-test test/test_coral.c)
    target_include_directories(coral-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(coral-unit-test
            PRIVATE
                coral)
    set_target_properties(coral-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(coral-unit-test coral-unit-test)
    # autorelease-pool-unit-test
    add_executable(autorelease-pool-unit-test test/test_autorelease_pool.c)
    target_include_directories(autorelease-pool-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(autorelease-pool-unit-test
            PRIVATE
                coral)
    set_target_properties(autorelease-pool-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(autorelease-pool-unit-test autorelease-pool-unit-test)
    # object-unit-test
    add_executable(object-unit-test test/test_object.c)
    target_include_directories(object-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(object-unit-test
            PRIVATE
                coral)
    set_target_properties(object-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(object-unit-test object-unit-test)
    # interface-unit-test
    add_executable(interface-unit-test test/test_interface.c)
    target_include_directories(interface-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(interface-unit-test
            PRIVATE
                coral)
    set_target_properties(interface-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(interface-unit-test interface-unit-test)
    # class-unit-test
    add_executable(class-unit-test test/test_class.c)
    target_include_directories(class-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(class-unit-test
            PRIVATE
                coral)
    set_target_properties(object-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(class-unit-test class-unit-test)
    # string-unit-test
    add_executable(string-unit-test test/test_string.c)
    target_include_directories(string-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(string-unit-test
            PRIVATE
                coral)
    set_target_properties(string-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(string-unit-test string-unit-test)
    # reference-unit-test
    add_executable(reference-unit-test test/test_reference.c)
    target_include_directories(reference-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(reference-unit-test
            PRIVATE
                coral)
    set_target_properties(reference-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(reference-unit-test reference-unit-test)
    # weak-reference-unit-test
    add_executable(weak-reference-unit-test test/test_weak_reference.c)
    target_include_directories(weak-reference-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(weak-reference-unit-test
            PRIVATE
                coral)
    set_target_properties(weak-reference-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(weak-reference-unit-test weak-reference-unit-test)
    # lock-unit-test
    add_executable(lock-unit-test test/test_lock.c)
    target_include_directories(lock-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(lock-unit-test
            PRIVATE
                coral)
    set_target_properties(lock-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(lock-unit-test lock-unit-test)
    # lock-condition-unit-test
    add_executable(lock-condition-unit-test test/test_lock_condition.c)
    target_include_directories(lock-condition-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(lock-condition-unit-test
            PRIVATE
                coral)
    set_target_properties(lock-condition-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(lock-condition-unit-test lock-condition-unit-test)
    # context-unit-test
    add_executable(context-unit-test test/test_context.c)
    target_include_directories(context-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(context-unit-test
            PRIVATE
                coral)
    set_target_properties(context-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(context-unit-test context-unit-test)
    # range-unit-test
    add_executable(range-unit-test test/test_range.c)
    target_include_directories(range-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(range-unit-test
            PRIVATE
                coral)
    set_target_properties(range-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(range-unit-test range-unit-test)
    # scope-unit-test
    add_executable(scope-unit-test test/test_scope.c)
    target_include_directories(scope-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(scope-unit-test
            PRIVATE
                coral)
    set_target_properties(scope-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(scope-unit-test scope-unit-test)
    # array-unit-test
    add_executable(array-unit-test test/test_array.c)
    target_include_directories(array-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(array-unit-test
            PRIVATE
                coral)
    set_target_properties(array-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(array-unit-test array-unit-test)
    # red-black-tree-unit-test
    add_executable(red-black-tree-unit-test test/test_red_black_tree.c)
    target_include_directories(red-black-tree-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(red-black-tree-unit-test
            PRIVATE
                coral)
    set_target_properties(red-black-tree-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(red-black-tree-unit-test red-black-tree-unit-test)
    # tree-set-unit-test
    add_executable(tree-set-unit-test test/test_tree_set.c)
    target_include_directories(tree-set-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(tree-set-unit-test
            PRIVATE
                coral)
    set_target_properties(tree-set-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(tree-set-unit-test tree-set-unit-test)
    # tree-map-unit-test
    add_executable(tree-map-unit-test test/test_tree_map.c)
    target_include_directories(tree-map-unit-test
            PRIVATE
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")
    target_link_libraries(tree-map-unit-test
            PRIVATE
                coral)
    set_target_properties(tree-map-unit-test
            PROPERTIES
                LINK_FLAGS ${LINK_FLAGS})
    add_test(tree-map-unit-test tree-map-unit-test)
else()
    # Shared Library
    add_library(coral SHARED "")
    target_sources(coral
            PRIVATE
                ${SOURCES})
    target_include_directories(coral
            PUBLIC
                "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
    target_link_libraries(coral
            PRIVATE
                m
                ${CMAKE_THREAD_LIBS_INIT})
endif()
