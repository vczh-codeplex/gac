/***********************************************************************
Vczh Library++ 3.0
Developer: �����(vczh)
Data Structure::Operations

��չ��
	ʵ��һ����������IteratorType CreateForEachIterator(const CollectionType& collection);
	CollectionType������Ҫ����������
	IteratorType�̳���ForEachIterator<T>
	����д��vl::collections�����ռ���
***********************************************************************/

#ifndef VCZH_COLLECTIONS_FOREACH
#define VCZH_COLLECTIONS_FOREACH
#include "..\Basic.h"
#include "..\Pointer.h"
#include "Interfaces.h"

namespace vl
{
	namespace collections
	{

/***********************************************************************
ForEach������ʩ
***********************************************************************/

		template<typename T>
		class ForEachIterator : public Object
		{
		public:
			virtual bool				Next(T& variable)const=0;

			operator bool()const
			{
				return true;
			}
		};

/***********************************************************************
IEnumerable<T>֧��
***********************************************************************/

		template<typename T>
		class EnumerableForEachIterator : public ForEachIterator<T>
		{
		protected:
			Ptr<IEnumerator<T>>			enumerator;
		public:
			EnumerableForEachIterator(const IEnumerable<T>& enumerable)
				:enumerator(enumerable.CreateEnumerator())
			{
			}

			EnumerableForEachIterator(const EnumerableForEachIterator<T>& enumerableIterator)
				:enumerator(enumerableIterator.iterator)
			{
			}

			bool Next(T& variable)const
			{
				if(enumerator->Next())
				{
					variable=enumerator->Current();
					return true;
				}
				else
				{
					return false;
				}
			}
		};

		template<typename T>
		EnumerableForEachIterator<T> CreateForEachIterator(const IEnumerable<T>& enumerable)
		{
			return enumerable;
		}

/***********************************************************************
ForEach��
***********************************************************************/

#define SCOPE_VARIABLE(TYPE, VARIABLE, VALUE)\
		if(bool __scope_variable_flag__=true)\
			for(TYPE VARIABLE = VALUE;__scope_variable_flag__;__scope_variable_flag__=false)

#define FOREACH(TYPE, VARIABLE, COLLECTION)\
		SCOPE_VARIABLE(const ForEachIterator<TYPE>&, __foreach_iterator__, CreateForEachIterator(COLLECTION))\
		for(TYPE VARIABLE;__foreach_iterator__.Next(VARIABLE);)

#define FOREACH_INDEXER(TYPE, VARIABLE, INDEXER, COLLECTION)\
		SCOPE_VARIABLE(const ForEachIterator<TYPE>&, __foreach_iterator__, CreateForEachIterator(COLLECTION))\
		SCOPE_VARIABLE(vint, INDEXER, 0)\
		for(TYPE VARIABLE;__foreach_iterator__.Next(VARIABLE);INDEXER++)
	}
}

#endif