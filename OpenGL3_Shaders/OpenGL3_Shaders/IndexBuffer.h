#pragma once
class IndexBuffer
{
public:
	IndexBuffer(unsigned int* data, int size);

	void Bind();
	void Unbind();

private:
	unsigned int id;
};
