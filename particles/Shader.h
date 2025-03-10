#pragma once


class Shader {
public:

	void create(const char* vertexPath, const char* fragmentPath);

	Shader();

	void use() const ;

	unsigned int ID{};

private:

};