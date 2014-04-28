#include "SolutionModel.h"

namespace vm
{
/***********************************************************************
StudioModel
***********************************************************************/

	StudioModel::StudioModel()
	{
	}

	StudioModel::~StudioModel()
	{
	}

	LazyList<Ptr<IFileFactoryModel>> StudioModel::GetProjectModels()
	{
		throw 0;
	}

	LazyList<Ptr<IFileFactoryModel>> StudioModel::GetFileModels(WString category)
	{
		throw 0;
	}
}