include(FetchContent)
FetchContent_Declare(
  microtar
  GIT_REPOSITORY https://github.com/rxi/microtar
  GIT_TAG        27076e1b9290e9c7842bb7890a54fcf172406c84
)
FetchContent_MakeAvailable(microtar)
add_library(microtar STATIC ${microtar_SOURCE_DIR}/src/microtar.c)
target_include_directories(microtar PUBLIC ${microtar_SOURCE_DIR}/src)
