include(FetchContent)

# SFML
FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.2
  GIT_SHALLOW ON
)

message(STATUS "Fetching SFML...")
FetchContent_MakeAvailable(SFML)


