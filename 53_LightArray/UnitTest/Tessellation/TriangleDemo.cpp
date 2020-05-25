#include "stdafx.h"
#include "TriangleDemo.h"

void TriangleDemo::Initialize()
{
	shader = new Shader(L"46_TessTriangle.fxo");

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -5);
	dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(10, 0);

	vertices[0].Position = Vector3(-0.9f, -0.9f, 0.0f);
	vertices[1].Position = Vector3(+0.0f, +0.9f, 0.0f);
	vertices[2].Position = Vector3(+0.9f, -0.9f, 0.0f);

	vertexBuffer = new VertexBuffer(vertices, 3, sizeof(Vertex));
	
}

void TriangleDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(vertexBuffer);
}

void TriangleDemo::Update()
{
	static Vector4 factor = Vector4(2, 2, 2, 1);
	ImGui::SliderFloat4("Factor", factor, 1, 64);
	shader->AsVector("Factor")->SetFloatVector(factor);
}

void TriangleDemo::Render()
{
	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 3);
}


