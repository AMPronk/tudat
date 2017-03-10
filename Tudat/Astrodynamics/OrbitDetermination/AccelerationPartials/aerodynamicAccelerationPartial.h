/*    Copyright (c) 2010-2017, Delft University of Technology
 *    All rigths reserved
 *
 *    This file is part of the Tudat. Redistribution and use in source and
 *    binary forms, with or without modification, are permitted exclusively
 *    under the terms of the Modified BSD license. You should have received
 *    a copy of the license with this file. If not, please or visit:
 *    http://tudat.tudelft.nl/LICENSE.
 */

#ifndef TUDAT_AERODYNAMICACCELERATIONPARTIALS_H
#define TUDAT_AERODYNAMICACCELERATIONPARTIALS_H

#include "Tudat/Astrodynamics/Aerodynamics/aerodynamicAcceleration.h"
#include "Tudat/Astrodynamics/Aerodynamics/flightConditions.h"
#include "Tudat/Astrodynamics/ReferenceFrames/aerodynamicAngleCalculator.h"

#include "Tudat/Astrodynamics/OrbitDetermination/AccelerationPartials/accelerationPartial.h"

namespace tudat
{

namespace acceleration_partials
{



class AerodynamicAccelerationPartial: public AccelerationPartial
{
public:

    AerodynamicAccelerationPartial(
            const boost::shared_ptr< aerodynamics::AerodynamicAcceleration > aerodynamicAcceleration,
            const boost::shared_ptr< aerodynamics::FlightConditions > flightConditions,
            const boost::function< Eigen::Vector6d( ) > vehicleStateGetFunction,
            const boost::function< void( const Eigen::Vector6d& ) > vehicleStateSetFunction,
            const std::string acceleratedBody,
            const std::string acceleratingBody );

    //! Function for calculating the partial of the acceleration w.r.t. the position of body undergoing acceleration..
    /*!
     *  Function for calculating the partial of the acceleration w.r.t. the position of body undergoing acceleration
     *  and adding it to the existing partial block
     *  Update( ) function must have been called during current time step before calling this function.
     *  \param partialMatrix Block of partial derivatives of acceleration w.r.t. Cartesian position of body
     *  undergoing acceleration where current partial is to be added.
     *  \param addContribution Variable denoting whether to return the partial itself (true) or the negative partial (false).
     *  \param startRow First row in partialMatrix block where the computed partial is to be added.
     *  \param startColumn First column in partialMatrix block where the computed partial is to be added.
     */
    void wrtPositionOfAcceleratedBody(
            Eigen::Block< Eigen::MatrixXd > partialMatrix,
            const bool addContribution = 1, const int startRow = 0, const int startColumn = 0 )
    {
        if( addContribution )
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) += currentAccelerationStatePartials_.block( 0, 0, 3, 3 );
        }
        else
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) -= currentAccelerationStatePartials_.block( 0, 0, 3, 3 );
        }
    }

    void wrtVelocityOfAcceleratedBody(
            Eigen::Block< Eigen::MatrixXd > partialMatrix,
            const bool addContribution = 1, const int startRow = 0, const int startColumn = 0 )
    {
        if( addContribution )
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) += currentAccelerationStatePartials_.block( 0, 3, 3, 3 );
        }
        else
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) -= currentAccelerationStatePartials_.block( 0, 3, 3, 3 );
        }
    }

    //! Function for calculating the partial of the acceleration w.r.t. the velocity of body undergoing acceleration..
    /*!
     *  Function for calculating the partial of the acceleration w.r.t. the velocity of body undergoing acceleration and
     *  adding it to the existing partial block.
     *  The update( ) function must have been called during current time step before calling this function.
     *  \param partialMatrix Block of partial derivatives of acceleration w.r.t. Cartesian position of body
     *  exerting acceleration where current partial is to be added.
     *  \param addContribution Variable denoting whether to return the partial itself (true) or the negative partial (false).
     *  \param startRow First row in partialMatrix block where the computed partial is to be added.
     *  \param startColumn First column in partialMatrix block where the computed partial is to be added.
     */
    void wrtPositionOfAcceleratingBody( Eigen::Block< Eigen::MatrixXd > partialMatrix,
                                        const bool addContribution = 1, const int startRow = 0, const int startColumn = 0 )
    {
        if( addContribution )
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) -= currentAccelerationStatePartials_.block( 0, 0, 3, 3 );
        }
        else
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) += currentAccelerationStatePartials_.block( 0, 0, 3, 3 );
        }
    }

    void wrtVelocityOfAcceleratingBody( Eigen::Block< Eigen::MatrixXd > partialMatrix,
                                        const bool addContribution = 1, const int startRow = 0, const int startColumn = 0 )
    {
        if( addContribution )
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) -= currentAccelerationStatePartials_.block( 0, 3, 3, 3 );
        }
        else
        {
            partialMatrix.block( startRow, startColumn, 3, 3 ) += currentAccelerationStatePartials_.block( 0, 3, 3, 3 );
        }
    }

    //! Function for determining if the acceleration is dependent on a non-translational integrated state.
    /*!
     *  Function for determining if the acceleration is dependent on a non-translational integrated state.
     *  No dependency is implemented, but a warning is provided if partial w.r.t. mass of body exerting acceleration
     *  (and undergoing acceleration if mutual attraction is used) is requested.
     *  \param stateReferencePoint Reference point id of propagated state
     *  \param integratedStateType Type of propagated state for which dependency is to be determined.
     *  \return True if dependency exists (non-zero partial), false otherwise.
     */
    bool isStateDerivativeDependentOnIntegratedNonTranslationalState(
                const std::pair< std::string, std::string >& stateReferencePoint,
                const propagators::IntegratedStateType integratedStateType )
    {
        if( ( ( stateReferencePoint.first == acceleratingBody_ ||
              ( stateReferencePoint.first == acceleratedBody_  ) )
              && integratedStateType == propagators::body_mass_state ) )
        {
            throw std::runtime_error( "Warning, dependency of central gravity on body masses not yet implemented" );
        }
        return 0;
    }

    //! Function for setting up and retrieving a function returning a partial w.r.t. a double parameter.
    /*!
     *  Function for setting up and retrieving a function returning a partial w.r.t. a double parameter.
     *  Function returns empty function and zero size indicator for parameters with no dependency for current acceleration.
     *  \param parameter Parameter w.r.t. which partial is to be taken.
     *  \return Pair of parameter partial function and number of columns in partial (0 for no dependency, 1 otherwise).
     */
    std::pair< boost::function< void( Eigen::MatrixXd& ) >, int >
    getParameterPartialFunction( boost::shared_ptr< estimatable_parameters::EstimatableParameter< double > > parameter )
    {
        boost::function< void( Eigen::MatrixXd& ) > partialFunction;
        int numberOfColumns = 0;

        // Check if parameter is gravitational parameter.
        if( parameter->getParameterName( ).first ==  estimatable_parameters::constant_drag_coefficient )
        {
            // Check if parameter body is accelerated body,
            if( parameter->getParameterName( ).second.first == acceleratedBody_ )
            {
                partialFunction = boost::bind(
                            &AerodynamicAccelerationPartial::computeAccelerationPartialWrtCurrentDragCoefficient,
                            this, _1 );
                numberOfColumns = 1;

            }
        }

        return std::make_pair( partialFunction, numberOfColumns );
    }

    //! Function for setting up and retrieving a function returning a partial w.r.t. a vector parameter.
    /*!
     *  Function for setting up and retrieving a function returning a partial w.r.t. a vector parameter.
     *  Function returns empty function and zero size indicator for parameters with no dependency for current acceleration.
     *  \param parameter Parameter w.r.t. which partial is to be taken.
     *  \return Pair of parameter partial function and number of columns in partial (0 for no dependency).
     */
    std::pair< boost::function< void( Eigen::MatrixXd& ) >, int > getParameterPartialFunction(
            boost::shared_ptr< estimatable_parameters::EstimatableParameter< Eigen::VectorXd > > parameter )
    {
        boost::function< void( Eigen::MatrixXd& ) > partialFunction;
        return std::make_pair( partialFunction, 0 );
    }

    void computeAccelerationPartialWrtCurrentDragCoefficient( Eigen::MatrixXd& accelerationPartial )
    {
        Eigen::Quaterniond rotationToAerodynamicFrame =
                flightConditions_->getAerodynamicAngleCalculator( )->getRotationQuaternionBetweenFrames(
                   reference_frames::inertial_frame, reference_frames::aerodynamic_frame );

        double currentAirspeed = flightConditions_->getCurrentAirspeed( );
        accelerationPartial =
                rotationToAerodynamicFrame * Eigen::Vector3d::UnitX( ) * (
                0.5 * flightConditions_->getCurrentDensity( ) * currentAirspeed * currentAirspeed *
                flightConditions_->getAerodynamicCoefficientInterface( )->getReferenceArea( ) );

    }

    //! Function for updating partial w.r.t. the bodies' positions
    /*!
     *  Function for updating common blocks of partial to current state. For the central gravitational acceleration,
     *  position partial is computed and set.
     *  \param currentTime Time at which partials are to be calculated
     */
    void update( const double currentTime = TUDAT_NAN )
    {
        Eigen::Vector6d nominalState = vehicleStateGetFunction_( );
        Eigen::Vector6d perturbedState;

        Eigen::Vector3d upperturbedAcceleration, downperturbedAcceleration;
        for( unsigned int i = 0; i < 6; i++ )
        {
            perturbedState = nominalState;
            perturbedState( i ) += bodyStatePerturbations_( i );

            flightConditions_->resetCurrentTime( TUDAT_NAN );
            aerodynamicAcceleration_->resetTime( TUDAT_NAN );
            vehicleStateSetFunction_( perturbedState );

            flightConditions_->updateConditions( currentTime );
            aerodynamicAcceleration_->updateMembers( currentTime );
            upperturbedAcceleration = aerodynamicAcceleration_->getAcceleration( );

            perturbedState = nominalState;
            perturbedState( i ) -= bodyStatePerturbations_( i );

            flightConditions_->resetCurrentTime( TUDAT_NAN );
            aerodynamicAcceleration_->resetTime( TUDAT_NAN );
            vehicleStateSetFunction_( perturbedState );

            flightConditions_->updateConditions( currentTime );
            aerodynamicAcceleration_->updateMembers( currentTime );
            downperturbedAcceleration = aerodynamicAcceleration_->getAcceleration( );

            currentAccelerationStatePartials_.block( 0, i, 3, 1 ) =
                    ( upperturbedAcceleration - downperturbedAcceleration ) / ( 2.0 * bodyStatePerturbations_( i ) );

        }

        flightConditions_->resetCurrentTime( TUDAT_NAN );
        aerodynamicAcceleration_->resetTime( TUDAT_NAN );
        vehicleStateSetFunction_( nominalState );

        flightConditions_->updateConditions( currentTime );
        aerodynamicAcceleration_->updateMembers( currentTime );
    }

protected:

    Eigen::Vector6d bodyStatePerturbations_;

    Eigen::Matrix< double, 3, 6 > currentAccelerationStatePartials_;

    boost::shared_ptr< aerodynamics::FlightConditions > flightConditions_;

    boost::shared_ptr< aerodynamics::AerodynamicAcceleration > aerodynamicAcceleration_;

    boost::function< Eigen::Vector6d( ) > vehicleStateGetFunction_;

    boost::function< void( const Eigen::Vector6d& ) > vehicleStateSetFunction_;

};

} // namespace acceleration_partials

} // namespace tudat

#endif // TUDAT_AERODYNAMICACCELERATIONPARTIALS_H
