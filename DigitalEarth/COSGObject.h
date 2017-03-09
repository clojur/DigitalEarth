#pragma once
#include "common.h"
class COSGObject
{
public:
	COSGObject(HWND hWnd);
	~COSGObject();
	void InitOSG();
	void InitCameraConfig();
	void InitSceneGraph(std::string str);
	void InitManipulator();

	void PreFrameUpdate();
	void PostFrameUpdate();
	void Render(void *ptr);
	void RemoveModel();
	void AddNode(osg::ref_ptr<osg::Node> node);
	osgViewer::Viewer* GetViewer();
	std::mutex *app_mutex;
private://≤‚ ‘
	osg::ref_ptr<osg::Group> testCull();
	osg::ref_ptr<osg::Group> CreatePageLOD();

private:
	HWND m_hWnd;
	osgViewer::Viewer *m_viewer;
	osg::ref_ptr<osg::Group> m_root;
	osg::ref_ptr<osg::Node> m_node;
	osg::ref_ptr<osgEarth::MapNode> m_mapNode;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> m_em;
};

