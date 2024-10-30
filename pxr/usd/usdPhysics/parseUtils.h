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
#include "pxr/usd/usdPhysics/parsePrimIterator.h"

PXR_NAMESPACE_OPEN_SCOPE


// -------------------------------------------------------------------------- //
// PHYSICSPARSEUTILS                                                          //
// -------------------------------------------------------------------------- //


/// UsdPhysicsReportFn - report function that reports parsed data
///
/// \param[in] type     Object type
/// \param[in] numDesc  Number of descriptors to be processed
/// \param[in] primPaths    Array of prim paths that were parsed
/// \param[in] objectDescs  Corresponding array of object descriptors of the reported type
/// \param[in] userData User data provided to the parsing function
using UsdPhysicsReportFn =
std::function<void(UsdPhysicsObjectType::Enum type, size_t numDesc,
    const SdfPath* primPaths, const UsdPhysicsObjectDesc* objectDescs,
    void* userData)>;

/// \struct CustomUsdPhysicsTokens
///
/// Token lists for custom physics objects
///
struct CustomUsdPhysicsTokens
{
    std::vector<TfToken> jointTokens;       ///< Custom joints to be reported by parsing
    std::vector<TfToken> shapeTokens;       ///< Custom shapes to be reported by parsing
    std::vector<TfToken> instancerTokens;   ///< Custom physics instancers to be 
    ///< skipped by parsing
};

/// Load USD physics from a given range
///
/// \param[in] stage      Stage to parse
/// \param[in] range      USDRange to parse
/// \param[in] reportFn   Report function that gets parsed USD physics data
/// \param[in] userData   User data passed to report function
/// \param[in] customPhysicsTokens Custom tokens to be reported by the parsing
/// \param[in] simulationOwners List of simulation owners that should be parsed, 
///                       adding SdfPath() indicates that objects without a 
///                       simulation owner should be parsed too.
/// \return True if load was successful
USDPHYSICS_API bool LoadUsdPhysicsFromRange(const UsdStageWeakPtr stage,
    ParsePrimIteratorBase& range,
    UsdPhysicsReportFn reportFn,
    void* userData,
    const CustomUsdPhysicsTokens* customPhysicsTokens = nullptr,
    const std::vector<SdfPath>* simulationOwners = nullptr);


PXR_NAMESPACE_CLOSE_SCOPE

#endif
