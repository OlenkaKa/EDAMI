FIND_PACKAGE (Boost 1.41.0 REQUIRED COMPONENTS system program_options regex)
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR})

ADD_DEFINITIONS (
    -std=c++11
)

ADD_EXECUTABLE (main
        main.cpp
        Attribute.cpp
        SimpleDataset.cpp
        Dataset.cpp
        GranuleCalculator.cpp
        MinimalSizeStrategy.cpp
        NormalizationParams.cpp
        SimpleCrossValidation.cpp
        LeaveOneOutValidation.cpp
        GranuleSet.cpp
        SimpleGranuleSet.cpp
        GranularReflectionCreator.cpp
        MaximalSizeStrategy.cpp
        Classifier.cpp
        AverageSizeStrategy.cpp
        AverageGranuleSizeSequenceGenerator.cpp
        GranuleSizeStrategy.cpp
        DatasetInfo.cpp
        DatasetInfoProvider.cpp
        ConfusionMatrix.cpp
        ClassificationInfo.cpp
        FileWriter.cpp)

TARGET_LINK_LIBRARIES(main ${Boost_LIBRARIES})
