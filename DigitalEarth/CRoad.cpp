#include "stdafx.h"
#include "CRoad.h"


CRoad::CRoad()
{
	m_root = new osg::Group;
}


CRoad::~CRoad()
{
}

osg::ref_ptr<osg::Geode> CRoad::CreatePerRoad(CAnyLine *line1, CAnyLine *line2)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.2f,0.6f,0.8f,1.0f));

	osg::ref_ptr<osg::Vec3Array> line1Points = line1->GetLineOfPoints();
	osg::ref_ptr<osg::Vec3Array> line2Points = line2->GetLineOfPoints();
	unsigned int line1Count, line2Count, cutoffCount;
	line1Count = line1Points->size();
	line2Count = line2Points->size();
	cutoffCount = line1Count > line2Count ? line2Count : line1Count;

	for (unsigned int i = 0; i < cutoffCount; ++i)
	{
		vertices->push_back(line1Points->at(i));
		vertices->push_back(line2Points->at(i));
	}

	gm->setVertexArray(vertices);
	gm->setColorArray(colors);
	gm->setColorBinding(osg::Geometry::BIND_OVERALL);
	gm->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLE_STRIP,0,vertices->size()));
	osgUtil::SmoothingVisitor::smooth(*gm);
	

	geode->addDrawable(gm);
	return geode.release();
}

void CRoad::AddRoad(osg::ref_ptr<osg::Geode> perRoad)
{
	m_root->addChild(perRoad);
}

osg::ref_ptr<osg::Group> CRoad::GetRoad()
{
	return m_root.release();
}

void CRoad::InitRoad()
{
	CAnyLine *line1, *line2,*line3;
	line1 = new CAnyLine;
	line2 = new CAnyLine;
	line3 = new CAnyLine;
	osg::ref_ptr<osg::Vec3Array> line1Points = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> line2Points = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> line3Points = new osg::Vec3Array;
	for (float i = -180; i <= 180.0; i += 5.0)
	{
		float h = osg::DegreesToRadians(i);
		float x =2*cos(h);
		float y = 2 * sin(h);
		float z = sin(h);
		line1Points->push_back(osg::Vec3(x,y,z));
	}
	for (float i = -180; i <=180.0; i += 5.0)
	{
		float h = osg::DegreesToRadians(i);
		float x = 4 * cos(h);
		float y = 4 * sin(h);
		float z = sin(h);
		line2Points->push_back(osg::Vec3(x, y, z));
	}
	for (float i = -180.0; i <=180.0; i += 5.0)
	{
		float h = osg::DegreesToRadians(i);
		float x = 3 * cos(h);
		float y = 3 * sin(h);
		float z = cos(h);
		line3Points->push_back(osg::Vec3(x, y, z));
	}

	line1->CreateLineByPoints(line1Points);
	line2->CreateLineByPoints(line2Points);
	line3->CreateLineByPoints(line3Points);
	AddRoad(this->CreatePerRoad(line1, line3));
	AddRoad(this->CreatePerRoad(line3, line2));
}

