/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkInitialValueProblemSolver.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkInitialValueProblemSolver - Integrate a set of ordinary
// differential equations (initial value problem) in time.

// .SECTION Description
// Given a vtkFunctionSet which returns dF_i(x_j, t)/dt given x_j and
// t, vtkInitialValueProblemSolver computes the value of F_i at t+deltat.

// .SECTION Warning
// vtkInitialValueProblemSolver and it's subclasses are not thread-safe.
// You should create a new integrator for each thread.

// .SECTION See Also
// vtkRungeKutta2 vtkRungeKutta4

#ifndef __vtkInitialValueProblemSolver_h
#define __vtkInitialValueProblemSolver_h

#include "vtkFunctionSet.h"

class VTK_COMMON_EXPORT vtkInitialValueProblemSolver : public vtkObject
{
public:
  vtkTypeRevisionMacro(vtkInitialValueProblemSolver,vtkObject);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Given initial values, xprev , initial time, t and a requested time 
  // interval, delT calculate values of x at t+delTActual (xnext).
  // For certain concrete sub-classes delTActual != delT. This occurs
  // when the solver supports adaptive stepsize control. If this
  // is the case, the solver tries to change to stepsize such that
  // the (estimated) error of the integration is less than maxError.
  // The solver will not set the stepsize smaller than minStep or
  // larger than maxStep.
  // Also note that delT is an in/out argument. Adaptive solvers
  // will modify delT to reflect the best (estimated) size for the next
  // integration step.
  // An estimated value for the error is returned (by reference) in error.
  // Note that only some concrete sub-classes support this. Otherwise,
  // the error is set to 0.
  // This method returns an error code representing the nature of
  // the failure:
  // OutOfDomain = 1,
  // NotInitialized = 2,
  // UnexpectedValue = 3
  virtual int ComputeNextStep(float* xprev, float* xnext, float t,
			      float& delT, float maxError, 
			      float& error) 
    {
      float minStep = delT;
      float maxStep = delT;
      float delTActual;
      return this->ComputeNextStep(xprev, 0, xnext, t, delT, delTActual,
				   minStep, maxStep, maxError, error);
    }
  virtual int ComputeNextStep(float* xprev, float* dxprev, float* xnext, 
			      float t, float& delT, float maxError, 
			      float& error)
    {
      float minStep = delT;
      float maxStep = delT;
      float delTActual;
      return this->ComputeNextStep(xprev, dxprev, xnext, t, delT, delTActual,
				   minStep, maxStep, maxError, error);
    }
  virtual int ComputeNextStep(float* xprev, float* xnext, 
			      float t, float& delT, float& delTActual,
			      float minStep, float maxStep,
			      float maxError, float& error)
    {
      return this->ComputeNextStep(xprev, 0, xnext, t, delT, delTActual,
				   minStep, maxStep, maxError, error);
    }
  virtual int ComputeNextStep(float* xprev, float* dxprev, float* xnext, 
			      float t, float& delT, float& delTActual, 
			      float minStep, float maxStep, 
			      float maxError, float& error) = 0;

  // Description:
  // Create concrete instance of the object.
  virtual vtkInitialValueProblemSolver *MakeObject()=0;
  
  // Description:
  // Set / get the dataset used for the implicit function evaluation.
  virtual void SetFunctionSet(vtkFunctionSet* functionset);
  vtkGetObjectMacro(FunctionSet,vtkFunctionSet);

  // Description:
  // Returns 1 if the solver uses adaptive stepsize control,
  // 0 otherwise
  virtual int IsAdaptive() { return this->Adaptive; }
  
//BTX
  enum ErrorCodes
  {
    OutOfDomain = 1,
    NotInitialized = 2,
    UnexpectedValue = 3
  };
//ETX

protected:
  vtkInitialValueProblemSolver();
  ~vtkInitialValueProblemSolver();

  virtual void Initialize();

  vtkFunctionSet* FunctionSet;

  float* Vals;
  float* Derivs;
  int Initialized;
  int Adaptive;

private:
  vtkInitialValueProblemSolver(const vtkInitialValueProblemSolver&);  // Not implemented.
  void operator=(const vtkInitialValueProblemSolver&);  // Not implemented.
};

#endif




