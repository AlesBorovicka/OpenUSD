//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef USDPHYSICS_PARSE_UTILS_H
#define USDPHYSICS_PARSE_UTILS_H

/// \file usdPhysics/parseUtils.h

#include "pxr/pxr.h"
#include "pxr/usd/usd/stage.h"
#include "pxr/usd/usdGeom/xformCache.h"
#include "pxr/usd/sdf/path.h"

#include "pxr/usd/usdPhysics/api.h"
#include "pxr/usd/usdPhysics/parseDesc.h"


PXR_NAMESPACE_OPEN_SCOPE


// -------------------------------------------------------------------------- //
// PHYSICSPARSEUTILS                                                          //
// -------------------------------------------------------------------------- //


/// UsdPhysicsReportFn - report function that reports parsed data
///
/// \param[in] type     Object type
/// \param[in] primPaths    Span of prim paths that were parsed
/// \param[in] objectDescs  Corresponding span of object descriptors of the 
/// reported type
/// \param[in] userData User data provided to the parsing function
using UsdPhysicsReportFn =
std::function<void(UsdPhysicsObjectType::Enum type,
    TfSpan<const SdfPath> primPaths, TfSpan<const UsdPhysicsObjectDesc> objectDescs,
    const VtValue& userData)>;

/// \struct CustomUsdPhysicsTokens
///
/// Token lists for custom physics objects
///
struct CustomUsdPhysicsTokens
{
    /// Custom joints to be reported by parsing
    std::vector<TfToken> jointTokens;
    /// Custom shapes to be reported by parsing
    std::vector<TfToken> shapeTokens;
    /// Custom physics instancers to be reported by parsing
    std::vector<TfToken> instancerTokens;
};

/// Load USD physics from a given list of paths
/// 
/// 
/// 
/// See the parsing utils documentation for more details.
///
/// \param[in] stage      Stage to parse.
/// \param[in] includePaths The vector of SdfPaths that are used to construct
///                       UsdPrimRange for traversals. For each path a UsdPrim
///                       is found on the stage and UsdRange traversal is 
///                       constructed from it. All the traversed UsdPrims are
///                       parsed and physics descriptors are constructed and
///                       reported through the report function.
/// \param[in] reportFn   Report function that gets parsed USD physics data
/// \param[in] userData   User data passed to report function, this can be
///                       arbitrary data structure, user data are just passed
///                       to the report function.
/// \param[in] excludePaths The vector of SdfPaths that are used to prune
///                       UsdPrims from the parsing UsdPrimRange traversals
///                       constructed from the include paths.
/// \param[in] customPhysicsTokens Custom tokens to be reported by the parsing
/// \param[in] simulationOwners List of simulation owners that should be parsed, 
///                       adding SdfPath() indicates that objects without a 
///                       simulation owner should be parsed too.
/// \return True if load was successful
USDPHYSICS_API bool LoadUsdPhysicsFromRange(const UsdStageWeakPtr stage,
    const std::vector<SdfPath>& includePaths,
    UsdPhysicsReportFn reportFn,
    const VtValue& userData,
    const std::vector<SdfPath>* excludePaths = nullptr,
    const CustomUsdPhysicsTokens* customPhysicsTokens = nullptr,
    const std::vector<SdfPath>* simulationOwners = nullptr);


PXR_NAMESPACE_CLOSE_SCOPE

#endif
