#include "scenario.h"

#include "testtorus.h"
#include "mycurve.h"
#include "myspline.h"
#include "mycurveblender2.h"
#include "myGERBScurve4.h"
#include "myGERBScurve6.h"

//// hidmanager
//#include "hidmanager/defaulthidmanager.h"

// gmlib
#include <gmOpenglModule>
#include <gmSceneModule>
#include <gmParametricsModule>

// qt
#include <QQuickItem>



void Scenario::initializeScenario() {

  // Insert a light
  GMlib::Point<GLfloat,3> init_light_pos( 2.0, 4.0, 10 );
  GMlib::PointLight *light = new GMlib::PointLight(  GMlib::GMcolor::white(), GMlib::GMcolor::white(),
                                                     GMlib::GMcolor::white(), init_light_pos );
  light->setAttenuation(0.8, 0.002, 0.0008);
  scene()->insertLight( light, false );

  // Insert Sun
  scene()->insertSun();

  // Default camera parameters
  int init_viewport_size = 600;
  GMlib::Point<float,3> init_cam_pos(  0.0f, 0.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_dir( 0.0f, 1.0f, 0.0f );
  GMlib::Vector<float,3> init_cam_up(  0.0f, 0.0f, 1.0f );

  // Projection cam
  auto proj_rcpair = createRCPair("Projection");
  proj_rcpair.camera->set(init_cam_pos,init_cam_dir,init_cam_up);
  proj_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  proj_rcpair.camera->rotateGlobal( GMlib::Angle(-45), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ) );
  proj_rcpair.camera->translateGlobal( GMlib::Vector<float,3>( 0.0f, -20.0f, 20.0f ) );
  scene()->insertCamera( proj_rcpair.camera.get() );
  proj_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Front cam
  auto front_rcpair = createRCPair("Front");
  front_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, -50.0f, 0.0f ), init_cam_dir, init_cam_up );
  front_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( front_rcpair.camera.get() );
  front_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Side cam
  auto side_rcpair = createRCPair("Side");
  side_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( -50.0f, 0.0f, 0.0f ), GMlib::Vector<float,3>( 1.0f, 0.0f, 0.0f ), init_cam_up );
  side_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( side_rcpair.camera.get() );
  side_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );

  // Top cam
  auto top_rcpair = createRCPair("Top");
  top_rcpair.camera->set( init_cam_pos + GMlib::Vector<float,3>( 0.0f, 0.0f, 50.0f ), -init_cam_up, init_cam_dir );
  top_rcpair.camera->setCuttingPlanes( 1.0f, 8000.0f );
  scene()->insertCamera( top_rcpair.camera.get() );
  top_rcpair.renderer->reshape( GMlib::Vector<int,2>(init_viewport_size, init_viewport_size) );







  // Surface visualizers
  // auto surface_visualizer = new GMlib::PSurfNormalsVisualizer<float,3>;

  // Surface
//  auto surface = new TestTorus;
//  surface->toggleDefaultVisualizer();
//  surface->insertVisualizer(surface_visualizer);
//  surface->replot(200,200,1,1);
//  scene()->insert(surface);

  // My curve
  auto mycurve = new GMlib::MyCurve<float>();
  mycurve->toggleDefaultVisualizer();
  mycurve->replot(200,0);
  scene()->insert(mycurve);

  // My spline
  GMlib::DVector<GMlib::Vector<float,3>>c(8);
    c[0] = GMlib::Vector<float,3>(0,0,0);
    c[1] = GMlib::Vector<float,3>(1,1,0);
    c[2] = GMlib::Vector<float,3>(2,2,0);
    c[3] = GMlib::Vector<float,3>(3,3,0);
    c[4] = GMlib::Vector<float,3>(4,3,0);
    c[5] = GMlib::Vector<float,3>(5,1,0);
    c[6] = GMlib::Vector<float,3>(6,0.5,0);
    c[7] = GMlib::Vector<float,3>(7,0,0);

  myPCurve = new GMlib::MSpline<float>(c, 2, 8);
  myPCurve->toggleDefaultVisualizer();
  myPCurve->replot(200,0);
  scene()->insert(myPCurve);

  // My blender 2
  GMlib::MyCurve<float>* myCurve1 = new GMlib::MyCurve<float>();
  GMlib::MyCurve<float>* myCurve2 = new GMlib::MyCurve<float>();
  myCurve2->translate(GMlib::Vector<float,3>(0,3,0));
  myCurve1->toggleDefaultVisualizer();
  myCurve1->replot(200,0);
  scene()->insert(myCurve1);

  myCurve2->toggleDefaultVisualizer();
  myCurve2->replot(200,0);
  scene()->insert(myCurve2);

  auto myMultiCurve = new GMlib::MyCurveBlender2<float>(myCurve1, myCurve2,0.3);
  myMultiCurve->toggleDefaultVisualizer();
  myMultiCurve->setColor(GMlib::GMcolor::blue());
  myMultiCurve->replot(200,0);
  scene()->insert(myMultiCurve);

  // my GERBS curve 4
  mybcurve = new GMlib::MyGERBScurve4<float>(myCurve1,8);
  mybcurve->toggleDefaultVisualizer();
  mybcurve->translate(GMlib::Vector<float,3>(3,0,0));
  mybcurve->replot(200,0);
  scene()->insert(mybcurve);

  // my GERBS curve 6
  auto myMSurface = new GMlib::PPlane<float>(GMlib::Point<float,3>(-10.0f, 10.0f, 20.0f),
                             GMlib::Vector<float,3>(0.0f, -20.0f, 0.0f),
                             GMlib::Vector<float,3>(0.0f, 0.0f, -20.0f));
  auto normalVis = new GMlib::PSurfNormalsVisualizer<float,3>();

  mybsurfe = new GMlib::MyGERBScurve6<float>(myMSurface,4,4);
  mybsurfe->toggleDefaultVisualizer();
  mybsurfe->insertVisualizer(normalVis);
  mybsurfe->replot(50,50,1,1);
  scene()->insert(mybsurfe);


  //surface->test01();

}

void Scenario::cleanupScenario() {

}

void Scenario::callGL()
{
    if (myPCurve)
        myPCurve->replot();

    if (mybcurve){
        mybcurve->replot(100,0);
    }

    if (mybsurfe){
        mybsurfe->replot(50,50,1,1);
    }
}
