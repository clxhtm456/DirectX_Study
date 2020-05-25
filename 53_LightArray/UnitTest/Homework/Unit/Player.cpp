#include "stdafx.h"
#include "Player.h"

Player::Player(ModelCreator* model):
	Unit(model)
{
	auto init = new Transform();
	init->Scale(10, 10, 50);
	init->Position(-10, 0, -30);

	auto transform = new Transform();
	//collider = new Collider(transform, init);
}

Player::~Player()
{
}

void Player::Update()
{
	Super::Update();

	/*Matrix attach;
	GetTransform()->World(attach);
	collider->GetTransform()->World(attach);
	collider->GetTransform()->Update();*/
}