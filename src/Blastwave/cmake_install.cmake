# Install script for directory: /Users/claudeapruneau/Documents/GitHub/WAC/Blastwave

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.rootmap;/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave_rdict.pcm")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/claudeapruneau/Documents/GitHub/WAC/lib" TYPE FILE FILES
    "/Users/claudeapruneau/Documents/GitHub/WAC/Blastwave/libBlastwave.rootmap"
    "/Users/claudeapruneau/Documents/GitHub/WAC/Blastwave/libBlastwave_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.dylib")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/Users/claudeapruneau/Documents/GitHub/WAC/lib" TYPE SHARED_LIBRARY FILES "/Users/claudeapruneau/Documents/GitHub/WAC/Blastwave/libBlastwave.dylib")
  if(EXISTS "$ENV{DESTDIR}/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/usr/local/Cellar/root/6.22.06_2/lib/root"
      "$ENV{DESTDIR}/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/claudeapruneau/Documents/GitHub/WAC/Base"
      "$ENV{DESTDIR}/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" -x "$ENV{DESTDIR}/Users/claudeapruneau/Documents/GitHub/WAC/lib/libBlastwave.dylib")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

