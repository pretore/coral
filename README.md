# coral

![main](https://github.com/pretore/coral/actions/workflows/cmake.yml/badge.svg?branch=main)

## Data types:
- `coral_integer` - _arbitrary-precision integer_

## Collections:
- `coral_array` - _dynamic memory allocated array_
- `coral_tree_set` - _set backed by a self-balancing tree_
- `coral_tree_map` - _map backed by a self-balancing tree_

## Ownership:
- `coral_reference` - _strong reference_
- `coral_weak_reference` - _weak reference_
- `coral_context` - _strong reference for C data types_

## Miscellaneous:
- `coral_range` - _beginning and end with progression and membership checks_
- `coral_lock` - _exclusive access to a shared resource_
- `coral_lock_condition` - _event signalling between two or more threads_
- `coral_rwlock` - _shared resource has exclusive write access while having unconstrained read access_
