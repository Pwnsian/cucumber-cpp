#ifndef _UTILITY_TYPES_H
#define _UTILITY_TYPES_H

#include <memory>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/move/unique_ptr.hpp>
#include <boost/make_shared.hpp>

//Gherkin-c forward declarations
struct GherkinDocument;
struct FileReader;
struct TokenScanner;
struct TokenMatcher;
struct Builder;
struct Parser;
struct Step;
struct Steps;

namespace cucumber {
namespace internal {

typedef boost::shared_ptr<const GherkinDocument> GherkinDocumentPtr;

}
}

#endif
