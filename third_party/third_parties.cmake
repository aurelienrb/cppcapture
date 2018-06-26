# catch (unit test lib)
if (CMAKE_VERSION VERSION_LESS "3.0")
  # cmake 2.8 does not support INTERFACE targets
  file(WRITE empty.cpp "")
  add_library(catch empty.cpp)
  include_directories(${CMAKE_CURRENT_LIST_DIR}/catch/include)
else()
  add_library(catch INTERFACE)
  target_include_directories(catch INTERFACE ${CMAKE_CURRENT_LIST_DIR}/catch/include)
endif()

# StackWalker (Win32 Stacktrace capture)
if (WIN32)
  add_library(StackWalker
    ${CMAKE_CURRENT_LIST_DIR}/JochenKalmbach.StackWalker/Main/StackWalker/StackWalker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JochenKalmbach.StackWalker/Main/StackWalker/StackWalker.h
  )
  target_include_directories(StackWalker PUBLIC ${CMAKE_CURRENT_LIST_DIR}/JochenKalmbach.StackWalker/Main)
endif()
