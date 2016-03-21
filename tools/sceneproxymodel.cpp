#include "sceneproxymodel.h"

#include "../application/gmlibwrapper.h"
#include "gmlibqmltypes.h"

// gmlib
#include <gmSceneModule>
#include <gmParametricsModule>

// qt
#include <QDebug>
#include <QColor>






SceneProxyModel::SceneProxyModel(std::shared_ptr<GMlibWrapper> gmlib)
  : _gmlib{gmlib}
{
  Q_ASSERT(gmlib);
}

QModelIndex SceneProxyModel::indexFromSceneObject(GMlib::SceneObject* so_to_find ) {

  for( int row = 0; row < scene()->getSize(); ++row ) {

    auto so = scene()->operator[](row);
    if( so == so_to_find ) return createIndex(row,0,so_to_find);

    return findQmiFromSo( so->getChildren(), so_to_find );
  }

  return QModelIndex();


}

QVariant
SceneProxyModel::getSceneProperty(const QString& name) const {

  if(name == "running_status" ) return scene()->isRunning();

  return QVariant();
}

bool
SceneProxyModel::setSceneProperty(const QString& name, const QString& value) {
  Q_UNUSED(name)
  Q_UNUSED(value)


  return false;
}

QVariantList
SceneProxyModel::getPropertyModules(const QModelIndex& index) const {

  if(!index.isValid()) return QVariantList();

  GMlib::SceneObject* so = static_cast<GMlib::SceneObject*>(index.internalPointer());

  QVariantList modules;
  modules << int(PropertyModules::SceneObject);

  if( dynamic_cast<GMlib::Camera*>(so) )                      modules << int(PropertyModules::Camera);

  else if(dynamic_cast<GMlib::Light*>(so)) {                  modules << int(PropertyModules::Light);
    if(dynamic_cast<GMlib::SpotLight*>(so) )                  modules << int(PropertyModules::SpotLight);
  }

  else if( dynamic_cast<GMlib::PSurf<float,3>*>(so) ) {       modules << int(PropertyModules::PSurf);
    if( dynamic_cast<GMlib::PTorus<float>*>(so) )             modules << int(PropertyModules::PTorus);
    else if( dynamic_cast<GMlib::PERBSSurf<float>*>(so) )     modules << int(PropertyModules::PERBSSurf);
  }


  modules << getCustomPropertyModules(so);

  return modules;
}

QVariant
SceneProxyModel::getProperty(const QModelIndex& index, int module, const QString& name) const {

  if(!index.isValid()) return QVariant();

  const auto obj = static_cast<GMlib::SceneObject*>(index.internalPointer());

  switch(PropertyModules(module)) {
    case PropertyModules::SceneObject: return getSceneObjectProperty(obj,name);
    case PropertyModules::Light: {
      auto light = dynamic_cast<GMlib::Light*>(obj);
      if(light) return getLightProperty(light,name);
      else return QVariant();
    }
  }

  return getCustomProperty(obj,module,name);
}

QVariant
SceneProxyModel::getSceneObjectProperty(const GMlib::SceneObject* sceneobject, const QString& name) const {

  if(     name == "identity")     return sceneobject->getIdentity().c_str();
  else if(name == "name")         return sceneobject->getName();
  else if(name == "virtual_name") return sceneobject->getVirtualName();
  else if(name == "lighted")      return sceneobject->isLighted();
  else if(name == "color")        return QVariant::fromValue(Color_GM(sceneobject->getColor()));
  else if(name == "material" )    return QVariant::fromValue(Material_GM(sceneobject->getMaterial()));

  return QVariant();
}

QVariant
SceneProxyModel::getLightProperty(const GMlib::Light* light, const QString& name) const {

  if(name == "enabled") return light->isEnabled();
  else if(name == "ambient")  return QVariant::fromValue(Color_GM(light->getAmbient()));
  else if(name == "diffuse")  return QVariant::fromValue(Color_GM(light->getDiffuse()));
  else if(name == "specular") return QVariant::fromValue(Color_GM(light->getSpecular()));

  return QVariant();
}

bool
SceneProxyModel::setProperty(const QModelIndex& index, int module, const QString& name, const QVariant& value) {

  if(!index.isValid()) return false;
  auto obj = static_cast<GMlib::SceneObject*>(index.internalPointer());

  switch(PropertyModules(module)) {
    case PropertyModules::SceneObject: return setSceneObjectProperty(obj,name,value);
    case PropertyModules::Light:  {
      auto light = dynamic_cast<GMlib::Light*>(obj);
      if(light) return setLightProperty(light,name,value);
      else return false;
    }
  }

  return setCustomProperty(obj,module,name,value);
}

bool
SceneProxyModel::setSceneObjectProperty(GMlib::SceneObject* sceneobject, const QString& name, const QVariant& value ) {

  if(name == "color") {

    sceneobject->setColor( toGMlibColor(value.value<QColor>()) );
    return true;
  }
  else if( name == "material" ) {

    auto material = value.value<Material_GM>();
    sceneobject->setMaterial(material);
    return true;
  }

  return false;
}

bool
SceneProxyModel::setLightProperty(GMlib::Light* light, const QString& name, const QVariant& value) const {

  if(     name == "enabled")  light->GMlib::Light::setEnabled( value.toBool() );
  else if(name == "ambient")  light->GMlib::Light::setColor( toGMlibColor(value.value<QColor>()), light->getDiffuse(), light->getSpecular() );
  else if(name == "diffuse")  light->GMlib::Light::setColor( light->getAmbient(), toGMlibColor(value.value<QColor>()), light->getSpecular() );
  else if(name == "specular") light->GMlib::Light::setColor( light->getAmbient(), light->getDiffuse(), toGMlibColor(value.value<QColor>()) );
  else return false;

  return true;
}

void
SceneProxyModel::selectSceneObject(const QModelIndex& index) const {

  if(!index.isValid()) return;

  scene()->clearSelection();
  auto obj = static_cast<GMlib::SceneObject*>(index.internalPointer());
  obj->setSelected(true);
}

QModelIndex
SceneProxyModel::index(int row, int column, const QModelIndex& parent) const {

  // No index evaluates to invalid index
  if( !hasIndex(row, column, parent) )
    return QModelIndex();

  // Create index for all child elements of root
  if( !parent.isValid() and row < scene()->getSize() )
      return createIndex( row, column, scene()->operator[](row) );

  // Create index for all child elements of a child
  auto parent_so = static_cast<GMlib::SceneObject*>(parent.internalPointer());
  if(row < parent_so->getChildren().size())
    return createIndex( row, column, parent_so->getChildren()(row) );

  return QModelIndex();
}

QModelIndex
SceneProxyModel::parent(const QModelIndex& index) const {

  // If invalid, return empty QModelIndex (which evaluates to top-level)
  if(!index.isValid())  return QModelIndex();

  auto so = static_cast<GMlib::SceneObject*>(index.internalPointer());
  auto parent_so = so->getParent();

  // Invalid parent index if root of scene
  if(!parent_so) return QModelIndex();

  // Find parent row
  auto pparent_so = parent_so->getParent();
  int row;
  if( pparent_so ) {
    for( row = 0; so != pparent_so->getChildren()(row); ++row );
    if(row >= pparent_so->getChildren().size()) return QModelIndex(); // Catch if scene structure changes outside qtquick model
  }
  else {
    for( row = 0; so != scene()->operator[](row); ++row );
    if(row >= scene()->getSize()) return QModelIndex(); // Catch if scene structure changes outside qtquick model
  }

  return createIndex(row,0,parent_so);
}

int
SceneProxyModel::rowCount(const QModelIndex& parent) const {

  // Only first column can have children (one-dimensional tree)
  if( parent.column() > 0 ) return 0;

  // If no parent, interpret as root scene item
  if( !parent.isValid() )  return scene()->getSize();

  auto parent_so = static_cast<GMlib::SceneObject*>(parent.internalPointer());
  return parent_so->getChildren().size();
}

int
SceneProxyModel::columnCount(const QModelIndex& parent) const {

  return 1;
}

QVariant
SceneProxyModel::data(const QModelIndex& index, int role) const {

  if( !index.isValid() )         return QVariant("INVALID");

  auto sceneobject = static_cast<GMlib::SceneObject*>(index.internalPointer());

  switch(UserRoles(role)) {
    case UserRoles::Identity:       return sceneobject->getIdentity().c_str();
    case UserRoles::Name:           return sceneobject->getName();
    case UserRoles::Visible:        return sceneobject->isVisible();
    case UserRoles::MemoryAddress:  return qulonglong(sceneobject);
    default:                        return QVariant("DEFAULT");
  }

}

Qt::ItemFlags
SceneProxyModel::flags(const QModelIndex& index) const {

//  if(!index.isValid()) return Qt::ItemIsEditable;
  if(!index.isValid()) return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

QHash<int, QByteArray>
SceneProxyModel::roleNames() const {

  QHash<int, QByteArray> role_names;
  role_names[int(UserRoles::Identity)]      = "so_identity";
  role_names[int(UserRoles::Name)]          = "so_name";
  role_names[int(UserRoles::Visible)]       = "so_visible";
  role_names[int(UserRoles::MemoryAddress)] = "so_memory_address";

  return role_names;
}

bool
SceneProxyModel::setData(const QModelIndex& index, const QVariant& value, int role) {

  if(!index.isValid() and role == Qt::EditRole) return false;

  auto sceneobject = static_cast<GMlib::SceneObject*>(index.internalPointer());
  sceneobject->setVisible(value.toBool(), true);

  emit dataChanged(index,index);
}

void
SceneProxyModel::stupidForceModelUpdate() {

  beginResetModel(); {
    // This is what happens
  }
  endResetModel();
}

const std::shared_ptr<GMlib::Scene>&
SceneProxyModel::scene() const {

  return _gmlib->scene();
}

QModelIndex
SceneProxyModel::findQmiFromSo(const GMlib::Array<GMlib::SceneObject*>& children, GMlib::SceneObject* obj_to_find) const {

  for( int row = 0; row < children.size(); ++row ) {

    auto so = children(row);
    if(so == obj_to_find) return createIndex(row,0,obj_to_find);

    return findQmiFromSo(so->getChildren(),obj_to_find);
  }

  return QModelIndex();
}

QVariantList
SceneProxyModel::getCustomPropertyModules(const GMlib::SceneObject* sceneobject) const {
  Q_UNUSED(sceneobject)

  return QVariantList();
}

QVariant
SceneProxyModel::getCustomProperty(const GMlib::SceneObject* sceneobject, int module, const QString& name) const {
  Q_UNUSED(sceneobject)
  Q_UNUSED(module)
  Q_UNUSED(name)

  return QVariant();
}

bool
SceneProxyModel::setCustomProperty(const GMlib::SceneObject* sceneobject, int module, const QString& name, const QVariant& value) {
  Q_UNUSED(sceneobject)
  Q_UNUSED(module)
  Q_UNUSED(name)
  Q_UNUSED(value)

  return false;
}

GMlib::Color
SceneProxyModel::toGMlibColor(const QColor& c) {

  GMlib::Color color(c.redF(),c.greenF(),c.blueF(),c.alphaF());
  return color;
}

QColor
SceneProxyModel::toQColor(const GMlib::Color& c) {

  QColor color;
  color.setRgbF(c.getRedC(),c.getGreenC(),c.getBlueC(),c.getAlphaC());
  return color;
}

QVariant
SceneProxyModel::createMaterial_GM() {

  return QVariant::fromValue(Material_GM());
}
