#pragma once
#include "CAnyLine.h"
class CRoad
{
public:
	CRoad();
	~CRoad();
	osg::ref_ptr<osg::Geode> CreatePerRoad(CAnyLine *line1,CAnyLine *line2);
	void AddRoad(osg::ref_ptr<osg::Geode> perRoad);
	osg::ref_ptr<osg::Group> GetRoad();
	void InitRoad();
private:
	std::vector<CAnyLine*> m_Lines;
	osg::ref_ptr<osg::Group> m_root;
};

