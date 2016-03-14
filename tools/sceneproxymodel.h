#ifndef SCENEPROXYMODEL_H
#define SCENEPROXYMODEL_H



// local
namespace placeholders {

  class Root{};
  class ComponentGroup{};
}
class GMlibWrapper;

// GMlib
namespace GMlib {
  class Scene;
  class SceneObject;
}
#include <core/gmarray>

// Qt
#include <QAbstractItemModel>


// stl
#include <memory>


class SceneProxyModel : public QAbstractItemModel {
  Q_OBJECT
public:
  SceneProxyModel( std::shared_ptr<GMlibWrapper> gmlib = nullptr);



  QModelIndex                   indexFromSceneObject( GMlib::SceneObject* );

  Q_INVOKABLE virtual QVariant  getSceneProperty( const QString& name ) const;
  Q_INVOKABLE virtual bool      setSceneProperty( const QString& name, const QString& value );

  Q_INVOKABLE QVariantList      getPropertyModules( const QModelIndex& index ) const;

  Q_INVOKABLE virtual QVariant  getProperty( const QModelIndex& index, int module, const QString& name ) const;
  QVariant                      getSceneObjectProperty( const GMlib::SceneObject& sceneobject, const QString& name ) const;


  Q_INVOKABLE virtual bool      setProperty( const QModelIndex& index, int module, const QString& name, const QVariant& value );
  bool                          setSceneObjectProperty( GMlib::SceneObject& sceneobject, const QString& name, const QVariant& value );



  // From QAbstractItemModel
  QModelIndex                   index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
  QModelIndex                   parent( const QModelIndex &child ) const override;
  int                           rowCount( const QModelIndex &parent = QModelIndex() ) const override;
  int                           columnCount( const QModelIndex &parent = QModelIndex() ) const override;
  QVariant                      data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
  Qt::ItemFlags                 flags( const QModelIndex &index ) const override;

  QHash<int,QByteArray>         roleNames() const override;

  bool                          setData(const QModelIndex &index, const QVariant &value, int role) override;




  enum PropertyModules : int {
    SceneObject   = 0,
    Camera,

    PSurf,
    PERBSSurf,
    PTorus,

    CustomModules = 100
  };
  Q_ENUM(PropertyModules)


public slots:
  void                      stupidForceModelUpdate();

private:
  enum class UserRoles : int {
    Identity = Qt::UserRole + 1,
    Name,
    Visible,
    MemoryAddress
  };



  std::shared_ptr<GMlibWrapper>               _gmlib;

//  mutable placeholders::Root                          _root;


  const std::shared_ptr<GMlib::Scene>&        scene() const;

  QModelIndex                                 findQmiFromSo( const GMlib::Array<GMlib::SceneObject*>& children,
                                                             GMlib::SceneObject* obj_to_find ) const;
};


#endif // SCENEPROXYMODEL_H
