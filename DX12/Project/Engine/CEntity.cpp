#include "pch.h"
#include "CEntity.h"

UINT Entity::g_NextID = 0;

Entity::Entity()
	:m_ID(g_NextID++)
{
}

Entity::Entity(const Entity& _origin)
	:m_ID(g_NextID++)
	, m_strName(_origin.m_strName)
{
}

Entity::~Entity()
{
}