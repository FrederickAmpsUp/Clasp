#pragma once

#define CLASP_UTIL_CLASS_PTR(className) using Ptr = std::shared_ptr<className>; \
template<typename ...Args> \
static Ptr make_ptr(Args&& ...args) { return std::make_shared<className>(std::forward<Args>(args)...); }
