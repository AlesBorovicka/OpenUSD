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
    virtual void Reset() = 0;

    /// Check if iterator is at the end; \return True if at the end
    virtual bool AtEnd() const = 0;

    /// Get current iterator; \return Current iterator
    virtual UsdPrimRange::const_iterator GetCurrent() = 0;

    /// Update iterator to next
    virtual void Next() = 0;

    /// Prune all children of the current iterator
    virtual void PruneChildren() = 0;
};

/// \struct ParsePrimIteratorRange
///
/// Class for standard UsdPrimRange iterator
///
class ParsePrimIteratorRange : public ParsePrimIteratorBase
{
public:
    ParsePrimIteratorRange(UsdPrimRange range) : _range(range)
    {
        Reset();
    }

    void Reset() override
    {
        _iter = _range.begin();
    }

    void PruneChildren() override
    {
        if (!AtEnd()) {
            _iter.PruneChildren();
        }
    }

    bool AtEnd() const override
    {
        return _iter == _range.end();
    }

    UsdPrimRange::const_iterator GetCurrent() override
    {
        return _iter;
    }

    void Next() override
    {
        if (_iter != _range.end())
        {
            _iter++;
        }
    }

private:
    UsdPrimRange _range;
    UsdPrimRange::const_iterator _iter;
};

/// \struct ParsePrimIteratorMapRange
///
/// Class for combined UsdPrimRange iterator that iterates over multiple 
/// hierarchies. This is required for processing newly added subtrees to be 
/// processed in one traverse.
///
class ParsePrimIteratorMapRange : public ParsePrimIteratorBase
{
public:
    ParsePrimIteratorMapRange(const UsdPrimMap& primMap) : _primMap(primMap)
    {
        Reset();
    }

    void Reset() override
    {
        _atEnd = true;

        _primMapIter = _primMap.begin();

        if (_primMapIter != _primMap.end())
        {
            _range = UsdPrimRange(_primMapIter->second, 
                                  UsdTraverseInstanceProxies());
            if (_range.begin() != _range.end())
            {
                _iter = _range.begin();
                _atEnd = false;
            }
        }
    }

    bool AtEnd() const override
    {
        return _atEnd;
    }

    void PruneChildren() override
    {
        if (!AtEnd()) {
            _iter.PruneChildren();
        }
    }

    UsdPrimRange::const_iterator GetCurrent() override
    {
        return _iter;
    }

    void Next() override
    {
        if (_iter != _range.end())
        {
            _iter++;

            if (_iter == _range.end())
            {
                _primMapIter++;

                if (_primMapIter == _primMap.end())
                {
                    _atEnd = true;
                }
                else
                {
                    _range = UsdPrimRange(_primMapIter->second);
                    _iter = _range.begin();
                }
            }
        }
    }

private:
    bool _atEnd;

    const UsdPrimMap& _primMap;
    UsdPrimMap::const_iterator _primMapIter;

    UsdPrimRange _range;
    UsdPrimRange::const_iterator _iter;
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
    ExcludeListPrimIteratorRange(UsdPrimRange range, 
                                 const SdfPathVector& pathList)
        : _range(range)
    {
        for (const SdfPath& path : pathList)
        {
            _pathSet.insert(path);
        }
        Reset();
    }

    void Reset() override
    {
        _iter = _range.begin();
    }

    void PruneChildren() override
    {
        _iter.PruneChildren();
    }

    bool AtEnd() const override
    {
        return _iter == _range.end();
    }

    UsdPrimRange::const_iterator GetCurrent() override
    {
        return _iter;
    }

    void Next() override
    {
        if (_iter != _range.end())
        {
            bool validPrim = false;
            while (!validPrim)
            {
                _iter++;
                if (_iter != _range.end())
                {
                    const UsdPrim& prim = *_iter;
                    if (prim)
                    {
                        ExcludePathsSet::const_iterator fit = 
                            _pathSet.find(prim.GetPrimPath());
                        if (fit != _pathSet.end())
                        {
                            _iter.PruneChildren();
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
    UsdPrimRange _range;
    UsdPrimRange::const_iterator _iter;
    ExcludePathsSet _pathSet;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif
