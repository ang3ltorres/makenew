#include <iostream>
#include <cstdlib>
#include <cstring>
#include <print>
#include <format>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>

static const std::filesystem::path homePath = "C:/Users/angel";

std::string projectName;
std::string structure;
std::string standard;
std::filesystem::path path;

bool validArguments();
void makenew();

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::println("Error: Wrong argument count");
		std::exit(EXIT_FAILURE);
	}

	projectName = argv[1];
	structure   = argv[2];
	standard    = argv[3];

	if (not validArguments())
	{
		std::println("Error: Invalid arguments");
		return EXIT_FAILURE;
	}

	path = homePath / projectName;

	if (std::filesystem::exists(path))
	{
		std::println("Error: Existing {}", std::filesystem::is_directory(path) ? "directory" : "file");
		return EXIT_FAILURE;
	}
	else
		makenew();

	return EXIT_SUCCESS;
}

bool validArguments()
{
	if (structure != "default" and structure != "simple")
		return false;

	try
	{
		int std = std::stoi(standard);
		switch (std)
		{
			case 98: case 11: case 14: case 17: case 20: case 23: case 26: break;
			default: return false;
		}
	}
	catch(...){ return false; }

	return true;
}

void makenew()
{
	bool simple = (structure == "simple");

	std::filesystem::create_directories(path);
	std::filesystem::create_directories(path / "src");

	if (not simple)
		std::filesystem::create_directories(path / "include");

	std::ofstream mainFile(path / "src" / "main.cpp", std::ofstream::out | std::ofstream::binary);
	mainFile << std::format("// Using C++{} standard\n", standard);
	mainFile << "#include <cstdio>\n\n";
	mainFile << "int main()\n";
	mainFile << "{\n";
	mainFile << "\tprintf(\"Hello world\\n\");\n";
	mainFile << "\treturn 0;\n";
	mainFile << "}\n";
	mainFile.close();

	std::ofstream cmakeFile(path / "CMakeLists.txt", std::ofstream::out | std::ofstream::binary);
	cmakeFile << std::format("cmake_minimum_required(VERSION 3.31)\n");
	cmakeFile << std::format("project({} LANGUAGES CXX)\n", projectName);
	cmakeFile << std::format("set(CMAKE_CXX_STANDARD {})\n", standard);
	cmakeFile << std::format("set(CMAKE_CXX_STANDARD_REQUIRED ON)\n");
	cmakeFile << std::format("set(CMAKE_CXX_EXTENSIONS OFF)\n\n");
	cmakeFile << std::format("file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS \"${{CMAKE_CURRENT_SOURCE_DIR}}/src/*.cpp\")\n");
	cmakeFile << std::format("add_executable({} ${{SOURCES}})\n", projectName);
	if (simple) cmakeFile << std::format("target_include_directories({} PRIVATE \"${{CMAKE_CURRENT_SOURCE_DIR}}/src\")\n", projectName);
	else        cmakeFile << std::format("target_include_directories({} PRIVATE \"${{CMAKE_CURRENT_SOURCE_DIR}}/include\")\n", projectName);
	cmakeFile << std::format("target_compile_options({} PRIVATE -Wall -Wextra -Wpedantic -fmax-errors=1)\n", projectName);
	cmakeFile << std::format("target_link_libraries({} PUBLIC stdc++exp)\n", projectName);
	cmakeFile.close();
}
