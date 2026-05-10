// Copyright (c) 2026 guyuemeng
//
// cxxaux is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan
// PSL v2. You may obtain a copy of Mulan PSL v2 at:
//             http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.

#ifndef ESTD___RANGES_VIEWS_H
#define ESTD___RANGES_VIEWS_H

/**
 * @defgroup ranges C++20-like ranges views
 *
 * Composable range adaptors inspired by C++20 std::views.
 * All views are lazy and can be composed using the pipe operator.
 *
 * Available views:
 * - **all_view / ref_view**: Wrap a range as a view
 * - **filter_view**: Filter elements by predicate
 * - **transform_view**: Transform elements by function
 * - **take_view**: Take first N elements
 * - **subrange**: Non-owning view over a range
 * - **owning_view**: Owning view over a moved range
 *
 * Example usage:
 * @code
 * std::vector<int> v = {1, 2, 3, 4, 5};
 * auto result = v | views::filter([](int x) { return x > 2; })
 *                | views::transform([](int x) { return x * 2; })
 *                | views::take(2);
 * @endcode
 * @{
 */

#include "all_view.h"
#include "filter_view.h"
#include "owning_view.h"
#include "ref_view.h"
#include "subrange.h"
#include "take_view.h"
#include "transform_view.h"

/** @} */

#endif
