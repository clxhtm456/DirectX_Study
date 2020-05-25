#pragma once

#include "Systems/IExecute.h"

class ActionDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {};
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};
private:
	void Mesh();
	//void AirPlane();
	void CreatePlayer();

	void Pass(UINT mesh,UINT model,UINT anim);

private:
	Shader * shader;

	class SkyCube* sky;
	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	map<UINT,ModelAnimator*> modelManager;

	MeshRender* sphere;
	MeshRender* cylinder;
	MeshRender* grid;

	ModelRender* airPlane = nullptr;
	class Unit* player = nullptr;
	class Unit* player2 = nullptr;
	class Unit* monster[5];

	ModelCreator* playerTest;

	vector<MeshRender*> meshes;
	vector<ModelRender*> models;
	vector<ModelAnimator*> animators;
};
