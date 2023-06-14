#include "Critter.h"

#include <iostream>

Critter::Critter()
{
	m_position = Vector2{ 0, 0 };
	m_velocity = Vector2{ 0, 0 };
	m_radius = 0;
	m_isLoaded = false;
}
Critter::~Critter()
{ }

void Critter::Init(Vector2 position, Vector2 velocity, float radius, const Texture2D &texture)
{
	m_position = position;
	m_velocity = velocity;
	m_radius = radius;
	
	m_texture = texture;

	m_isLoaded = true;
}

void Critter::Destroy()
{ 
	m_isLoaded = false;
}

void Critter::Update(float dt)
{
	if (m_isLoaded == false)
		return;

	m_position.x += m_velocity.x * dt;
	m_position.y += m_velocity.y * dt;

	m_isDirty = false;
}


void Critter::Draw()
{
	if (m_isLoaded == false)
		return;

	DrawTexturePro(m_texture, 
		{ 0, 0, (float)m_texture.width, (float)m_texture.height }, 
		{ m_position.x, m_position.y, (float)m_texture.width, (float)m_texture.height }, 
		{ (float)m_texture.width/2, (float)m_texture.height/2 }, 
		0.0f, WHITE);
}

CritterPool::CritterPool(unsigned int poolSize)
	: m_poolSize { poolSize }
{ }
CritterPool::~CritterPool()
{ 
	for (int i = 0; i < m_poolSize; i++)
	{
		m_activeCritters[i]->Destroy();
	}

	m_activeCritters.Clear();
	m_inactiveCritters.Clear();
}

void CritterPool::AllocateObject(Critter *critter)
{
	// Remove from Inactive List and put in Active List
	if (m_activeCritters.Count() < m_poolSize) // Don't exceed size of pool
	{
		m_inactiveCritters.Remove(critter);
		m_activeCritters.PushBack(critter);
	}
}
void CritterPool::DeallocateObject(Critter *critter)
{
	// Remove from Active List and put in Inactive List
	m_activeCritters.Remove(critter);
	m_inactiveCritters.PushBack(critter);
}

void CritterPool::InitObject(int index, Vector2 position, Vector2 velocity, float radius, const Texture2D &texture)
{
	if (m_activeCritters.IsEmpty())
		return;
	Critter *critter = m_activeCritters[index];
	critter->Init(position, velocity, radius, texture);
}
void CritterPool::DestroyObject(int index)
{
	if (m_activeCritters.IsEmpty())
		return;
	Critter *critter = m_activeCritters[index];
	critter->Destroy();
}

void CritterPool::Update(float dt)
{
	if (m_activeCritters.IsEmpty())
		return;
	for (int i = 0; i < m_poolSize; i++)
	{
		m_activeCritters[i]->Update(dt);
	}
}
void CritterPool::Draw()
{
	if (m_activeCritters.IsEmpty())
		return;
	for (int i = 0; i < m_poolSize; i++)
	{
		m_activeCritters[i]->Draw();
	}
}
