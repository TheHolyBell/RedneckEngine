#pragma once
#include <string>

struct PBRInitInfo
{
	std::string albedo_path;
	std::string normal_path;
	std::string metallic_path;
	std::string roughness_path;
	std::string ao_path;
};