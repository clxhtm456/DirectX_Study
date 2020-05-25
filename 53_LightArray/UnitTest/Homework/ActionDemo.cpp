#include "stdafx.h"
#include "ActionDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"
#include "Environment/SkyCube.h"
#include "Unit/Player.h"

enum ModelIndex
{
	INDEX_KACHUJIN = 0,
	INDEX_MONSTER = 1
};

static pair<UINT, wstring> ModelDir[] =
{
	make_pair(INDEX_KACHUJIN,L"Arissa/arissa")
};

static pair<UINT, wstring> AnimDir[] =
{
	make_pair(INDEX_KACHUJIN,L"Arissa/idle"),
	make_pair(INDEX_KACHUJIN,L"Arissa/nidle"),
	make_pair(INDEX_KACHUJIN,L"Arissa/aim"),
	make_pair(INDEX_KACHUJIN,L"Arissa/idle03"),
	make_pair(INDEX_KACHUJIN,L"Arissa/kick"),
	make_pair(INDEX_KACHUJIN,L"Arissa/run")
};

void ActionDemo::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 32, -67);
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(50, 5);

	shader = new Shader(L"38_Lighting.fx");
	sky = new SkyCube(L"Environment/GrassCube1024.dds");

	Mesh();

	playerTest = ModelCreator::Create(shader, L"Kachujin/Mesh");
	playerTest->AddAnimation(L"Kachujin/Idle");
	playerTest->AddAnimation(L"Kachujin/Running");
	playerTest->AddAnimation(L"Kachujin/Jump");
	playerTest->AddAnimation(L"Kachujin/Hip_Hop_Dancing");
	playerTest->Pass(2);

	player = new Player(playerTest);
	player->SetPosition(-25, 0, -30);
	player->SetScale(0.075f, 0.075f, 0.075f);

	auto playerTest2 = ModelCreator::Create(shader, L"Kachujin/Mesh");

	player2 = new Player(playerTest2);
	player2->SetPosition(10, 0, -30);
	player2->SetScale(0.075f, 0.075f, 0.075f);
	player2->PlayAnimation(1);
}

void ActionDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(sky);

	for (auto pair : modelManager)
	{
		delete pair.second;
	}
}

void ActionDemo::Update()
{
	sky->Update();

	sphere->Update();
	cylinder->Update();
	grid->Update();

	player->Update();

	//player2->Update();

	ModelCreator::Update();


}

void ActionDemo::Render()
{
	sky->Render();

	Pass(0, 1, 2);

	wall->Render();
	sphere->Render();

	brick->Render();
	cylinder->Render();

	stone->Render();

	floor->Render();
	grid->Render();

	ModelCreator::Render();
}

void ActionDemo::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->SpecularMap("Floor_Specular.png");
		floor->NormalMap("Floor_Normal.png");
		floor->Specular(1, 1, 1, 20);

		stone = new Material(shader);
		stone->DiffuseMap("Stones.png");
		stone->SpecularMap("Stones_Specular.png");
		stone->NormalMap("Stones_Normal.png");
		stone->Specular(1, 1, 1, 20);

		brick = new Material(shader);
		brick->DiffuseMap("Bricks.png");
		brick->SpecularMap("Bricks_Specular.png");
		brick->NormalMap("Bricks_Normal.png");
		brick->Specular(1, 1, 1, 20);

		wall = new Material(shader);
		wall->DiffuseMap("Wall.png");
		wall->SpecularMap("Wall_Specular.png");
		wall->NormalMap("Wall_Normal.png");
		wall->Specular(1, 1, 1, 20);
		wall->Emissive(0.3f, 0.3f, 0.3f, 0.3f);
	}

	//Create Mesh
	{
		Transform* transform = NULL;

		grid = new MeshRender(shader, new MeshGrid(5, 5));
		transform = grid->AddTransform();
		transform->Position(0, 0, 0);
		transform->Scale(12, 1, 12);

		cylinder = new MeshRender(shader, new MeshCylinder(0.5f, 3.0f, 20, 20));
		sphere = new MeshRender(shader, new MeshSphere(0.5f, 20, 20));
		for (UINT i = 0; i < 5; i++)
		{
			transform = cylinder->AddTransform();
			transform->Position(-30, 6, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = cylinder->AddTransform();
			transform->Position(30, 6, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = sphere->AddTransform();
			transform->Position(-30, 15.5f, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);

			transform = sphere->AddTransform();
			transform->Position(30, 15.5f, -15.0f + (float)i * 15.0f);
			transform->Scale(5, 5, 5);
		}
	}

	sphere->UpdateTransforms();
	cylinder->UpdateTransforms();
	grid->UpdateTransforms();

	meshes.push_back(sphere);
	meshes.push_back(cylinder);
	meshes.push_back(grid);
}


void ActionDemo::CreatePlayer()
{
	//auto model = modelManager.at(INDEX_KACHUJIN);

	player = new Player(playerTest);
}



void ActionDemo::Pass(UINT mesh, UINT model, UINT anim)
{
	for (MeshRender* temp : meshes)
		temp->Pass(mesh);

	for (ModelRender* temp : models)
		temp->Pass(model);

	for (ModelAnimator* temp : animators)
		temp->Pass(anim);
}

