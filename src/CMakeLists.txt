FIND_PACKAGE (Boost 1.41.0 REQUIRED COMPONENTS system)
INCLUDE_DIRECTORIES (SYSTEM ${Boost_INCLUDE_DIR})

ADD_DEFINITIONS (
    -std=c++11
)

ADD_EXECUTABLE (main
    main.cpp
    Attribute.cpp
    Dataset.cpp
    SimpleDataset.cpp
)

