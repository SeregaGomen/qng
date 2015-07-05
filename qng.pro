#-------------------------------------------------
#
# Project created by QtCreator 2015-05-31T09:00:23
#
#-------------------------------------------------

unix:QMAKE_CXXFLAGS += -std=c++0x

DEFINES += _AFXDLL\
           _CRT_SECURE_NO_WARNINGS

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qng
TEMPLATE = app

INCLUDEPATH += libsrc/include \
               nglib

unix:LIBS += -lz

SOURCES += main.cpp\
        mainwindow.cpp\
        glwidget.cpp \
        libsrc/csg/python_csg.cpp \
        libsrc/csg/zrefine.cpp \
        libsrc/geom2d/python_geom2d.cpp \
        libsrc/meshing/python_mesh.cpp \
        libsrc/occ/vsocc.cpp \
        libsrc/csg/algprim.cpp \
        libsrc/csg/brick.cpp \
        libsrc/csg/bspline2d.cpp \
        libsrc/csg/csgeom.cpp \
        libsrc/csg/csgparser.cpp \
        libsrc/csg/curve2d.cpp \
        libsrc/csg/edgeflw.cpp \
        libsrc/csg/explicitcurve2d.cpp \
        libsrc/csg/extrusion.cpp \
        libsrc/csg/gencyl.cpp \
        libsrc/csg/genmesh.cpp \
        libsrc/csg/identify.cpp \
        libsrc/csg/manifold.cpp \
        libsrc/csg/meshsurf.cpp \
        libsrc/csg/polyhedra.cpp \
        libsrc/csg/revolution.cpp \
        libsrc/csg/singularref.cpp \
        libsrc/csg/solid.cpp \
        libsrc/csg/specpoin.cpp \
        libsrc/csg/spline3d.cpp \
        libsrc/csg/surface.cpp \
        libsrc/csg/triapprox.cpp \
        libsrc/general/array.cpp \
        libsrc/general/bitarray.cpp \
        libsrc/general/dynamicmem.cpp \
        libsrc/general/flags.cpp \
        libsrc/general/gzstream.cpp \
        libsrc/general/hashtabl.cpp \
        libsrc/general/mpi_interface.cpp \
        libsrc/general/mystring.cpp \
        libsrc/general/ngexception.cpp \
        libsrc/general/optmem.cpp \
        libsrc/general/parthreads.cpp \
        libsrc/general/profiler.cpp \
        libsrc/general/seti.cpp \
        libsrc/general/sort.cpp \
        libsrc/general/spbita2d.cpp \
        libsrc/general/symbolta.cpp \
        libsrc/general/table.cpp \
        libsrc/geom2d/genmesh2d.cpp \
        libsrc/geom2d/geom2dmesh.cpp \
        libsrc/geom2d/geometry2d.cpp \
        libsrc/gprim/adtree.cpp \
        libsrc/gprim/geom2d.cpp \
        libsrc/gprim/geom3d.cpp \
        libsrc/gprim/geomfuncs.cpp \
        libsrc/gprim/geomtest3d.cpp \
        libsrc/gprim/spline.cpp \
        libsrc/gprim/splinegeometry.cpp \
        libsrc/gprim/transform3d.cpp \
        libsrc/interface/nginterface.cpp \
        libsrc/interface/nginterface_v2.cpp \
        libsrc/interface/read_fnf_mesh.cpp \
        libsrc/interface/readtetmesh.cpp \
        libsrc/interface/readuser.cpp \
        libsrc/interface/writeabaqus.cpp \
        libsrc/interface/writediffpack.cpp \
        libsrc/interface/writedolfin.cpp \
        libsrc/interface/writeelmer.cpp \
        libsrc/interface/writefeap.cpp \
        libsrc/interface/writefluent.cpp \
        libsrc/interface/writegmsh.cpp \
        libsrc/interface/writegmsh2.cpp \
        libsrc/interface/writejcm.cpp \
        libsrc/interface/writeOpenFOAM15x.cpp \
        libsrc/interface/writepermas.cpp \
        libsrc/interface/writetecplot.cpp \
        libsrc/interface/writetet.cpp \
        libsrc/interface/writetochnog.cpp \
        libsrc/interface/writeuser.cpp \
        libsrc/interface/wuchemnitz.cpp \
        libsrc/linalg/bfgs.cpp \
        libsrc/linalg/densemat.cpp \
        libsrc/linalg/linopt.cpp \
        libsrc/linalg/linsearch.cpp \
        libsrc/linalg/polynomial.cpp \
        libsrc/meshing/adfront2.cpp \
        libsrc/meshing/adfront3.cpp \
        libsrc/meshing/basegeom.cpp \
        libsrc/meshing/bcfunctions.cpp \
        libsrc/meshing/bisect.cpp \
        libsrc/meshing/boundarylayer.cpp \
        libsrc/meshing/clusters.cpp \
        libsrc/meshing/curvedelems.cpp \
        libsrc/meshing/delaunay.cpp \
        libsrc/meshing/delaunay2d.cpp \
        libsrc/meshing/geomsearch.cpp \
        libsrc/meshing/global.cpp \
        libsrc/meshing/hprefinement.cpp \
        libsrc/meshing/improve2.cpp \
        libsrc/meshing/improve2gen.cpp \
        libsrc/meshing/improve3.cpp \
        libsrc/meshing/localh.cpp \
        libsrc/meshing/meshclass.cpp \
        libsrc/meshing/meshfunc.cpp \
        libsrc/meshing/meshfunc2d.cpp \
        libsrc/meshing/meshing2.cpp \
        libsrc/meshing/meshing3.cpp \
        libsrc/meshing/meshtool.cpp \
        libsrc/meshing/meshtype.cpp \
        libsrc/meshing/msghandler.cpp \
        libsrc/meshing/netrule2.cpp \
        libsrc/meshing/netrule3.cpp \
        libsrc/meshing/parallelmesh.cpp \
        libsrc/meshing/paralleltop.cpp \
        libsrc/meshing/parser2.cpp \
        libsrc/meshing/parser3.cpp \
        libsrc/meshing/prism2rls.cpp \
        libsrc/meshing/pyramid2rls.cpp \
        libsrc/meshing/pyramidrls.cpp \
        libsrc/meshing/quadrls.cpp \
        libsrc/meshing/refine.cpp \
        libsrc/meshing/ruler2.cpp \
        libsrc/meshing/ruler3.cpp \
        libsrc/meshing/secondorder.cpp \
        libsrc/meshing/smoothing2.5.cpp \
        libsrc/meshing/smoothing2.cpp \
        libsrc/meshing/smoothing3.cpp \
        libsrc/meshing/specials.cpp \
        libsrc/meshing/tetrarls.cpp \
        libsrc/meshing/topology.cpp \
        libsrc/meshing/triarls.cpp \
        libsrc/meshing/validate.cpp \
        libsrc/occ/occconstruction.cpp \
        libsrc/occ/occgenmesh.cpp \
        libsrc/occ/occgeom.cpp \
        libsrc/occ/occmeshsurf.cpp \
        libsrc/occ/Partition_Inter2d.cxx \
        libsrc/occ/Partition_Inter3d.cxx \
        libsrc/occ/Partition_Loop.cxx \
        libsrc/occ/Partition_Loop2d.cxx \
        libsrc/occ/Partition_Loop3d.cxx \
        libsrc/occ/Partition_Spliter.cxx \
        libsrc/stlgeom/meshstlsurface.cpp \
        libsrc/stlgeom/stlgeom.cpp \
        libsrc/stlgeom/stlgeomchart.cpp \
        libsrc/stlgeom/stlgeommesh.cpp \
        libsrc/stlgeom/stlline.cpp \
        libsrc/stlgeom/stltool.cpp \
        libsrc/stlgeom/stltopology.cpp \
        nglib/nglib.cpp \
    setupimagedialog.cpp


HEADERS  += mainwindow.h \
        qstdredirector.h\
        glwidget.h \
        imageparams.h \
        libsrc/general/ngpython.hpp \
        libsrc/include/incopengl.hpp \
        libsrc/include/incvis.hpp \
        libsrc/include/visual.hpp \
        libsrc/csg/algprim.hpp \
        libsrc/csg/brick.hpp \
        libsrc/csg/csg.hpp \
        libsrc/csg/csgeom.hpp \
        libsrc/csg/csgparser.hpp \
        libsrc/csg/curve2d.hpp \
        libsrc/csg/edgeflw.hpp \
        libsrc/csg/explicitcurve2d.hpp \
        libsrc/csg/extrusion.hpp \
        libsrc/csg/gencyl.hpp \
        libsrc/csg/geoml.hpp \
        libsrc/csg/identify.hpp \
        libsrc/csg/manifold.hpp \
        libsrc/csg/meshsurf.hpp \
        libsrc/csg/polyhedra.hpp \
        libsrc/csg/revolution.hpp \
        libsrc/csg/singularref.hpp \
        libsrc/csg/solid.hpp \
        libsrc/csg/specpoin.hpp \
        libsrc/csg/spline3d.hpp \
        libsrc/csg/surface.hpp \
        libsrc/csg/triapprox.hpp \
        libsrc/csg/vscsg.hpp \
        libsrc/general/archive_base.hpp \
        libsrc/general/array.hpp \
        libsrc/general/autodiff.hpp \
        libsrc/general/autoptr.hpp \
        libsrc/general/bitarray.hpp \
        libsrc/general/dynamicmem.hpp \
        libsrc/general/flags.hpp \
        libsrc/general/gzstream.h \
        libsrc/general/hashtabl.hpp \
        libsrc/general/mpi_interface.hpp \
        libsrc/general/myadt.hpp \
        libsrc/general/mystring.hpp \
        libsrc/general/netgenout.hpp \
        libsrc/general/ngexception.hpp \
        libsrc/general/optmem.hpp \
        libsrc/general/parthreads.hpp \
        libsrc/general/profiler.hpp \
        libsrc/general/seti.hpp \
        libsrc/general/sort.hpp \
        libsrc/general/spbita2d.hpp \
        libsrc/general/stack.hpp \
        libsrc/general/symbolta.hpp \
        libsrc/general/table.hpp \
        libsrc/general/template.hpp \
        libsrc/geom2d/geom2dmesh.hpp \
        libsrc/geom2d/geometry2d.hpp \
        libsrc/geom2d/vsgeom2d.hpp \
        libsrc/gprim/adtree.hpp \
        libsrc/gprim/geom2d.hpp \
        libsrc/gprim/geom3d.hpp \
        libsrc/gprim/geomfuncs.hpp \
        libsrc/gprim/geomobjects.hpp \
        libsrc/gprim/geomobjects2.hpp \
        libsrc/gprim/geomops.hpp \
        libsrc/gprim/geomops2.hpp \
        libsrc/gprim/geomtest3d.hpp \
        libsrc/gprim/gprim.hpp \
        libsrc/gprim/spline.hpp \
        libsrc/gprim/splinegeometry.hpp \
        libsrc/gprim/transform3d.hpp \
        libsrc/include/acisgeom.hpp \
        libsrc/include/csg.hpp \
        libsrc/include/geometry2d.hpp \
        libsrc/include/gprim.hpp \
        libsrc/include/linalg.hpp \
        libsrc/include/meshing.hpp \
        libsrc/include/myadt.hpp \
        libsrc/include/mydefs.hpp \
        libsrc/include/mystdlib.h \
        libsrc/include/nginterface.h \
        libsrc/include/nginterface_v2.hpp \
        libsrc/include/nginterface_v2_impl.hpp \
        libsrc/include/occgeom.hpp \
        libsrc/include/opti.hpp \
        libsrc/include/parallel.hpp \
        libsrc/include/stlgeom.hpp \
        libsrc/interface/writeuser.hpp \
        libsrc/linalg/densemat.hpp \
        libsrc/linalg/linalg.hpp \
        libsrc/linalg/opti.hpp \
        libsrc/linalg/polynomial.hpp \
        libsrc/linalg/vector.hpp \
        libsrc/meshing/adfront2.hpp \
        libsrc/meshing/adfront3.hpp \
        libsrc/meshing/basegeom.hpp \
        libsrc/meshing/bcfunctions.hpp \
        libsrc/meshing/bisect.hpp \
        libsrc/meshing/boundarylayer.hpp \
        libsrc/meshing/classifyhpel.hpp \
        libsrc/meshing/clusters.hpp \
        libsrc/meshing/curvedelems.hpp \
        libsrc/meshing/findip.hpp \
        libsrc/meshing/findip2.hpp \
        libsrc/meshing/geomsearch.hpp \
        libsrc/meshing/global.hpp \
        libsrc/meshing/hpref_hex.hpp \
        libsrc/meshing/hpref_prism.hpp \
        libsrc/meshing/hpref_pyramid.hpp \
        libsrc/meshing/hpref_quad.hpp \
        libsrc/meshing/hpref_segm.hpp \
        libsrc/meshing/hpref_tet.hpp \
        libsrc/meshing/hpref_trig.hpp \
        libsrc/meshing/hprefinement.hpp \
        libsrc/meshing/improve2.hpp \
        libsrc/meshing/improve3.hpp \
        libsrc/meshing/localh.hpp \
        libsrc/meshing/meshclass.hpp \
        libsrc/meshing/meshfunc.hpp \
        libsrc/meshing/meshing.hpp \
        libsrc/meshing/meshing2.hpp \
        libsrc/meshing/meshing3.hpp \
        libsrc/meshing/meshtool.hpp \
        libsrc/meshing/meshtype.hpp \
        libsrc/meshing/msghandler.hpp \
        libsrc/meshing/paralleltop.hpp \
        libsrc/meshing/ruler2.hpp \
        libsrc/meshing/ruler3.hpp \
        libsrc/meshing/specials.hpp \
        libsrc/meshing/topology.hpp \
        libsrc/meshing/validate.hpp \
        libsrc/occ/occgeom.hpp \
        libsrc/occ/occmeshsurf.hpp \
        libsrc/occ/Partition_Inter2d.hxx \
        libsrc/occ/Partition_Inter3d.hxx \
        libsrc/occ/Partition_Loop.hxx \
        libsrc/occ/Partition_Loop2d.hxx \
        libsrc/occ/Partition_Loop3d.hxx \
        libsrc/occ/Partition_Spliter.hxx \
        libsrc/occ/utilities.h \
        libsrc/occ/vsocc.hpp \
        libsrc/stlgeom/meshstlsurface.hpp \
        libsrc/stlgeom/stlgeom.hpp \
        libsrc/stlgeom/stlline.hpp \
        libsrc/stlgeom/stltool.hpp \
        libsrc/stlgeom/stltopology.hpp \
        libsrc/stlgeom/vsstl.hpp \
        nglib/nglib.h \
    libsrc/include/csgeom.hpp \
    setupimagedialog.h


FORMS    += mainwindow.ui \
    setupimagedialog.ui

RESOURCES += \
    qng.qrc

