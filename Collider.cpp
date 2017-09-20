#include "Collider.h"

#include "Primitive.h"

Collider::Collider(COLLIDER_TYPE type, bool render) :
	Collider(float3(0.f, 0.f, 0.f), type, render)
{}

Collider::Collider(vec initial_pos, COLLIDER_TYPE type, bool render) :
	pos(initial_pos), type(type), render(render), shape(nullptr)
{
	SetShape(type);
}

Collider::~Collider()
{
	delete shape;
}

bool Collider::Intersect(Collider* other) const
{
	bool ret = false;

	switch (type)
	{
	case C_CUBE:
	{
		switch (other->type)
		{
		case C_CUBE:
		{
			// cube - cube
			break;
		}
		case C_SPHERE:
		{
			// cube - sphere
			break;
		}
		}

		break;
	}
	case C_SPHERE:
	{
		switch (other->type)
		{
		case C_CUBE:
		{
			// sphere - cube
			break;
		}
		case C_SPHERE:
		{
			// sphere - sphere
			break;
		}
		}

		break;
	}
	}

	return ret;
}

void Collider::Update()
{
	if (render && shape)
		shape->Render();
}


void Collider::SetShape(COLLIDER_TYPE new_type)
{
	if (shape != nullptr)
	{
		delete shape;
		shape = nullptr;
	}

	type = new_type;
	switch (type)
	{
	case C_CUBE:
	{
		// collider cube's dimmension depends on AABB's x,y and z dimensions
		shape = new PrimCube(1.f, 1.f, 1.f);
		break;
	}
	case C_SPHERE:
	{
		// collider sphere radius is half AABB's diagonal dimmension
		shape = new PrimSphere(1.f);
		break;
	}
	}
}

