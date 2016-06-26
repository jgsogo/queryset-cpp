
#pragma once

#include "spdlog/spdlog.h"

#include "_impl/queryset.h"


template <typename... Args>
using QuerySet = qs::_impl::QuerySet<void, Args...>;
