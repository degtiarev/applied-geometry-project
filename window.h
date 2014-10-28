#ifndef WINDOW_H
#define WINDOW_H


class GLContextSurfaceWrapper;

// qt
#include <QQuickView>

// stl
#include <memory>

class Window : public QQuickView {
  Q_OBJECT
public:
  explicit Window(QWindow *parent = 0);


  std::shared_ptr<GLContextSurfaceWrapper>    getGLSurface() const { return _glsurface; }

  void      initGLSurface();

private:
  std::shared_ptr<GLContextSurfaceWrapper>    _glsurface;


signals:
  void      signGuiViewportChanged( const QString& name, const QRectF& new_geometry );
  void      signFrameReady();

  // Relay singals from qml side
  void      signMousePressed( const QString& name, QMouseEvent* event );
  void      signKeyPressed( const QString& name, QKeyEvent* event );
};

#endif // WINDOW_H
