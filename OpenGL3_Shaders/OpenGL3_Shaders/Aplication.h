#pragma once

#include <memory>
#include "VertexArray.h"
#include "Shader.h"
class Aplication
{
public:
	void Run();

private:
	void Init();
	void Update();
	void Render();

	std::unique_ptr<VertexArray> _va;
	std::unique_ptr<Shader> _shader;
};

