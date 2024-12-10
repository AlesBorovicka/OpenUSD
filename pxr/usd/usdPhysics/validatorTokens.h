//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#ifndef USDPHYSICS_VALIDATOR_TOKENS_H
#define USDPHYSICS_VALIDATOR_TOKENS_H

/// \file

#include "pxr/pxr.h"
#include "pxr/usd/usdPhysics/api.h"
#include "pxr/base/tf/staticTokens.h"

PXR_NAMESPACE_OPEN_SCOPE

#define USD_PHYSICS_VALIDATOR_NAME_TOKENS                              \
    ((rigidBodyChecker, "usdPhysics:RigidBodyChecker"))        \
    ((colliderChecker, "usdPhysics:ColliderChecker"))                    \
    ((physicsJointChecker, "usdPhysics:PhysicsJointChecker"))                    \
    ((articulationChecker, "usdPhysics:ArticulationChecker"))

#define USD_PHYSICS_VALIDATOR_KEYWORD_TOKENS  (UsdPhysicsValidators) 

#define USD_PHYSICS_VALIDATION_ERROR_NAME_TOKENS                     \
    ((nestedRigidBody, "NestedRigidBody"))    \
    ((nestedArticulation, "NestedArticulation"))                  \
    ((articulationOnStaticBody, "ArticulationOnStaticBody"))                  \
    ((articulationOnKinematicBody, "ArticulationOnKinematicBody"))                  \
    ((rigidBodyOrientationScale,  "RigidBodyOrientationScale"))   \
    ((rigidBodyNonXformable,  "RigidBodyNonXformable"))   \
    ((rigidBodyNonInstanceable,  "RigidBodyNonInstanceable"))   \
    ((jointInvalidPrimRel,  "JointInvalidPrimRel"))   \
    ((jointMultiplePrimsRel,  "JointMultiplePrimsRel"))   \
    ((colliderNonUniformScale, "ColliderNonUniformScale")) \
    ((colliderSpherePointsDataMissing, "ColliderSpherePointsDataMissing"))

/// \def USD_PHYSICS_VALIDATOR_NAME_TOKENS
/// Tokens representing validator names. Note that for plugin provided
/// validators, the names must be prefixed by usdPhysics:, which is the name of
/// the usdPhysics plugin.
    TF_DECLARE_PUBLIC_TOKENS(UsdPhysicsValidatorNameTokens, USDPHYSICS_API,
                             USD_PHYSICS_VALIDATOR_NAME_TOKENS);

/// \def USD_PHYSICS_VALIDATOR_KEYWORD_TOKENS
/// Tokens representing keywords associated with any validator in the usdPhysics
/// plugin. Clients can use this to inspect validators contained within a
/// specific keywords, or use these to be added as keywords to any new
/// validator.
    TF_DECLARE_PUBLIC_TOKENS(UsdPhysicsValidatorKeywordTokens, USDPHYSICS_API,
                             USD_PHYSICS_VALIDATOR_KEYWORD_TOKENS);

/// \def USD_PHYSICS_VALIDATION_ERROR_NAME_TOKENS
/// Tokens representing validation error identifier.
TF_DECLARE_PUBLIC_TOKENS(UsdPhysicsValidationErrorNameTokens, USDPHYSICS_API, 
                         USD_PHYSICS_VALIDATION_ERROR_NAME_TOKENS);

PXR_NAMESPACE_CLOSE_SCOPE

#endif
