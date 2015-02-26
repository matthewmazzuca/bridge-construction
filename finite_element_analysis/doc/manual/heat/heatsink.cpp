/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*- vim:fenc=utf-8:ft=tcl:et:sw=4:ts=4:sts=4

  This file is part of the Feel library

  Author(s): Christophe Prud'homme <christophe.prudhomme@feelpp.org>
       Date: 2007-06-11

  Copyright (C) 2007-2012 Université Joseph Fourier (Grenoble I)
  Copyright (C) 2012-2013 Feel++ Consortium

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <feel/feelalg/backend.hpp>
#include <feel/feelts/bdf.hpp>
#include <feel/feeldiscr/pch.hpp>

#include <feel/feelfilters/loadmesh.hpp>
#include <feel/feelfilters/exporter.hpp>

#include <feel/feelvf/expr.hpp>
#include <feel/feelvf/integrate.hpp>
#include <feel/feelvf/form.hpp>
#include <feel/feelvf/operators.hpp>
#include <feel/feelvf/operations.hpp>
#include <feel/feelvf/measure.hpp>
#include <feel/feelvf/mean.hpp>
#include <feel/feelvf/trans.hpp>
#include <feel/feelvf/vf.hpp>

/// [marker1]
inline
Feel::po::options_description
makeOptions()
{
    Feel::po::options_description heatsinkoptions( "heatsink options" );
    heatsinkoptions.add_options()
    // mesh parameters
    ( "L", Feel::po::value<double>()->default_value( 20 ),
      "dimensional length of the sink (in meters)" )
    ( "width", Feel::po::value<double>()->default_value( 0.5 ),
      "dimensional width of the fin (in meters)" )

    // 3D parameter
    ( "depth", Feel::po::value<double>()->default_value( 0 ),
      "depth of the mesh (in meters) only in 3D simulation" )

    // thermal conductivities parameters
    ( "kappa_s", Feel::po::value<double>()->default_value( 386e-3 ),
      "thermal conductivity of the base spreader in SI unit W.mm^{-1}.K^{-1}" )
    ( "kappa_f", Feel::po::value<double>()->default_value( 386e-3 ),
      "thermal conductivity of the fin in SI unit W.mm^{-1}.K^{-1}" )

    // density parameter
    ( "rho_s", Feel::po::value<double>()->default_value( 8940e-9 ),
      "density of the spreader's material in SI unit kg.mm^{-3}" )
    ( "rho_f", Feel::po::value<double>()->default_value( 8940e-9 ),
      "density of the fin's material in SI unit kg.mm^{-3}" )

    // heat capacities parameter
    ( "c_s", Feel::po::value<double>()->default_value( 385 ),
      "heat capacity of the spreader's material in SI unit J.kg^{-1}.K^{-1}" )
    ( "c_f", Feel::po::value<double>()->default_value( 385 ),
      "heat capacity of the fin's material in SI unit J.kg^{-1}.K^{-1}" )

    // physical coeff
    ( "therm_coeff", Feel::po::value<double>()->default_value( 1e3 ),
      "thermal coefficient" )
    ( "Tamb", Feel::po::value<double>()->default_value( 300 ),
      "ambiant temperature" )
    ( "heat_flux", Feel::po::value<double>()->default_value( 1e6 ),
      "heat flux generated by CPU" )

    ( "steady", Feel::po::value<bool>()->default_value( false ),
      "if true : steady else unsteady" )

    // export
    ( "export-matlab", "export matrix and vectors in matlab" );

    return heatsinkoptions.add( Feel::feel_options() );
}
/// [marker1]

inline
Feel::AboutData
makeAbout()
{
    Feel::AboutData about( "heatsink" ,
                           "heatsink" ,
                           "0.1",
                           "nD(n=1,2,3) Heat sink thermal fin on simplices or simplex products",
                           Feel::AboutData::License_GPL,
                           "Copyright (c) 2006-2011 Universite Joseph Fourier" );

    about.addAuthor( "Christophe Prud'homme", "developer", "christophe.prudhomme@feelpp.org", "" );
    about.addAuthor( "Baptiste Morin", "junior developer", "baptistemorin@gmail.com","" );
    return about;
}


namespace Feel
{
/**
 * Heat sink application
 */
template<int Dim, int Order>
class HeatSink
    :
        public Application
{
    typedef Application super;
public:

#define Entity Simplex

    // -- TYPEDEFS --
    typedef double value_type;

    /*mesh*/
    typedef Entity<Dim> entity_type;
    typedef Mesh<entity_type> mesh_type;
    typedef boost::shared_ptr<mesh_type> mesh_ptrtype;

    typedef FunctionSpace<mesh_type, bases<Lagrange<0, Scalar> >, Discontinuous > p0_space_type;
    typedef typename p0_space_type::element_type p0_element_type;

    /*basis*/
    typedef bases<Lagrange<Order, Scalar> > basis_type;

    /*space*/
    typedef FunctionSpace<mesh_type, basis_type, value_type> space_type;
    typedef boost::shared_ptr<space_type> space_ptrtype;
    typedef typename space_type::element_type element_type;

    /* BDF discretization */
    typedef Bdf<space_type>  bdf_type;
    typedef boost::shared_ptr<bdf_type> bdf_ptrtype;

    /* export */
    typedef Exporter<mesh_type> export_type;

    /* constructor */
    HeatSink();

    /* run the simulation */
    void run();


private:

    /* mesh parameters */
    double depth;
    double L;
    double width;

    /* thermal conductivities */
    double kappa_s;
    double kappa_f;

    /* density of the materials */
    double rho_s;
    double rho_f;

    /* heat capacity of the materials*/
    double c_s;
    double c_f;

    /* thermal coeff */
    double therm_coeff;

    /* ambien temperature, and heat flux (Q) */
    double Tamb;
    double heat_flux;

    /* surfaces*/
    double surface_base, surface_fin;

    /* mesh, pointers and spaces */
    mesh_ptrtype mesh;
    space_ptrtype Xh;

    /* time management */
    bdf_ptrtype M_bdf;
    bool steady;

    // average file
    std::ofstream out;
    double Tavg, Tgamma1;
    boost::shared_ptr<export_type> M_exporter;

}; // HeatSink class

/* Constructor */
template<int Dim, int Order>
HeatSink<Dim,Order>::HeatSink()
    :
    super(),
    depth( doption(_name="depth") ),
    L( doption(_name="L") ),
    width( option(_name="width").template as <double>() ),
    kappa_s( doption(_name="kappa_s") ),
    kappa_f( doption(_name="kappa_f") ),
    rho_s( doption(_name="rho_s") ),
    rho_f( doption(_name="rho_f") ),
    c_s( doption(_name="c_s") ),
    c_f( doption(_name="c_f") ),
    therm_coeff( option(_name="therm_coeff").template as <double>() ),
    Tamb( option(_name="Tamb").template as <double>() ),
    heat_flux( option(_name="heat_flux").template as <double>() ),
    steady( boption(_name="steady") ),
    M_exporter()
{
    this->changeRepository( boost::format( "%1%/%2%/" )
                            % this->about().appName()
                            % doption(_name="gmsh.hsize") );

    // Create mesh: it automatically changes the geometric parameters using the
    // associated options
    mesh = loadMesh( _mesh = new mesh_type );

    // build exporter
    M_exporter = exporter( _mesh=mesh, _geo="static" );

    /// [marker2]
    /*
     * Calculate the two surfaces used for averages calculation
     */
    surface_base = measure( _range= markedfaces( mesh, "gamma4" ) );
    surface_fin = measure( _range= markedfaces( mesh, "gamma1" ) );
    /// [marker2]

    /*
     * The function space associated to the mesh
     */
    Xh = space_type::New( mesh );
    M_bdf = bdf( _space=Xh, _name="Temperature" );

}



template<int Dim, int Order>
void
HeatSink<Dim, Order>::run()
{
    LOG(INFO) << "meshSize = " << doption(_name="gmsh.hsize") << "\n"
              << "L = "<< L <<"\n"
              << "width = " << width << "\n"
              << "depth = " << depth << "\n"
              << "kappa_spreader = " << kappa_s << "\n"
              << "kappa_fin = " << kappa_f << "\n"
              << "rho_spreader = " << rho_s << "\n"
              << "rho_fin = " << rho_f << "\n"
              << "heat capacity of the base = " << c_s << "\n"
              << "heat capacity of the fin = " << c_f << "\n"
              << "ambient temperature = " << Tamb << "\n"
              << "thermal coefficient = " << therm_coeff << "\n"
              << "heat flux Q = " << heat_flux << "\n"
              << "steady state = " << steady << "\n";

    /*
     * T is the unknown, v the test function
     */
    element_type T( Xh, "T" );
		element_type v( Xh, "v" );

    /// [marker3]
    /*
     * Right hand side construction (steady state)
     */
    auto l = form1( _test=Xh );
    auto lt = form1( _test=Xh );
    l = integrate( _range= markedfaces( mesh, "gamma1" ), _expr= therm_coeff*Tamb*id( v ) );

    /*
     * Left hand side construction (steady state)
     */
    auto a = form2( Xh, Xh );
    a = integrate( _range= markedelements( mesh,"spreader" ), _expr= kappa_s*gradt( T )*trans( grad( v ) ) );
    a += integrate( _range= markedelements( mesh,"fin" ), _expr= kappa_f*gradt( T )*trans( grad( v ) ) );
    a += integrate( _range= markedfaces( mesh, "gamma1" ), _expr= therm_coeff*idt( T )*id( v ) );


    M_bdf->start();

    //from now if the option "steady" is set to true then M_bdf->setSteady will set time-step=time-final=1e30
    if ( steady )
    {
        M_bdf->setSteady();
    }

    a +=
        integrate( _range=markedelements( mesh, "spreader" ), _expr=rho_s*c_s*idt( T )*id( v )*M_bdf->polyDerivCoefficient( 0 ) );
    a +=
        integrate( _range=markedelements( mesh, "fin" ), _expr=rho_f*c_f*idt( T )*id( v )*M_bdf->polyDerivCoefficient( 0 ) );
    /// [marker3]

    /*
     * Left and right hand sides construction (non-steady state) with BDF
     */
    T = vf::project( _space=Xh, _range=elements(mesh), _expr=cst( Tamb ) );

    M_bdf->initialize( T );

    Tavg = mean( _range=markedfaces( mesh,"gamma4" ), _expr=idv( T ) )( 0,0 );
    Tgamma1 = mean( _range=markedfaces( mesh,"gamma1" ), _expr=idv( T ) )( 0,0 );
    M_exporter->step( M_bdf->timeInitial() )->addScalar( "Tavg", Tavg );
    M_exporter->step( M_bdf->timeInitial() )->addScalar( "Tgamma1", Tgamma1 );
    M_exporter->step( M_bdf->timeInitial() )->add( "Temperature", T );
    M_exporter->save();


    std::cout << "The step is : " << M_bdf->timeStep() << "\n"
              << "The initial time is : " << M_bdf->timeInitial() << "\n"
              << "The final time is  : " << M_bdf->timeFinal() << "\n";

    // average file which contains: time Tavg_base Tavg_gamma1
    out.open( "averages", std::ios::out );

    /// [marker4]
    for ( M_bdf->start(); M_bdf->isFinished()==false; M_bdf->next(T) )
    {
        lt.zero();

        // update right hand side with time dependent terms
        auto bdf_poly = M_bdf->polyDeriv();
        lt =
            integrate( _range=markedelements( mesh, "spreader" ), _expr=rho_s*c_s*idv( bdf_poly )*id( v ) );
        lt +=
            integrate( _range=markedelements( mesh, "fin" ), _expr=rho_f*c_f*idv( bdf_poly )*id( v ) );
        lt +=
            integrate( _range= markedfaces( mesh,"gamma4" ), _expr= heat_flux*( 1-math::exp( -M_bdf->time() ) )*id( v ) );
        lt += l;

        a.solve( _solution=T, _rhs=lt );

        Tavg = integrate( _range=markedfaces( mesh,"gamma4" ), _expr=( 1/surface_base )*idv( T ) ).evaluate()( 0,0 );
        Tgamma1 = integrate( _range=markedfaces( mesh,"gamma1" ), _expr=( 1/surface_fin )*idv( T ) ).evaluate()( 0,0 );

        // export results
        out << M_bdf->time() << " " << Tavg << " " << Tgamma1 << "\n";

        M_exporter->step( M_bdf->time() )->addScalar( "Tavg", Tavg );
        M_exporter->step( M_bdf->time() )->addScalar( "Tgamma1", Tgamma1 );
        M_exporter->step( M_bdf->time() )->add( "Temperature", T );
        M_exporter->save();

    }
    /// [marker4]

    LOG(INFO) << "Resolution ended, export done \n";

} // HeatSink::run

} // Feel


int
main( int argc, char** argv )
{
    using namespace Feel;
    Environment env( _argc=argc, _argv=argv,
                     _desc=makeOptions(),
                     _about=makeAbout() );

    /* Parameters to be changed */
    const int nDim = 2;
    const int nOrder = 2;

    /* define application */
    typedef Feel::HeatSink<nDim, nOrder> heat_sink_type;

    /* instanciate */
    heat_sink_type heatsink;

    /* run */
    heatsink.run();
}
