#include "pch.h"
#include "CComponent.h"

Component::Component(COMPONENT_TYPE _type)
	:m_Type(_type)
	, m_pOwner(nullptr)
{
}

Component::~Component()
{
}
