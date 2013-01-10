#include "..\..\Public\Source\GacUI.h"

namespace vl
{
	namespace sdi
	{
		struct SDIConfiguration
		{
			WString				applicationName;
		};

		extern int				StartSDI(const SDIConfiguration& config);
	}
}