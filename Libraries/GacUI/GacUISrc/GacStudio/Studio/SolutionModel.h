/***********************************************************************
Vczh Library++ 3.0
Developer: ³Âè÷å«(vczh)
GacStudio::SolutionModel

Interfaces:
***********************************************************************/

#ifndef GACSTUDIO_STUDIO_SOLUTIONMODEL
#define GACSTUDIO_STUDIO_SOLUTIONMODEL

#include "..\UI\GacStudioUI.h"

using namespace vl::collections;

namespace vm
{
	class StudioModel : public Object, public virtual IStudioModel
	{
	public:
		StudioModel();
		~StudioModel();

		LazyList<Ptr<IFileFactoryModel>> GetProjectModels()override;
		LazyList<Ptr<IFileFactoryModel>> GetFileModels(WString category)override;
	};
}

#endif