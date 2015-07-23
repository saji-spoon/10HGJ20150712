#include"Resource.hpp"

void ResourceRegister()
{
	const FilePath ResourcePath = L"Resource/";
	const FilePath CityPath = L"City/";

	const FilePath CityRegister = ResourcePath + CityPath;
	//TextureAsset::Register(L"TestCity", CityRegister + L"TestCity.png", {}, AssetFlag::Default);

	FontAsset::Register(L"Debug", 24, Typeface::Default);
}