//  Copyright (c) Meta Platforms, Inc. and affiliates.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#pragma once

#include "rocksdb/cleanable.h"
#include "rocksdb/slice.h"
#include "rocksdb/status.h"

namespace ROCKSDB_NAMESPACE {

class IteratorBase : public Cleanable {
 public:
  IteratorBase() {}
  // No copying allowed
  IteratorBase(const IteratorBase&) = delete;
  void operator=(const IteratorBase&) = delete;

  virtual ~IteratorBase() {}

  // An iterator is either positioned at a key/value pair, or
  // not valid.  This method returns true iff the iterator is valid.
  // Always returns false if !status().ok().
  virtual bool Valid() const = 0;

  // Position at the first key in the source.  The iterator is Valid()
  // after this call iff the source is not empty.
  virtual void SeekToFirst() = 0;

  // Position at the last key in the source.  The iterator is
  // Valid() after this call iff the source is not empty.
  virtual void SeekToLast() = 0;

  // Position at the first key in the source that at or past target.
  // The iterator is Valid() after this call iff the source contains
  // an entry that comes at or past target.
  // All Seek*() methods clear any error status() that the iterator had prior to
  // the call; after the seek, status() indicates only the error (if any) that
  // happened during the seek, not any past errors.
  // Target does not contain timestamp.
  virtual void Seek(const Slice& target) = 0;

  // Position at the last key in the source that at or before target.
  // The iterator is Valid() after this call iff the source contains
  // an entry that comes at or before target.
  // Target does not contain timestamp.
  virtual void SeekForPrev(const Slice& target) = 0;

  // Moves to the next entry in the source.  After this call, Valid() is
  // true iff the iterator was not positioned at the last entry in the source.
  // REQUIRES: Valid()
  virtual void Next() = 0;

  // Moves to the previous entry in the source.  After this call, Valid() is
  // true iff the iterator was not positioned at the first entry in source.
  // REQUIRES: Valid()
  virtual void Prev() = 0;

  // Return the key for the current entry.  The underlying storage for
  // the returned slice is valid only until the next modification of the
  // iterator (i.e. the next SeekToFirst/SeekToLast/Seek/SeekForPrev/Next/Prev
  // operation).
  // REQUIRES: Valid()
  virtual Slice key() const = 0;

  // If an error has occurred, return it.  Else return an ok status.
  // If non-blocking IO is requested and this operation cannot be
  // satisfied without doing some IO, then this returns Status::Incomplete().
  virtual Status status() const = 0;
};

}  // namespace ROCKSDB_NAMESPACE
