function(set_project_warnings project_name)
  option(WARNINGS_AS_ERRORS "Treat compiler warnings as errors" TRUE)

  set(MSVC_WARNINGS
      /W4 # /WX is mad
      /we4289 # variable declared in loop is used outside the loop
      /analyze /permissive- # only standard C++
  )

  set(CLANG_WARNINGS
      -Wall
      -Wextra
      -Wshadow # warn if variable declaration shows one from a parent context
      -Wnon-virtual-dtor # use virtual destructor if class has virtual functions
      -Wold-style-cast
      -Wcast-align
      -Wunused
      -Woverloaded-virtual
      -Wpedantic # only standard C++
      -Wconversion
      -Wsign-conversion
      -Wnull-dereference)

  if(WARNINGS_AS_ERRORS)
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
    # set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
  endif()

  set(GCC_WARNINGS ${CLANG_WARNINGS} -Wmisleading-indentation -Wduplicated-cond
                   -Wduplicated-branches -Wlogical-op -Wuseless-cast)

  if(MSVC)
    set(PROJECT_WARNINGS ${MSVC_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
  else()
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
  endif()

  target_compile_options(${project_name} INTERFACE ${PROJECT_WARNINGS})
endfunction()
