#-------------------------------------------------
#
# Project created by QtCreator 2011-12-21T13:34:30
#
#-------------------------------------------------

include(global.pri)

QT       += network sql xml

TARGET = ilwiscore
TEMPLATE = lib

DEFINES += CORE_LIBRARY

SOURCES += core/kernel.cpp \
    core/version.cpp \
    core/module.cpp \
    core/errorobject.cpp \
    core/abstractfactory.cpp \
    core/ilwisobjects/ilwisobject.cpp \
    core/ilwisobjects/geometry/coordinatesystem/coordinatesystem.cpp \
    core/ilwisobjects/geometry/georeference/georeference.cpp \
    core/ilwisobjects/geometry/georeference/simpelgeoreference.cpp \
    core/ilwisobjects/geometry/georeference/cornersgeoreference.cpp \
    core/ilwisobjects/coverage/coverage.cpp \
    core/publicdatabase.cpp \
    core/ilwisobjects/ilwisobjectfactory.cpp \
    core/issuelogger.cpp \
    core/identity.cpp \
    core/ilwisobjects/domain/domain.cpp \
    core/ilwisobjects/domain/thematicitem.cpp \
    core/util/range.cpp \
    core/catalog/catalog.cpp \
    core/catalog/catalogconnector.cpp \
    core/connectorfactory.cpp \
    core/ilwiscontext.cpp \
    core/catalog/mastercatalog.cpp \
    core/catalog/catalogquery.cpp \
    core/catalog/resource.cpp \
    core/ilwisobjects/ilwisobjectconnector.cpp \
    core/ilwisobjects/geometry/coordinatesystem/conventionalcoordinatesystem.cpp \
    core/util/juliantime.cpp \
    core/ilwisobjects/domain/numericdomain.cpp \
    core/ilwisobjects/domain/identifierrange.cpp \
    core/ilwisobjects/domain/identifieritem.cpp \
    core/ilwisobjects/domain/itemrange.cpp \
    core/util/angle.cpp \
    core/ilwisobjects/coverage/featurecoverage.cpp \
    core/ilwisobjects/coverage/feature.cpp \
    core/ilwisobjects/coverage/grid.cpp \
    core/ilwisobjects/coverage/pixeliterator.cpp \
    core/ilwisobjects/table/flattable.cpp \
    core/ilwisobjects/table/databasetable.cpp \
    core/ilwisobjects/table/columndefinition.cpp \
    core/ilwisobjects/coverage/featureiterator.cpp \
    core/ilwisobjects/table/basetable.cpp \
    core/ilwisobjects/table/attributerecord.cpp \
    core/ilwisobjects/coverage/featurefactory.cpp \
    core/ilwisobjects/geometry/coordinatesystem/projectionimplementation.cpp \
    core/ilwisobjects/geometry/coordinatesystem/projection.cpp \
    core/ilwisobjects/geometry/coordinatesystem/proj4parameters.cpp \
    core/ilwisobjects/geometry/coordinatesystem/ellipsoid.cpp \
    core/ilwisobjects/geometry/coordinatesystem/geodeticdatum.cpp \
    core/ilwisobjects/operation/operation.cpp \
    core/ilwisobjects/operation/operationmetadata.cpp \
    core/ilwisobjects/operation/operationExpression.cpp \
    core/ilwisobjects/operation/commandhandler.cpp \
    core/ilwisobjects/coverage/blockiterator.cpp \
    core/util/locker.cpp \
    core/ilwisobjects/domain/datadefinition.cpp \
    core/ilwisobjects/geometry/georeference/ctpgeoreference.cpp \
    core/ilwisobjects/geometry/georeference/controlpoint.cpp \
    core/ilwisobjects/geometry/georeference/planarctpgeoreference.cpp \
    core/util/mathhelper.cpp \
    core/ilwisobjects/domain/textdomain.cpp \
    core/ilwisobjects/geometry/georeference/georefadapter.cpp \
    core/ilwisobjects/operation/symboltable.cpp \
    core/ilwisobjects/operation/operationhelpergrid.cpp \
    core/ilwisobjects/operation/operationhelper.cpp \
    core/ilwisobjects/operation/operationhelperfeatures.cpp \
    core/ilwisobjects/geometry/georeference/georefimplementation.cpp \
    core/ilwisobjects/geometry/georeference/georefimplementationfactory.cpp \
    core/ilwisobjects/domain/coordinatedomain.cpp \
    core/ilwisobjects/coverage/indexslicer.cpp \
    core/ilwisobjects/coverage/rastercoverage.cpp \
    core/ilwisobjects/coverage/rasterinterpolator.cpp \
    core/ilwisobjects/operation/logicalexpressionparser.cpp \
    core/ilwisobjects/table/tableselector.cpp \
    core/ilwisobjects/table/tablemerger.cpp \
    core/ilwisobjects/domain/domainmerger.cpp \
    core/util/tranquilizer.cpp \
    core/ilwisobjects/operation/numericoperation.cpp \
    core/util/coordinate.cpp \
    core/ilwisobjects/geometry/coordinatesystem/csytransform.cpp \
    core/ilwisobjects/coverage/geometryhelper.cpp \
    core/ilwisobjects/operation/rasterfilter.cpp \
    core/catalog/dataformat.cpp \
    core/ilwisobjects/domain/domainhelper.cpp \
    core/ilwisobjects/domain/numericrange.cpp \
    core/ilwisobjects/domain/colordomain.cpp \
    core/ilwisobjects/domain/colorrange.cpp \
    core/catalog/catalogexplorer.cpp \
    core/catalog/foldercatalogexplorer.cpp \
    core/catalog/catalogview.cpp \
    core/ilwisobjects/coverage/vertexiterator.cpp \
    core/ilwisobjects/domain/coloritem.cpp \
    core/oshelper.cpp \
    core/util/bresenham.cpp \
    core/ilwisobjects/operation/operationoverloads.cpp \
    core/ilwisobjects/operation/classification/featurespace.cpp \
    core/ilwisobjects/operation/classification/sampleset.cpp \
    core/ilwisobjects/operation/classification/samplestatistics.cpp \
    core/ilwisobjects/domain/interval.cpp \
    core/ilwisobjects/domain/intervalrange.cpp
    core/util/bresenham.cpp

HEADERS += core/kernel.h\
      core/kernel_global.h \
    core/version.h \
    core/module.h \
    core/ilwis.h \
    core/errorobject.h \
    core/factory.h \
    core/abstractfactory.h \
    core/ilwisobjects/ilwisdata.h \
    core/ilwisobjects/ilwisobject.h \
    core/ilwisobjects/geometry/coordinatesystem/coordinatesystem.h \
    core/ilwisobjects/geometry/georeference/georeference.h \
    core/ilwisobjects/geometry/georeference/simpelgeoreference.h \
    core/ilwisobjects/geometry/georeference/cornersgeoreference.h \
    core/ilwisobjects/coverage/coverage.h \
    core/publicdatabase.h \
    core/ilwisobjects/ilwisobjectfactory.h \
    core/issuelogger.h \
    core/identity.h \
    core/ilwisobjects/table/table.h \
    core/ilwisobjects/domain/domain.h \
    core/ilwisobjects/domain/thematicitem.h \
    core/util/range.h \
    core/catalog/catalog.h \
    core/catalog/catalogconnector.h \
    core/connectorfactory.h \
    core/connectorinterface.h \
    core/ilwiscontext.h \
    core/catalog/mastercatalog.h \
    core/catalog/catalogquery.h \
    core/catalog/resource.h \
    core/ilwisobjects/ilwisobjectconnector.h \
    core/ilwisobjects/geometry/coordinatesystem/conventionalcoordinatesystem.h \
    core/util/juliantime.h \
    core/ilwisobjects/domain/numericdomain.h \
    core/ilwisobjects/domain/itemdomain.h \
    core/ilwisobjects/domain/identifierrange.h \
    core/ilwisobjects/domain/identifieritem.h \
    core/ilwisobjects/domain/domainitem.h \
    core/util/memorymanager.h \
    core/ilwisobjects/domain/itemrange.h \
    core/ilwisobjects/coverage/blockiterator.h \
    core/util/angle.h \
    core/util/box.h \
    core/ilwisobjects/coverage/pixeliterator.h \
    core/ilwisobjects/coverage/feature.h \
    core/ilwisobjects/coverage/featurecoverage.h \
    core/util/containerstatistics.h \
    core/ilwisobjects/coverage/grid.h \
    core/util/size.h \
    core/ilwisobjects/table/flattable.h \
    core/ilwisobjects/table/databasetable.h \
    core/ilwisobjects/table/columndefinition.h \
    core/ilwisobjects/coverage/featureiterator.h \
    core/ilwisobjects/table/basetable.h \
    core/ilwisobjects/table/attributerecord.h \
    core/ilwisobjects/coverage/featurefactory.h \
    core/util/errmessages.h \
    core/ilwisobjects/geometry/coordinatesystem/projectionimplementation.h \
    core/ilwisobjects/geometry/coordinatesystem/projectionfactory.h \
    core/ilwisobjects/geometry/coordinatesystem/projection.h \
    core/ilwisobjects/geometry/coordinatesystem/proj4parameters.h \
    core/ilwisobjects/geometry/coordinatesystem/ellipsoid.h \
    core/ilwisobjects/geometry/coordinatesystem/geodeticdatum.h \
    core/ilwisobjects/operation/operation.h \
    core/ilwisobjects/operation/operationmetadata.h \
    core/ilwisobjects/operation/operationExpression.h \
    core/ilwisobjects/operation/operationconnector.h \
    core/ilwisobjects/operation/commandhandler.h \
    core/util/locker.h \
    core/ilwisobjects/coverage/raster.h \
    core/ilwisobjects/operation/ilwisoperation.h \
    core/ilwisobjects/domain/datadefinition.h \
    core/util/geometries.h \
    core/ilwisobjects/geometry/georeference/ctpgeoreference.h \
    core/ilwisobjects/geometry/georeference/controlpoint.h \
    core/ilwisobjects/geometry/georeference/planarctpgeoreference.h \
    core/util/mathhelper.h \
    core/ilwisobjects/domain/textdomain.h \
    core/ilwisobjects/geometry/georeference/georefadapter.h \
    core/ilwisobjects/operation/symboltable.h \
    core/ilwisobjects/operation/operationhelpergrid.h \
    core/ilwisobjects/operation/operationhelper.h \
    core/ilwisobjects/operation/operationhelperfeatures.h \
    core/ilwisobjects/geometry/georeference/georefimplementation.h \
    core/ilwisobjects/geometry/georeference/georefimplementationfactory.h \
    core/ilwisobjects/domain/coordinatedomain.h \
    core/ilwisobjects/coverage/indexslicer.h \
    core/ilwisobjects/coverage/rastercoverage.h \
    core/ilwisobjects/coverage/rasterinterpolator.h \
    core/ilwisobjects/domain/itemiterator.h \
    core/ilwisobjects/operation/logicalexpressionparser.h \
    core/ilwisobjects/table/tableselector.h \
    core/ilwisobjects/table/tablemerger.h \
    core/ilwisobjects/domain/domainmerger.h \
    core/util/tranquilizer.h \
    core/ilwisobjects/operation/numericoperation.h \
    core/util/location.h \
    core/util/coordinate.h \
    core/ilwisobjects/geometry/coordinatesystem/csytransform.h \
    core/ilwisobjects/coverage/geometryhelper.h \
    core/ilwisobjects/operation/rasterfilter.h \
    core/catalog/dataformat.h \
    core/ilwisobjects/domain/domainhelper.h \
    core/ilwisobjects/domain/numericrange.h \
    core/ilwisobjects/domain/colordomain.h \
    core/ilwisobjects/domain/colorrange.h \
    core/catalog/catalogexplorer.h \
    core/catalog/foldercatalogexplorer.h \
    core/prepareoptions.h \
    core/catalog/catalogview.h \
    core/ilwisobjects/coverage/vertexiterator.h \
    core/ilwisobjects/domain/coloritem.h \
    core/ilwistypes.h \
    core/oshelper.h \
    core/util/bresenham.h \
    core/ilwisobjects/operation/operationoverloads.h \
    core/ilwisobjects/operation/classification/featurespace.h \
    core/ilwisobjects/operation/classification/sampleset.h \
    core/ilwisobjects/operation/classification/samplestatistics.h \
    core/ilwisobjects/domain/interval.h \
    core/ilwisobjects/domain/intervalrange.h \
    core/ilwisobjects/geometry/coordinatesystem/projectionimplementation.h \
    core/kernel_global.h
    core/util/bresenham.h


OTHER_FILES += \
    core/resources/referencesystems.csv \
    core/resources/projections.csv \
    core/resources/numericdomains.csv \
    core/resources/filters.csv \
    core/resources/epsg.pcs \
    core/resources/ellipsoids.csv \
    core/resources/datums.csv \
    LICENSE-2.0.txt \
    installer.nsi


QMAKE_POST_LINK += $${QMAKE_COPY} $$PWD/../libraries/$$PLATFORM$$CONF/core/lib$${TARGET}.so $$PWD/../output/$$PLATFORM$$CONF/bin


resources.files = core/resources/referencesystems.csv \
    core/resources/projections.csv \
    core/resources/numericdomains.csv \
    core/resources/filters.csv \
    core/resources/epsg.pcs \
    core/resources/ellipsoids.csv \
    core/resources/datums.csv
resources.path = $$DLLDESTDIR/resources

license.files =  LICENSE-2.0.txt
license.path = $$DLLDESTDIR

installer.files =installer.nsi
installer.path = $$PWD/../output/$$PLATFORM$$CONF

win32{
    CONFIG(debug, debug|release) {
        qtdlls.files = "$$[QT_INSTALL_PREFIX]/bin/Qt5Cored.dll" \
                       "$$[QT_INSTALL_PREFIX]/bin/Qt5Sqld.dll" \
                       "$$[QT_INSTALL_PREFIX]/bin/Qt5Guid.dll"
    }else{
        qtdlls.files = "$$[QT_INSTALL_PREFIX]/bin/Qt5Core.dll" \
                       "$$[QT_INSTALL_PREFIX]/bin/Qt5Sql.dll" \
                       "$$[QT_INSTALL_PREFIX]/bin/Qt5Gui.dll"
    }
    qtdlls.files +=   "$$[QT_INSTALL_PREFIX]/bin/libgcc_s_dw2-1.dll" \
                      "$$[QT_INSTALL_PREFIX]/bin/libstdc~1.dll" \
                      "$$[QT_INSTALL_PREFIX]/bin/libwinpthread-1.dll" \
                      "$$[QT_INSTALL_PREFIX]/bin/icudt51.dll" \
                      "$$[QT_INSTALL_PREFIX]/bin/icuin51.dll" \
                      "$$[QT_INSTALL_PREFIX]/bin/icuuc51.dll"
    qtdlls.path = $$PWD/../output/$$PLATFORM$$CONF/bin

    qtcreatepluginsdir.commands += @echo "exists($$DLLDESTDIR/qtplugins)" $$escape_expand(\\n\\t)
    !exists($$DLLDESTDIR/qtplugins) {
        qtcreatepluginsdir.commands += md $$DLLDESTDIR/qtplugins $$escape_expand(\\n\\t)
        !exists($$DLLDESTDIR/qtplugins/sqldrivers) {
            qtcreatepluginsdir.commands += md $$DLLDESTDIR/qtplugins/sqldrivers $$escape_expand(\\n\\t)
        }
    }

    CONFIG(debug, debug|release) {
        qtsqlplugin.files = $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/qsqlited.dll
    }else{
        qtsqlplugin.files = $$[QT_INSTALL_PREFIX]/plugins/sqldrivers/qsqlite.dll
    }
    qtsqlplugin.path = $$PWD/../output/$$PLATFORM$$CONF/bin/qtplugins/sqldrivers+

    INSTALLS += resources license installer qtdlls qtsqlplugin
}



INSTALLS += resources
QMAKE_EXTRA_TARGETS = qtcreatepluginsdir


