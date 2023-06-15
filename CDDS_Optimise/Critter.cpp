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
	if (!m_activeCritters.IsEmpty())
		m_activeCritters.DeleteAll();
	if (!m_inactiveCritters.IsEmpty())
		m_inactiveCritters.DeleteAll();
}

void CritterPool::AllocateObject(Critter *critter)
{
	// Remove from Inactive List and put in Active List
	if (m_activeCritters.Count() < m_poolSize) // Don't exceed size of pool
	{
		if (!m_inactiveCritters.IsEmpty())
			m_inactiveCritters.Remove(critter);
		m_activeCritters.PushBack(critter);
	}
}
Critter *CritterPool::AllocateObject(int index)
{
	if (m_activeCritters.Count() < m_poolSize) // Don't exceed size of pool
	{
		Critter *critter = m_inactiveCritters[index];
		if (!m_inactiveCritters.IsEmpty())
			m_inactiveCritters.Remove(critter);
		m_activeCritters.PushBack(critter);
		
		return critter;
	}
	return nullptr;
}
Critter *CritterPool::AllocateObject()
{
	if (m_activeCritters.Count() < m_poolSize) // Don't exceed size of pool
	{
		Critter *critter = m_inactiveCritters.First()->data;
		if (!m_inactiveCritters.IsEmpty())
			m_inactiveCritters.Remove(critter);
		m_activeCritters.PushBack(critter);
		return critter;
	}
	return nullptr;
}
void CritterPool::DeallocateObject(Critter *critter)
{
	// Remove from Active List and put in Inactive List
	if (!m_activeCritters.IsEmpty())
		m_activeCritters.Remove(critter);
	m_inactiveCritters.PushBack(critter);
}
Critter *CritterPool::DeallocateObject(int index)
{
	Critter *critter = m_activeCritters[index];
	if (!m_activeCritters.IsEmpty())
		m_activeCritters.Remove(critter);
	m_inactiveCritters.PushBack(critter);
	return critter;
}

void CritterPool::Update(float dt)
{
	if (m_activeCritters.IsEmpty())
		return;
	for (int i = 0; i < m_activeCritters.Count(); i++)
	{
		m_activeCritters[i]->Update(dt);
	}
}
void CritterPool::Draw()
{
	if (m_activeCritters.IsEmpty())
		return;
	for (int i = 0; i < m_activeCritters.Count(); i++)
	{
		m_activeCritters[i]->Draw();
	}
}

float CritterPool::GetX(int index)
{
	if (m_activeCritters.IsEmpty())
		return 0.0f;
	return m_activeCritters[index]->GetX();
}
float CritterPool::GetY(int index)
{
	if (m_activeCritters.IsEmpty())
		return 0.0f;
	return m_activeCritters[index]->GetY();
}
void CritterPool::SetX(int index, float x)
{
	if (m_activeCritters.IsEmpty())
		return;
	m_activeCritters[index]->SetX(x);
}
void CritterPool::SetY(int index, float y)
{
	if (m_activeCritters.IsEmpty())
		return;
	m_activeCritters[index]->SetY(y);
}
Vector2 CritterPool::GetPosition(int index)
{
	if (m_activeCritters.IsEmpty())
		return { 0.0f, 0.0f };
	return m_activeCritters[index]->GetPosition();
}
void CritterPool::SetPosition(int index, Vector2 position)
{
	if (m_activeCritters.IsEmpty())
		return;
	m_activeCritters[index]->SetPosition(position);
}
Vector2 CritterPool::GetVelocity(int index)
{
	if (m_activeCritters.IsEmpty())
		return { 0.0f, 0.0f };
	return m_activeCritters[index]->GetVelocity();
}
void CritterPool::SetVelocity(int index, Vector2 velocity)
{
	if (m_activeCritters.IsEmpty())
		return;
	m_activeCritters[index]->SetVelocity(velocity);
}
float CritterPool::GetRadius(int index)
{
	if (m_activeCritters.IsEmpty())
		return 0.0f;
	return m_activeCritters[index]->GetRadius();
}
bool CritterPool::IsDirty(int index)
{
	if (m_activeCritters.IsEmpty())
		return false;
	return m_activeCritters[index]->IsDirty();
}
void CritterPool::SetDirty(int index)
{
	if (m_activeCritters.IsEmpty())
		return;
	m_activeCritters[index]->SetDirty();
}
bool CritterPool::IsDead(int index)
{
	if (m_inactiveCritters.IsEmpty())
		return false;
	return m_inactiveCritters[index]->IsDead();
}
