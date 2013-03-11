#include "PluginBase.h"
//DEBUG
#include <QDebug>


/*!
 * \class Plugins::PluginEnums
 * \code
 * #include <PluginEnum.h>
 * \endcode
 *
 * \brief Contains the various enums needed by plugins
 *
 * \inmodule Tepee3D
 */

/*!
 * \fn PluginsEnums::PluginState Plugins::PluginEnums::valueOf(int value)
 *
 * Returns the PluginState enum corresponding to \a value.
 */


/*!
 * \class Plugins::PluginBase
 * \code
 * #include <PluginBase.h>
 * \endcode
 * \brief The Plugins::PluginBase class is the base class a Tepee3D plugin
 * has to inherit from. It wraps access to the various services a plugin has access
 * to as well as providing informations about the plugin that will be needed to by the
 * application to properly manage plugins.
 *
 * \inmodule Tepee3D
 */

/*!
 * \fn int Plugins::PluginBase::getPluginId()
 *
 * Returns the id of the plugins.
 */

/*!
 * \fn void Plugins::PluginBase::initPlugin()
 *
 * Initializes the plugins.
 */

/*!
 * \fn QString Plugins::PluginBase::getPluginName()
 *
 * Returns the name of the plugins.
 */

/*!
 * \fn QString Plugins::PluginBase::getPluginDescription()
 *
 * Returns a description of the plugin.
 */


/*!
 * \fn QString Plugins::PluginBase::getRoomPluginQmlFile() const
 *
 * Returns the main qml file's name so that the engine knows which file to load.
 */

/*!
 * \fn QString Plugins::PluginBase::getMenuPluginQmlFile() const
 *
 * Returns the name of the qml file used to set the plugin and that will be loaded in the plugin menu area.
 */

/*!
 * \fn Plugins::PluginBase*  Plugins::PluginBase::createNewInstance()
 *
 * Returns a new instance of the plugin.
 */

/*!
 * \fn void Plugins::PluginBase::exposeContentToQml(QQmlContext *context)
 *
 * Exposes QML content to the QML \a context if the plugins needs it.
 */

/*!
 * \fn void Plugins::PluginBase::receiveResultFromSQLQuery(QList<QSqlRecord> result, int id)
 *
 * Called by the Database Service Library to transmit the \a result of a previously executed query as well
 * as and \a id to help recognize the query.
 */

/*!
 * \fn void Plugins::PluginBase::receiveResultFromHttpRequest(QNetworkReply *reply)
 *
 * Triggered when the \a reply of a previously executed network request is received.
 */

/*!
 * \fn void Plugins::PluginBase::executeSQLQuery(const QString& query, QObject *sender, int id)
 *
 * Emitted when a database \a query needs to be executed. The result of its execution will be transmitted to the \a sender
 * along with an \a id to recognize the query.
 */

/*!
 * \fn void Plugins::PluginBase::executeHttpRequest(const QNetworkRequest &request, int requestType, QHttpMultiPart *multipart, QObject *sender)
 *
 * Emitted when a network \a request needs to be performed, with the \a requestType to identify the type HTTP request and a \a multipart which can be null to transmit
 * data through the request. The reply will be transmitted to \a sender.
 */

/*!
 * \fn void Plugins::PluginBase::askForFocusState(Plugins::PluginEnums::PluginState requestedState, QObject *sender)
 *
 * Emitted when a plugin ask for a new focus state \a requestedState. The response will be transmitted to \a sender.
 */

/*!
 * \fn void Plugins::PluginBase::focusStateChanged(QVariant focusState)
 *
 * Emitted when the \a focusState of a plugin has changed.
 */

/*!
 * \fn void Plugins::PluginBase::roomEntered()
 *
 * Emitted when the room in which the plugin is loaded is entered.
 */

/*!
 * \fn void Plugins::PluginBase::roomLeft()
 *
 * Emitted when the room in which the plugin is loaded is left.
 */


/*!
 * Constructs a new Plugins::PluginBase instance.
 */
Plugins::PluginBase::PluginBase() : QObject(NULL)
{
    qDebug() << "NEW PLUGINBASE INSTANCE CREATED ";
    this->setFocusState(Plugins::PluginEnums::pluginIdleState);
    QObject::connect(this, SIGNAL(roomLeft()), this, SLOT(onRoomEntered()));
}

/*!
 * Returns the current instance of the plugin.
 */
Plugins::PluginBase* Plugins::PluginBase::getPluginBase()
{
    return this;
}

/*!
 * Slot triggered when the Tepee3DEngine enters the room containing the plugin.
 */
void    Plugins::PluginBase::onRoomEntered()
{
    this->setFocusState(Plugins::PluginEnums::pluginIdleState);
}

/*!
 * Sets the plugin's focus state and alert the qml view that the focusState \a requestedState has changed.
 */
void    Plugins::PluginBase::setFocusState(Plugins::PluginEnums::PluginState requestedState)
{
    qDebug() << "Setting Focus State " << requestedState;
    this->focusState = requestedState;
    emit (focusStateChanged(QVariant(requestedState)));
}

/*!
 * When a plugin wishes to change its focusState, call this method with the required \a requestedState.
 */
void    Plugins::PluginBase::askForFocusState(Plugins::PluginEnums::PluginState requestedState)
{
    // ASK THE ROOM FOR A NEW FOCUS STATE
    emit askForFocusState(requestedState, this);
}

/*!
 * Called when receiving SQL results from the Database Service.
 */
void    Plugins::PluginBase::resultFromSQL()
{
    qDebug() << "PLUGIN BASE RESULT FROM SQL";
}

/*!
 * Sends Http Get \a request to network manager.
 */
void    Plugins::PluginBase::executeHttpGetRequest(const QNetworkRequest &request)
{
    emit executeHttpRequest(request, Get, NULL, this);
}

/*!
 * Sends Http Delete \a request to network manager.
 */
void    Plugins::PluginBase::executeHttpDeleteRequest(const QNetworkRequest &request)
{
    emit executeHttpRequest(request, Delete, NULL, this);
}

/*!
 * Sends Http Post \a request to network manager with \a multiPart for data.
 */
void    Plugins::PluginBase::executeHttpPostRequest(const QNetworkRequest &request, QHttpMultiPart* multiPart)
{
    emit executeHttpRequest(request, Post, multiPart, this);
}

/*!
 * Sends Http Put \a request to network manager with \a multiPart for data.
 */
void    Plugins::PluginBase::executeHttpPutRequest(const QNetworkRequest &request, QHttpMultiPart* multiPart)
{
    emit executeHttpRequest(request, Put, multiPart, this);
}

/*!
 * Returns a boolean telling whether this pluugin needs logical updating every seconds or not.
 */
bool    Plugins::PluginBase::needsUpdating() const
{
    return false;
}

/*!
 * Returns the current focusState of the plugin.
 */
Plugins::PluginEnums::PluginState    Plugins::PluginBase::getFocusState()   const
{
    return this->focusState;
}

