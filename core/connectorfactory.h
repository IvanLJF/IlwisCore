#ifndef CONNECTORFACTORY_H
#define CONNECTORFACTORY_H

#include "Kernel_global.h"
#include <QScopedPointer>
#include <QHash>

namespace Ilwis {

class ConnectorInterface;

//typedef ConnectorInterface* (*ConnectorCreate)(const IlwisResource &resource);
//-----------------------------------------------------------------------------------------
struct ConnectorFilter{
    ConnectorFilter(quint64 types, const QString& provider) : _objectTypes(types), _provider(provider) {}
    quint64 _objectTypes;
    QString _provider;
};

struct ConnectorFormatSelector{
    ConnectorFormatSelector(const QString& format, const QString& provider=sUNDEF) : _format(format), _provider(provider) {}

    QString _format;
    QString _provider;

};

KERNELSHARED_EXPORT uint qHash(const ConnectorFilter& resource );
KERNELSHARED_EXPORT bool operator==(const ConnectorFilter& filter1, const ConnectorFilter& filter2 );
KERNELSHARED_EXPORT bool operator<(const ConnectorFilter& filter1, const ConnectorFilter& filter2 );

KERNELSHARED_EXPORT uint qHash(const ConnectorFormatSelector& resource );
KERNELSHARED_EXPORT bool operator==(const ConnectorFormatSelector& filter1, const ConnectorFormatSelector& filter2 );
KERNELSHARED_EXPORT bool operator<(const ConnectorFormatSelector& filter1, const ConnectorFormatSelector& filter2 );


/*!
  This class creates a connector for a certain resource
  Connectors (implementations of the connector interface) exist in many forms in Ilwis. They are usually tied to specific type of format. The factory
  produces a specific implementation of the interface. It does this by applying a filter on the set of known connectors to find a suitable create function
  for a specific type of resource. Connectors can be created based on a format or purely on the resource.
 */
class KERNELSHARED_EXPORT ConnectorFactory : public AbstractFactory
{
    friend class ConnectorInterface;

public:
    /*!
     * \brief ConnectorFactory constructor for the factory
     */
    ConnectorFactory();

    /*!
     * \brief Each connector must implement a create method (static method) that can be passed to this factory to be available to the system.
     *
     * \param objecttypes a create function creates an connector that creates at least one type of objects but it may create more types. This parameter is OR combination of all the types it might create
     * \param provider the search key that is used to locate a particular create method
     */
    void addCreator(quint64 objecttypes, const QString &provider, ConnectorCreate);
    /*!
     * \brief Each connector must implement a create method (static method) that can be passed to this factory to be available to the system.
     * \param format
     * \param provider
     */
    void addCreator(const QString& format, const QString &provider, ConnectorCreate);

    /*!
     * Creates a connector based on the properties of the resource. The method will try to find a create function based on the type of the resource and the provider
     This method is usually called from objectfactories in a plug-in that implements a defined set of connectors. At that place the name of the provider is known and can be used.
     outside the plug-in the name of the provider is often not known and can not be used.
     * \param item a resource of a certain type. To work the type must have a valid IlwisType
     * \param provider an identifier identifying the connector type
     * \return an instantiation of a connector or 0. In the case of 0 the error can be found in the issuelogger
     */

    template<class T=ConnectorInterface> T *createFromResource(const Resource& item, const QString &provider) const{
        ConnectorFilter filter(item.ilwisType(), provider);
        auto iter = _creatorsPerObject.find(filter);
        if ( iter == _creatorsPerObject.end())
            return 0;
        ConnectorCreate createConnector = iter.value();
        if ( createConnector ) {
            ConnectorInterface *cif =  createConnector(item, true);
            return dynamic_cast<T *>(cif);
        }
        kernel()->issues()->log(TR(ERR_COULDNT_CREATE_OBJECT_FOR_2).arg("Connector",item.name()));
        return 0;

    }

    /*!
     * Creates a connector based on the properties of the resource. The method will try to find a create function based on the type of the resource and the format
     This method is usually called when a object must be connected to a specific format. For example when a script forces the use of a certain output format.
     * \param item a resource of a certain type. To work the type must have a valid IlwisType
     * \return an instantiation of a connector or 0. In the case of 0 the error can be found in the issuelogger
     */
    template<class T=ConnectorInterface> T *createFromFormat(const Resource& item, const QString &format, const QString& provider=sUNDEF) const{
        ConnectorFormatSelector filter(format, provider);
        auto iter = _creatorsPerFormat.find(filter);
        if ( iter == _creatorsPerFormat.end())
            return 0;
        ConnectorCreate createConnector = iter.value();
        if ( createConnector ) {
            ConnectorInterface *cif =  createConnector(item, false);
            cif->format(format);
            return dynamic_cast<T *>(cif);
        }


        kernel()->issues()->log(TR(ERR_COULDNT_CREATE_OBJECT_FOR_2).arg("Connector",item.name()));
        return 0;
    }

protected:
    QHash<ConnectorFilter, ConnectorCreate > _creatorsPerObject;
    QHash<ConnectorFormatSelector, ConnectorCreate > _creatorsPerFormat;
};
}


#endif // CONNECTORFACTORY_H
