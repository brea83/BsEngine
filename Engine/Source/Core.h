#pragma once
#define BIND_EVENT_FUNCTION(x) [this](auto&&... args) -> decltype(auto){ return this->x(std::forward<decltype(args)>(args)...);}