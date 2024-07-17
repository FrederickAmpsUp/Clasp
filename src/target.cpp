#include <clasp/target.hpp>
#include <boost/dll.hpp>

namespace clasp::target {

BuildTarget::BuildTarget(const std::string& filename) {
    boost::dll::shared_library lib(filename.c_str());

    // TODO: detect build target type and add bytecode interpreter stuff

    auto instantiate = lib.get<ASTVisitorTarget::Ptr()>("instantiate");

    this->visitor_ = instantiate();
}

std::vector<uint8_t> BuildTarget::run(clasp::ast::BaseStatement::Ptr stmt) {
    visitor_->visit(stmt);
    return visitor_->output();
}
}