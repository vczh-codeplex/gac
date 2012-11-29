/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Data Structure::Operations

Functions:
	CopyFrom(TargetContainer, SourceContainer)
	[T] >>	Select(T->K) => [K]
	[T] >>	Where(T->bool) => [T]
	[T] >>	OrderBy(T->T->int) => [T]
	[T] >>	Aggregate(T->T->T) => T
	[T] >>	Aggregate(T->T->T, T) => T
	[T] >>	All(T->bool) => bool
	[T] >>	Any(T->bool) => bool
	[T] >>	Max() => T
	[T] >>	Min() => T
	[T] >>	First() => T
	[T] >>	Count() => vint
	[T] >>	IsEmpty() => bool
	[T] >>	Concat([T]) => [T]
	[T] >>	Repeat(vint) => [T]
	[T] >>	Take(vint) => [T]
	[T] >>	Skip(vint) => [T]
	[T] >>	Distinct() => [T]
	[T] >>	Reverse() => [T]
			FromIterator<T>::Wrap(begin, end) => [T]
			FromPointer<T>(begin, end) => [T]
			FromArray<T[]>(array) => [T]
	[T] >>	Intersect([T]) => [T]
	[T] >>	Union([T]) => [T]
	[T] >>	Except([T]) => [T]
	[T] >>	Pairwise([K]) => [(T,K)]

FOREACH(X, a, XList)
FOREACH_INDEXER(X, a, index, XList)
***********************************************************************/

#ifndef VCZH_COLLECTIONS_OPERATION
#define VCZH_COLLECTIONS_OPERATION

#include "OperationCopyFrom.h"
#include "OperationEnumerable.h"
#include "OperationSelect.h"
#include "OperationWhere.h"
#include "OperationOrderBy.h"
#include "OperationAggregate.h"
#include "OperationConcat.h"
#include "OperationSequence.h"
#include "OperationSet.h"
#include "OperationPair.h"
#include "OperationString.h"
#include "OperationForEach.h"

#endif