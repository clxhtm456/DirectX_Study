#include "stdafx.h"
#include "Unit.h"

Unit::Unit(ModelCreator* model)
{
	AddModel(model);
}

Unit::~Unit()
{
}

void Unit::AddModel(ModelCreator* model)
{
	this->model = model;
	
	model->PlayClip( 0, 0.25f);
}

void Unit::SetPosition(float x, float y, float z)
{
	if (model == nullptr)
		return;

	Transform* transform = nullptr;

	transform = model->GetTransform();
	transform->Position(x, y, z);
}

void Unit::SetScale(float x, float y, float z)
{
	if (model == nullptr)
		return;

	Transform* transform = nullptr;

	transform = model->GetTransform();
	transform->Scale(x, y, z);
}

void Unit::PlayAnimation(UINT clip, float speed, float takeTime)
{
	model->PlayClip(clip, speed, takeTime);
}

