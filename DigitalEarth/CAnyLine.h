#pragma once
#include "common.h"
class CAnyLine
{
public:
	CAnyLine();
	~CAnyLine();
	void CreateLineByPoints(osg::ref_ptr<osg::Vec3Array> Points);
	osg::ref_ptr<osg::Vec3Array> GetLineOfPoints();
private:
	osg::ref_ptr<osg::Vec3Array> m_Points;
};

