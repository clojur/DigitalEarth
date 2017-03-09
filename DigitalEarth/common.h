#pragma  once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#include <osgViewer/Viewer>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgViewer/ViewerEventHandlers>

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>
#include<osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>

#include <osg/Group>
#include <osg/Geometry>
#include <osg/Vec4>
#include <osg/ShapeDrawable>
#include <osgEarth/MapNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/Sky>
#include <osgEarth/SpatialReference>