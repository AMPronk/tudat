/*! \file tabulatedAtmosphere.h
 *    Header file that defines the tabulatedAtmosphere class included in Tudat.
 *
 *    Path              : /Astrodynamics/EnvironmentModels/AtmosphereModel/
 *    Version           : 3
 *    Check status      : Checked
 *
 *    Author            : F.M. Engelen
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : F.M.Engelen@student.tudelft.nl
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 20 June, 2011
 *    Last modified     : 21 July, 2011
 *
 *    References
 *
 *    Notes
 *    The provided USSA1976 table file, generated with the pascal file, has a small error
 *    which can be observed at the pressure at sea level. This in 101320 in the file
 *    but should be 101325. If this error is not acceptable, an other table file should be
 *    used.
 *
 *    Copyright (c) 2010-2011 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      110620    F.M. Engelen      File created.
 *      110721    J. Melman         Comments, file names,
 *                                  and consistency modified.
 */


#ifndef TABULATEDATMOSPHERE_H
#define TABULATEDATMOSPHERE_H

//include statements
#include "atmosphereModel.h"
#include <string>
#include <iostream>
#include <sstream>
#include "textFileReader.h"
#include "linearAlgebra.h"
#include "cubicSplineInterpolation.h"

//using statements
using std::stringstream;
using std::string;

//! Tabulated atmosphere class.
/*!
 * Tabulated atmospheres class, for example US1976. The default path
 * from which the files are obtained is:
 * Astrodynamics/EnvironmentModels/AtmosphereModel/AtmosphereTables
 * NOTE: for the moment it only works for tables with 4 columns:
 * altitude, density, pressure and temperature.
 */
class TabulatedAtmosphere : public AtmosphereModel
{
public:
    //! Default constructor.
    /*!
     *  Default constructor
     */
    TabulatedAtmosphere( );

    //! Default destructor.
    /*!
     *  Default destructor.
     */
    ~TabulatedAtmosphere( );

    //! Initialise the atmosphere table reader.
    /*!
     * Initialise the atmosphere table reader.
     * \param atmosphereTableFile The name of the atmosphere table.
     */
    void initialize( string atmosphereTableFile );

    //! Get atmosphere table file name.
    /*!
     *  Get atmosphere table file name.
     * \return The atmosphere table file.
     */
    string getAtmosphereTableFile( );

    //! Get relative path.
    /*!
     * Get relative path.
     * \return The relative path.
     */
    string getRelativePath( );

    //! Get local density.
    /*!
     * Return the local density parameter of the atmosphere in kg per meter^3.
     * \param altitude Altitude.
     * \param longitude Longitude.
     * \param latitude Latitude.
     * \param time Time.
     * \return Atmospheric density.
     */
    double getDensity( const double& altitude,
                       const double& longitude,
                       const double& latitude,
                       const double& time = 0.0 );

    //! Get local density.
    /*!
     * Return the local density parameter of the atmosphere in kg per meter^3.
     * \param altitude Altitude.
     * \return Atmospheric density.
     */
    double getDensity( const double& altitude );


    //! Get local pressure.
    /*!
     * Return the local pressure of the atmosphere in Newton per meter^2.
     * \param altitude Altitude.
     * \param longitude Longitude.
     * \param latitude Latitude.
     * \param time Time.
     * \return Atmospheric pressure.
     */
    double getPressure( const double& altitude,
                        const double& longitude,
                        const double& latitude,
                        const double& time = 0.0 );

    //! Get local pressure.
    /*!
     * Return the local pressure of the atmosphere in Newton per meter^2.
     * \param altitude Altitude.
     * \return Atmospheric pressure.
     */
    double getPressure( const double& altitude );

    //! Get local temperature.
    /*!
     * Return the local temperature of the atmosphere in Kelvin.
     * \param altitude Altitude.
     * \param longitude Longitude.
     * \param latitude Latitude.
     * \param time Time.
     * \return Atmospheric temperature.
     */
    double getTemperature( const double& altitude,
                           const double& longitude,
                           const double& latitude,
                           const double& time = 0.0 );

    //! Get local temperature.
    /*!
     * Return the local temperature of the atmosphere parameter in Kelvin.
     * \param altitude Altitude.
     * \return Atmospheric temperature.
     */
    double getTemperature( const double& altitude );


protected:

private:
    //! The relative path
    /*!
     *  The relative path
     */
    string relativePath_;

    //! The file name of the atmosphere table.
    /*!
     *  The file name of the atmosphere table.
     */
    string atmosphereTableFile_;

    //! The file reader.
    /*!
     *  The file reader.
     */
    TextFileReader textFileReader;

    //! Container to store the read atmosphere table file.
    /*!
     *  Container to store the read atmosphere table file.
     */
    map< unsigned int, string > containerOfAtmosphereTableFileData;

    //! Vector containing the altitude.
    /*!
     *  Vector containing the altitude.
     */
    VectorXd altitudeData_;

    //! Vector containing the density data as a function of the altitude.
    /*!
     *  Vector containing the density data as a function of the altitude.
     */
    VectorXd densityData_;

    //! Vector containing the pressure data as a function of the altitude.
    /*!
     *  Vector containing the pressure data as a function of the altitude.
     */
    VectorXd pressureData_;

    //! Vector containing the temperature data as a function of the altitude.
    /*!
     *  Vector containing the temperature data as a function of the altitude.
     */
    VectorXd temperatureData_;

    //! Iterator for map container of data from file.
    /*!
     *  Iterator for map container of string data from data file.
     */
    map< unsigned int, string >::iterator iteratorContainerOfData_;

    //! Cubic spline interpolation for density.
    /*!
     *  Cubic spline interpolation for density.
     */
    CubicSplineInterpolation cubicSplineInterpolationForDensity_;

    //! Cubic spline interpolation for pressure.
    /*!
     *  Cubic spline interpolation for pressure.
     */
    CubicSplineInterpolation cubicSplineInterpolationForPressure_;

    //! Cubic spline interpolation for temperature.
    /*!
     *  Cubic spline interpolation for temperature.
     */
    CubicSplineInterpolation cubicSplineInterpolationForTemperature_;

};

#endif // TABULATEDATMOSPHERE_H

// End of file.
