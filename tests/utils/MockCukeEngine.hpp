#ifndef CUKE_MOCKENGINE_HPP_
#define CUKE_MOCKENGINE_HPP_

#include <gmock/gmock.h>
#include <cucumber-cpp/internal/CukeEngine.hpp>

namespace cucumber {
namespace internal {

class MockCukeEngine : public CukeEngine {
public:
    MOCK_CONST_METHOD1(stepMatches, std::vector<StepMatch>(const std::string & name));
    MOCK_METHOD1(endScenario, void(const tags_type & tags));
    MOCK_METHOD3(invokeStep, void(const std::string & id, const invoke_args_type & args, const invoke_table_type & tableArg));
    MOCK_METHOD1(beginScenario, void(const tags_type & tags));
    MOCK_CONST_METHOD3(snippetText, std::string(const std::string & keyword, const std::string & name, const std::string & multilineArgClass));
};

}
}

#endif
