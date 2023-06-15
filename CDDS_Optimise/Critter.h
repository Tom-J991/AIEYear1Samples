#pragma once

#include "raylib.h"

#include "DoublyLinkedList.h"

class Critter
{
protected:	
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;

	Texture2D m_texture;

	bool m_isLoaded;
	bool m_isDirty;		// indicates if we've already processed a collision response for this critter
	
public:
	Critter();
	~Critter();

	void Init(Vector2 position, Vector2 velocity, float radius, const Texture2D &texture);
	void Destroy();
	void Update(float dt);
	void Draw();

	float GetX() { return m_position.x; }
	float GetY() { return m_position.y; }
	void SetX(float x) { m_position.x = x; }
	void SetY(float y) { m_position.y = y; }

	Vector2 GetPosition() { return m_position; }
	void SetPosition(Vector2 position) { m_position = position; }

	Vector2 GetVelocity() { return m_velocity; }
	void SetVelocity(Vector2 velocity) { m_velocity = velocity; }

	float GetRadius() { return m_radius; }

	bool IsDirty() { return m_isDirty; }
	void SetDirty() { m_isDirty = true; }

	bool IsDead() { return m_isLoaded == false; }

};

class CritterPool
{
public:
	CritterPool(unsigned int poolSize);
	~CritterPool();

	void AllocateObject(Critter *critter);
	Critter *AllocateObject(int index);
	Critter *AllocateObject();
	void DeallocateObject(Critter *critter);
	Critter *DeallocateObject(int index);

	void Update(float dt);
	void Draw();

	float GetX(int index);
	float GetY(int index);
	void SetX(int index, float x);
	void SetY(int index, float y);
	Vector2 GetPosition(int index);
	void SetPosition(int index, Vector2 position);
	Vector2 GetVelocity(int index);
	void SetVelocity(int index, Vector2 velocity);
	float GetRadius(int index);
	bool IsDirty(int index);
	void SetDirty(int index);
	bool IsDead(int index);

	unsigned int ActiveObjects() const { return m_activeCritters.Count(); }
	unsigned int InactiveObjects() const { return m_inactiveCritters.Count(); }
	unsigned int PoolCount() const { return m_activeCritters.Count() + m_inactiveCritters.Count(); }

private:
	unsigned int m_poolSize;

	DoublyLinkedList<Critter> m_activeCritters;
	DoublyLinkedList<Critter> m_inactiveCritters;

};
