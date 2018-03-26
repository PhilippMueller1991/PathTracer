#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

class MeshLoader
{
public:
	static bool LoadOBJMesh(const std::string& fileName, 
		std::vector<Vec3f>& vertices, 
		std::vector<Vec3f>& normals,
		std::vector<Vec2f>& uvs,
		std::vector<uint32_t>& vidxs,
		std::vector<uint32_t>& uvidxs,
		std::vector<uint32_t>& nidxs)
	{
		std::ifstream file;
		file.open(fileName.c_str());

		std::string line;
		if (!file.is_open())
		{
			std::cerr << "Unable to load mesh: " << fileName.c_str() << std::endl;
			return false;
		}

		while (file.good())
		{
			std::getline(file, line);

			uint32_t lineLength = static_cast<uint32_t>(line.length());
			if (lineLength < 2)
				continue;

			//const char* lineCStr = line.c_str();
			switch (line[0])
			{
			case 'v':
				if (line[1] == 't')	// Texture entry
					uvs.push_back(ParseVec2f(line));
				else if (line[1] == 'n')	// Normal vector entry
					normals.push_back(ParseVec3f(line));
				else if (line[1] == ' ' || line[1] == '\t')	// Vertex position entry
					vertices.push_back(ParseVec3f(line));
				break;
			case 'f':	// Connectivity information
				CreateFace(line, vidxs, uvidxs, nidxs);
				break;
			default:
				break;
			};
		}

		return true;
	}

private:
	MeshLoader() = default;

	static Vec2f ParseVec2f(const std::string& line)
	{ 
		uint32_t tokenLength = static_cast<uint32_t>(line.length());
		const char* tokenString = line.c_str();

		uint32_t vertIndexStart = 3;
		while (vertIndexStart < tokenLength)
		{
			if (tokenString[vertIndexStart] != ' ')
				break;
			vertIndexStart++;
		}
		// X
		uint32_t vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, ' ');
		float x = ParseFloat(line, vertIndexStart, vertIndexEnd);
		// Y
		vertIndexStart = vertIndexEnd + 1;
		vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, ' ');
		float y = ParseFloat(line, vertIndexStart, vertIndexEnd);

		return Vec2f(x, y);
	}

	static Vec3f ParseVec3f(const std::string& line)
	{
		uint32_t tokenLength = static_cast<uint32_t>(line.length());
		const char* tokenString = line.c_str();

		uint32_t vertIndexStart = 2;
		while (vertIndexStart < tokenLength)
		{
			if (tokenString[vertIndexStart] != ' ')
				break;
			vertIndexStart++;
		}
		// X
		uint32_t vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, ' ');
		float x = ParseFloat(line, vertIndexStart, vertIndexEnd);
		// Y
		vertIndexStart = vertIndexEnd + 1;
		vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, ' ');
		float y = ParseFloat(line, vertIndexStart, vertIndexEnd);
		// Z
		vertIndexStart = vertIndexEnd + 1;
		vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, ' ');
		float z = ParseFloat(line, vertIndexStart, vertIndexEnd);

		return Vec3f(x, y, z);
	}

	static void CreateFace(const std::string& line, 
		std::vector<uint32_t>& vidxs,
		std::vector<uint32_t>& uvidxs,
		std::vector<uint32_t>& nidxs)
	{
		std::vector<std::string> tokens = SplitString(line, ' ');

		ParseIndex(tokens[1], vidxs, uvidxs, nidxs);
		ParseIndex(tokens[2], vidxs, uvidxs, nidxs);
		ParseIndex(tokens[3], vidxs, uvidxs, nidxs);
	}

	static inline void ParseIndex(const std::string& token, 
		std::vector<uint32_t>& vidxs, 
		std::vector<uint32_t>& uvidxs, 
		std::vector<uint32_t>& nidxs)
	{
		const char* tokenString = token.c_str();
		uint32_t tokenLength = static_cast<uint32_t>(token.length());

		// Format
		// f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3

		// v
		uint32_t vertIndexStart = 0;
		uint32_t vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, '/');
		vidxs.push_back(ParseIndex(token, vertIndexStart, vertIndexEnd));
		
		// vt
		if (vertIndexEnd >= tokenLength)
			return;
		vertIndexStart = vertIndexEnd + 1;
		vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, '/');
		uvidxs.push_back(ParseIndex(token, vertIndexStart, vertIndexEnd));
		
		// vn
		if (vertIndexEnd >= tokenLength)
			return;
		vertIndexStart = vertIndexEnd + 1;
		vertIndexEnd = FindNextToken(vertIndexStart, tokenString, tokenLength, '/');
		nidxs.push_back(ParseIndex(token, vertIndexStart, vertIndexEnd));
	}

	static inline uint32_t FindNextToken(uint32_t start, const char* str, uint32_t length, char token)
	{
		uint32_t result = start;
		while (result < length)
		{
			if (str[result] == token)
				break;
			result++;
		}
		return result;
	}

	static inline int ParseIndex(const std::string& token, uint32_t start, uint32_t end)
	{
		return atoi(token.substr(start, end - start).c_str()) - 1;
	}

	static inline float ParseFloat(const std::string& token, uint32_t start, uint32_t end)
	{
		return static_cast<float>(atof(token.substr(start, end - start).c_str()));
	}

	static inline std::vector<std::string> SplitString(const std::string &s, char delim)
	{
		std::vector<std::string> elems;

		const char* cstr = s.c_str();
		uint32_t strLength = static_cast<uint32_t>(s.length());
		uint32_t start = 0;
		uint32_t end = 0;
		while (end <= strLength)
		{
			while (end <= strLength)
			{
				if (cstr[end] == delim)
					break;
				end++;
			}
			elems.push_back(s.substr(start, end - start));
			start = end + 1;
			end = start;
		}
		return elems;
	}
};