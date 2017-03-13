#include "stdafx.h"
#include "COSGObject.h"
#include "CRoad.h"

COSGObject::COSGObject(HWND hWnd) :m_hWnd(hWnd)
{
	app_mutex = new std::mutex;
}


COSGObject::~COSGObject()
{
	delete app_mutex;
}

void COSGObject::InitOSG()
{
	m_viewer = new osgViewer::Viewer;
	InitSceneGraph("earth");
	InitCameraConfig();
}

void COSGObject::InitCameraConfig()
{
	RECT rect;
	::GetWindowRect(m_hWnd, &rect);
	osg::ref_ptr<osg::GraphicsContext::Traits> traits =new osg::GraphicsContext::Traits;
	osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(m_hWnd);
	traits->x = 0;
	traits->y = 0;
	traits->width = rect.right - rect.left;
	traits->height = rect.bottom - rect.top;
	traits->doubleBuffer = true;
	traits->windowDecoration = false;
	traits->sharedContext = 0;
	traits->setInheritedWindowPixelFormat = true;
	traits->inheritedWindowData = windata;
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
	osg::ref_ptr <osg::Camera> masterCamera = m_viewer->getCamera();
	masterCamera->setGraphicsContext(gc);
	masterCamera->setViewport(0, 0, traits->width, traits->height);
	masterCamera->setProjectionMatrixAsPerspective(45.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 0.1f, 1000.0f);
	masterCamera->setNearFarRatio(0.000001f);
	
	m_viewer->setCamera(masterCamera);
	//m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	m_viewer->addEventHandler(new osgGA::StateSetManipulator(m_root->getOrCreateStateSet()));
	m_viewer->addEventHandler(new osgViewer::WindowSizeHandler);
	m_viewer->addEventHandler(new osgViewer::StatsHandler);
	m_viewer->setSceneData(m_root);
	m_viewer->realize();
	m_viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES);
	m_viewer->getCamera()->setNearFarRatio(0.000003f);
	
}

void COSGObject::InitSceneGraph(std::string str)
{
	m_root = new osg::Group;
	if (str == "road")
	{
		CRoad *MyRoad = new CRoad;
		MyRoad->InitRoad();
		m_node = MyRoad->GetRoad();
		m_root->addChild(m_node);
		InitManipulator();
	}
	if (str == "earth")
	{
		m_node = osgDB::readNodeFile("../earthResource/earthfile/MyEarth.earth");//../earthResource/earthfile/MyEarth.earth
		m_mapNode = dynamic_cast<osgEarth::MapNode*>(m_node.get());
		osg::ref_ptr<osg::ClearNode> clearNode = new osg::ClearNode;
		clearNode->setClearColor(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
		m_root->addChild(clearNode);
		m_root->addChild(m_node);
		InitManipulator();
	}
	if (str == "test")
	{
		m_node = testCull();//*/ CreatePageLOD()/*;
		m_root->addChild(m_node);
		InitManipulator();
	}
}

void COSGObject::InitManipulator()
{
	//初始化操作器
	if (m_mapNode.valid())
	{
		m_em = new osgEarth::Util::EarthManipulator;
		m_em->setNode(m_mapNode);
		m_em->getSettings()->setArcViewpointTransitions(true);
		m_viewer->setCameraManipulator(m_em);
		//初始化天空
		m_mapNode->addExtension(osgEarth::Extension::create("sky_simple", osgEarth::ConfigOptions()));
	}
	else
	{
		m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
	}	
}

void COSGObject::Render(void *ptr)
{
	COSGObject *osg = (COSGObject*)ptr;
	osgViewer::Viewer *viewer = osg->GetViewer();
	while (!viewer->done())
	{
		//app_mutex->lock();
		osg->PreFrameUpdate();
		viewer->frame();
		osg->PostFrameUpdate();
		//app_mutex->unlock();
	}
}

void COSGObject::PostFrameUpdate()
{

}

void COSGObject::PreFrameUpdate()
{

}

osgViewer::Viewer* COSGObject::GetViewer()
{
	return m_viewer;
}

void COSGObject::RemoveModel()
{
	app_mutex->lock();
	m_root->removeChild(m_node);
	m_node.~ref_ptr();
	app_mutex->unlock();
}

void COSGObject::AddNode(osg::ref_ptr<osg::Node> node)
{
	m_node = node;
	app_mutex->lock();
	m_root->addChild(m_node);
	app_mutex->unlock();
}

osg::ref_ptr<osg::Group> COSGObject::testCull()
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	osg::ref_ptr<osg::Box> box1 = new osg::Box(osg::Vec3(5.0f,0.0f,5.0f),2.0f);
	osg::ref_ptr<osg::Box> box2 = new osg::Box(osg::Vec3(5.0f,5.0f,5.0f),2.0f);
	osg::ref_ptr<osg::Box> box3 = new osg::Box(osg::Vec3(-5.0f, 5.0f, 5.0f), 2.0f);
	osg::ref_ptr<osg::TessellationHints> hints = new osg::TessellationHints;
	hints->setDetailRatio(0.2f);
	osg::ref_ptr<osg::ShapeDrawable> sd1 = new osg::ShapeDrawable(box1,hints);
	osg::ref_ptr<osg::ShapeDrawable> sd2 = new osg::ShapeDrawable(box2,hints);
	osg::ref_ptr<osg::ShapeDrawable> sd3 = new osg::ShapeDrawable(box3, hints);
	sd1->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
	//osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	//pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	//sd1->getOrCreateStateSet()->setAttributeAndModes(pm,osg::StateAttribute::ON);
	sd2->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	sd3->setColor(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
	geode->addDrawable(sd1);
	geode->addDrawable(sd2);
	geode->addDrawable(sd3);
	root->addChild(geode);
	return root.release();
}

osg::ref_ptr<osg::Group> COSGObject::CreatePageLOD()
{
	osg::ref_ptr<osg::PagedLOD> pl = new osg::PagedLOD;
	pl->setCenter(osg::Vec3(0.0f,0.0f,0.0f));
	pl->setFileName(0, "axes.osgt");
	pl->setRange(0, 0.0f, 5.0f);
	pl->setFileName(1, "glider.osg");
	pl->setRange(1, 5.0f, 50.0f);
	pl->setFileName(2, "cow1.osg");
	pl->setRange(2, 50.0f,80.0f);
	pl->setFileName(3, "ground1.osg");
	pl->setRange(3,80.0f,FLT_MAX);
	return pl.release();
}
