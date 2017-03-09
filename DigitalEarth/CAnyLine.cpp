#include "stdafx.h"
#include "CAnyLine.h"


CAnyLine::CAnyLine()
{
	m_Points = new osg::Vec3Array;
}


CAnyLine::~CAnyLine()
{
}

void CAnyLine::CreateLineByPoints(osg::ref_ptr<osg::Vec3Array> Points)
{
	this->m_Points = Points;
}

osg::ref_ptr<osg::Vec3Array> CAnyLine::GetLineOfPoints()
{
	if (m_Points->size() > 0)
		return m_Points;
	else
		return nullptr;
}
