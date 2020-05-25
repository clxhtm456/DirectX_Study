#include "stdafx.h"
#include "TessDistanceDemo.h"


void TessDistanceDemo::Initialize()
{
	shader = new Shader(L"46_TessDistance.fxo");

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -5);
	dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(10, 10);

	Vertex vertices[4];
	vertices[0].Position = Vector3(-10.0f, -10.0f, 0.0f);
	vertices[1].Position = Vector3(-10.0f, +10.0f, 0.0f);
	vertices[2].Position = Vector3(+10.0f, -10.0f, 0.0f);
	vertices[3].Position = Vector3(+10.0f, +10.0f, 0.0f);

	vertexBuffer = new VertexBuffer(vertices, 4, sizeof(Vertex));

	transform = new Transform(shader);
	perFrame = new PerFrame(shader);
	
}

void TessDistanceDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(vertexBuffer);
	delete transform;
	delete perFrame;
}

void TessDistanceDemo::Update()
{
	static Vector2 distance = Vector2(1,100);
	ImGui::SliderFloat2("Distance", distance, 0, 200);
	shader->AsVector("Distance")->SetFloatVector(distance);



	transform->Update();
	perFrame->Update();
}

void TessDistanceDemo::Render()
{
	vertexBuffer->Render();
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	transform->Render();
	perFrame->Render();

	shader->Draw(0, 0, 4);
}


