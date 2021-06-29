#pragma once

namespace Geothermal
{
	class FileIO
	{
	public:
		static std::string ReadData(winrt::hstring const& filename);
	};
}
