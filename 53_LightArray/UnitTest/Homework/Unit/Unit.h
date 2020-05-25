#pragma once

class Unit
{
public:
	Unit(ModelCreator* model);
	virtual ~Unit();

	virtual void JustDied(Unit* target) {}
	virtual void Update() {}
	virtual void DamageTaken(Unit* target){}

	void SetPosition(float x, float  y, float z);
	void SetScale(float x, float  y, float z);

	void PlayAnimation(UINT clip, float speed = 1.0f, float takeTime = 1.0f);
protected:
	void AddModel(ModelCreator* model);
private:
	Collider* collider;
	ModelCreator* model;
};