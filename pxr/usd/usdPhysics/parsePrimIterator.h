//
// Copyright 2024 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef USDPHYSICS_PARSE_PRIM_ITERATOR_H
#define USDPHYSICS_PARSE_PRIM_ITERATOR_H

/// \file usdPhysics/parseParsePrimIterator.h

#include "pxr/pxr.h"
#include "pxr/usd/usd/primRange.h"

#include <unordered_set>

PXR_NAMESPACE_OPEN_SCOPE


// -------------------------------------------------------------------------- //
// PHYSICSPARSEPRIMITERATOR                                                   //
// -------------------------------------------------------------------------- //

using UsdPrimMap = std::map<const SdfPath, UsdPrim>;

/// \struct ParsePrimIteratorBase
///
/// Base class for iterator used by parsing
///
class ParsePrimIteratorBase
{
public:
    virtual ~ParsePrimIteratorBase() = default;

    /// Reset the iteration
    virtual void reset() = 0;

    /// Check if iterator is at the end
    /// \return True if at the end
    virtual bool atEnd() const = 0;

    /// Get current iterator
    /// \return Current iterator
    virtual UsdPrimRange::const_iterator getCurrent() = 0;

    /// Update iterator to next
    virtual void next() = 0;

    /// Prune all children of the current iterator
    virtual void pruneChildren() = 0;
};

/// \struct ParsePrimIteratorRange
///
/// Class for standard UsdPrimRange iterator
///
class ParsePrimIteratorRange : public ParsePrimIteratorBase
{
public:
    ParsePrimIteratorRange(UsdPrimRange range) : mRange(range)
    {
        reset();
    }

    virtual void reset() override
    {
        mIter = mRange.begin();
    }

    virtual void pruneChildren() override
    {
        if (!atEnd())
            mIter.PruneChildren();
    }

    virtual bool atEnd() const override
    {
        return mIter == mRange.end();
    }

    virtual UsdPrimRange::const_iterator getCurrent() override
    {
        return mIter;
    }

    virtual void next() override
    {
        if (mIter != mRange.end())
        {
            mIter++;
        }
    }

private:
    UsdPrimRange mRange;
    UsdPrimRange::const_iterator mIter;
};

/// \struct ParsePrimIteratorMapRange
///
/// Class for combined UsdPrimRange iterator that iterates over multiple hierarchies.
/// This is required for processing newly added subtrees to be processed in one traverse.
///
class ParsePrimIteratorMapRange : public ParsePrimIteratorBase
{
public:
    ParsePrimIteratorMapRange(const UsdPrimMap& primMap) : mPrimMap(primMap)
    {
        reset();
    }

    virtual void reset() override
    {
        mAtEnd = true;

        mPrimMapIter = mPrimMap.begin();

        if (mPrimMapIter != mPrimMap.end())
        {
            mRange = UsdPrimRange(mPrimMapIter->second, UsdTraverseInstanceProxies());
            if (mRange.begin() != mRange.end())
            {
                mIter = mRange.begin();
                mAtEnd = false;
            }
        }
    }

    virtual bool atEnd() const override
    {
        return mAtEnd;
    }

    virtual void pruneChildren() override
    {
        if (!atEnd())
            mIter.PruneChildren();
    }

    virtual UsdPrimRange::const_iterator getCurrent() override
    {
        return mIter;
    }

    virtual void next() override
    {
        if (mIter != mRange.end())
        {
            mIter++;

            if (mIter == mRange.end())
            {
                mPrimMapIter++;

                if (mPrimMapIter == mPrimMap.end())
                {
                    mAtEnd = true;
                }
                else
                {
                    mRange = UsdPrimRange(mPrimMapIter->second);
                    mIter = mRange.begin();
                }
            }
        }
    }

private:
    bool mAtEnd;

    const UsdPrimMap& mPrimMap;
    UsdPrimMap::const_iterator mPrimMapIter;

    UsdPrimRange mRange;
    UsdPrimRange::const_iterator mIter;
};

using ExcludePathsSet = std::unordered_set<SdfPath, SdfPath::Hash>;
class ExcludeListPrimIteratorRange : public ParsePrimIteratorBase
{
public:
    /// ExcludeListPrimIteratorRange constructor, this iterator
    /// takes a regular range as input and exclude set for paths an
    /// its descendants to get pruned. 
    ///
    /// \param[in] range      UsdPrimRange to traverse
    /// \param[in] pathList   Paths to get pruned
    ExcludeListPrimIteratorRange(UsdPrimRange range, const SdfPathVector& pathList)
        : mRange(range)
    {
        for (const SdfPath& path : pathList)
        {
            mPathSet.insert(path);
        }
        reset();
    }

    virtual void reset() override
    {
        mIter = mRange.begin();
    }

    virtual void pruneChildren() override
    {
        mIter.PruneChildren();
    }

    virtual bool atEnd() const override
    {
        return mIter == mRange.end();
    }

    virtual UsdPrimRange::const_iterator getCurrent() override
    {
        return mIter;
    }

    virtual void next() override
    {
        if (mIter != mRange.end())
        {
            bool validPrim = false;
            while (!validPrim)
            {
                mIter++;
                if (mIter != mRange.end())
                {
                    const UsdPrim& prim = *mIter;
                    if (prim)
                    {
                        ExcludePathsSet::const_iterator fit = mPathSet.find(prim.GetPrimPath());
                        if (fit != mPathSet.end())
                        {
                            mIter.PruneChildren();
                        }
                        else
                        {
                            validPrim = true;
                        }
                    }
                }
                else
                {
                    validPrim = true;
                }
            }
        }
    }

private:
    UsdPrimRange mRange;
    UsdPrimRange::const_iterator mIter;
    ExcludePathsSet mPathSet;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
