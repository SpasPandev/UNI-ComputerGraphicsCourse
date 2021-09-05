#pragma once

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
class VertexArray
{
public:
	VertexArray();

	void Bind();
	void Unbind();

	void SetVertexBuffer(std::unique_ptr<VertexBuffer> vb);
	void SetIndexBuffer(std::unique_ptr<IndexBuffer> ib);

private:
	unsigned id;

	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<IndexBuffer> ibo;
};

