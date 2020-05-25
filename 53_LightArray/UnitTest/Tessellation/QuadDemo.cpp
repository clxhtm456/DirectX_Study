#include "stdafx.h"
#include "QuadDemo.h"


void QuadDemo::Initialize()
{
	shader = new Shader(L"46_TessQuad.fxo");

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -5);
	dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(10, 0);

	vertices[0].Position = Vector3(-0.9f, -0.9f, 0.0f);
	vertices[1].Position = Vector3(-0.9f, +0.9f, 0.0f);
	vertices[2].Position = Vector3(+0.9f, -0.9f, 0.0f);
	vertices[3].Position = Vector3(+0.9f, +0.9f, 0.0f);

	vertexBuffer = new VertexBuffer(vertices, 4, sizeof(Vertex));
	
}

void QuadDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(vertexBuffer);
}

void QuadDemo::Update()
{
	static Vector4 factor = Vector4(2, 2, 2, 2);
	ImGui::SliderFloat4("Factor", factor, 1,64);
	shader->AsVector("Factor")->SetFloatVector(factor);

	static Vector2 inside = Vector2(1,1);
	ImGui::SliderFloat2("inside", inside, 1, 64);
	shader->AsVector("Inside")->SetFloatVector(inside);
}

void QuadDemo::Render()
{
	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	shader->Draw(0, 0, 4);
}


