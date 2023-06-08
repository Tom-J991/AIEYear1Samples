#pragma once


#include "raylib.h"

class Critter
{
protected:	
	Vector2 m_position;
	Vector2 m_velocity;
	float m_radius;

	Texture2D m_texture;

	bool m_isLoaded;
	bool m_isDirty;		// indicates if we've already processed a collision response for this critter

	Critter *m_next;
	Critter *m_previous;
	
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

	Critter *GetNext() const { return m_next; }
	void SetNext(Critter *next) { m_next = next; }

	Critter *GetPrevious() const { return m_previous; }
	void SetPrevious(Critter *previous) { m_previous = previous; }

};

class CritterPool
{
public:
	CritterPool(unsigned int poolSize);
	~CritterPool();

	void Create(Vector2 position, Vector2 velocity, float radius, const Texture2D &texture);
	void Destroy();

	unsigned int GetPoolSize() const { return m_poolSize; }
	unsigned int GetActiveCount() const { return m_activeCount; }

private:
	unsigned int m_poolSize;
	unsigned int m_activeCount;
	Critter *m_critters;

};

