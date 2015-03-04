/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ============================================================================
 * DerivativeHelpers uses code adapated from the following vtk modules:
 *
 * * vtkQuad.cxx
 *
 * The vtk license is reproduced below.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ============================================================================
 * Copyright (c) 1993-2008 Ken Martin, Will Schroeder, Bill Lorensen
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names of
 * any contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "DerivativeHelpers.h"

#include "QuadGeom.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void DerivativeHelpers::QuadDeriv::operator()(QuadGeom* quads, int64_t quadId, float* values, float* derivs, int dim)
{
  float v0[2], v1[2], v2[2], v3[2], v10[3], v20[3], lenX, lenTmp;
  float x0[3], x1[3], x2[3], x3[3], n[3], vec20[3], vec30[3], pCoords[3];
  float funcDerivs[8], sum[2], dBydx, dBydy;
  float* J[2], J0[2], J1[2];
  float* JI[2], JI0[2], JI1[2];
  int64_t verts[4];

  // Project points of quad into 2D system
  quads->getVertsAtQuad(quadId, verts);
  quads->getCoords(verts[0], x0);
  quads->getCoords(verts[1], x1);
  quads->getCoords(verts[2], x2);
  quads->getCoords(verts[3], x3);

  QuadGeom::ComputeNormal(x0, x1, x2, n);

  if (n[0] == 0.0 && n[1] == 0.0 && n[2] == 0.0)
  {
    QuadGeom::ComputeNormal(x1, x2, x3, n);
  }

  for (size_t i = 0; i < 3; i++)
  {
    v10[i] = x1[i] - x0[i];
    vec20[i] = x2[i] - x0[i];
    vec30[i] = x3[i] - x0[i];
  }

  MatrixMath::CrossProduct(n, v10, v20);

  lenX = sqrtf(v10[0] * v10[0] + v10[1] * v10[1] + v10[2] * v10[2]);
  lenTmp = sqrtf(v20[0] * v20[0] + v20[1] * v20[1] + v20[2] * v20[2]);

  if ( lenX <= 0.0
       || lenTmp <= 0.0 ) //degenerate
  {
    for (int j=0; j < dim; j++ )
    {
      for (int i=0; i < 3; i++ )
      {
        derivs[j*dim + i] = 0.0;
      }
    }
    return;
  }

  v0[0] = v0[1] = 0.0; //convert points to 2D (i.e., local system)
  v1[0] = lenX; v1[1] = 0.0;
  v2[0] = MatrixMath::DotProduct3x1(vec20, v10);
  v2[1] = MatrixMath::DotProduct3x1(vec20, v20);
  v3[0] = MatrixMath::DotProduct3x1(vec30, v10);
  v3[1] = MatrixMath::DotProduct3x1(vec30, v20);

  QuadGeom::GetParametricCenter(pCoords);
  QuadGeom::InterpolationDerivatives(pCoords, funcDerivs);

//  float jPtr[4];

//  jPtr[0] = v0[0]*funcDerivs[0] + v1[0]*funcDerivs[1] +
//            v2[0]*funcDerivs[2] + v3[0]*funcDerivs[3];
//  jPtr[1] = v0[1]*funcDerivs[0] + v1[1]*funcDerivs[1] +
//            v2[1]*funcDerivs[2] + v3[1]*funcDerivs[3];
//  jPtr[2] = v0[0]*funcDerivs[4] + v1[0]*funcDerivs[5] +
//            v2[0]*funcDerivs[6] + v3[0]*funcDerivs[7];
//  jPtr[3] = v0[1]*funcDerivs[4] + v1[1]*funcDerivs[5] +
//            v2[1]*funcDerivs[6] + v3[1]*funcDerivs[7];

//  Eigen::Map<QuadJacobian> jMat(jPtr);
//  QuadJacobian jMatI;
//  bool invertible;

//  jMat.computeInverseWithCheck(jMatI, invertible);

  // Compute Jacobian and inverse Jacobian
  J[0] = J0; J[1] = J1;
  JI[0] = JI0; JI[1] = JI1;

  J[0][0] = v0[0]*funcDerivs[0] + v1[0]*funcDerivs[1] +
            v2[0]*funcDerivs[2] + v3[0]*funcDerivs[3];
  J[0][1] = v0[1]*funcDerivs[0] + v1[1]*funcDerivs[1] +
            v2[1]*funcDerivs[2] + v3[1]*funcDerivs[3];
  J[1][0] = v0[0]*funcDerivs[4] + v1[0]*funcDerivs[5] +
            v2[0]*funcDerivs[6] + v3[0]*funcDerivs[7];
  J[1][1] = v0[1]*funcDerivs[4] + v1[1]*funcDerivs[5] +
            v2[1]*funcDerivs[6] + v3[1]*funcDerivs[7];

  // Compute inverse Jacobian, return if Jacobian is singular
  if (!DerivativeHelpers::QuadDeriv::InvertMatrix(J,JI,2))
    {
    for (size_t j=0; j < dim; j++ )
      {
      for (size_t i=0; i < 3; i++ )
        {
        derivs[j*dim + i] = 0.0;
        }
      }
    return;
    }

//  if (!invertible)
//  {
//    for (int j = 0; j < dim; j++ )
//    {
//      for (int i = 0; i < 3; i++ )
//      {
//        derivs[j*dim+i] = 0.0;
//      }
//    }
//    qDebug() << "failed at" << quadId;
//    qDebug() << "fail values" << values[0] << " " << values[1] << " " << values[2] << " " << values[3];
//    qDebug() << "fail jacobian" << jPtr[0] << " " << jPtr[1] << " " << jPtr[2] << " " << jPtr[3];
//    return;
//  }

//  if (invertible)
//  {
//    qDebug() << "success at" << quadId;
//    qDebug() << "success values" << values[0] << " " << values[1] << " " << values[2] << " " << values[3];
//    qDebug() << "success jacobian" << jPtr[0] << " " << jPtr[1] << " " << jPtr[2] << " " << jPtr[3];
//  }

  // Loop over "dim" derivative values. For each set of values,
  // compute derivatives
  // in local system and then transform into modelling system.
  // First compute derivatives in local x'-y' coordinate system
  for (int j = 0; j < dim; j++ )
  {
    sum[0] = sum[1] = 0.0;
    for (size_t i=0; i < 4; i++) //loop over interp. function derivatives
    {
      sum[0] += funcDerivs[i] * values[dim*i + j];
      sum[1] += funcDerivs[4 + i] * values[dim*i + j];
    }

//    dBydx = sum[0]*jMatI(0,0) + sum[1]*jMatI(0,1);
//    dBydy = sum[0]*jMatI(1,0) + sum[1]*jMatI(1,1);

    dBydx = sum[0]*JI[0][0] + sum[1]*JI[0][1];
    dBydy = sum[0]*JI[1][0] + sum[1]*JI[1][1];

    // Transform into global system (dot product with global axes)
    derivs[3*j] = dBydx * v10[0] + dBydy * v20[0];
    derivs[3*j + 1] = dBydx * v10[1] + dBydy * v20[1];
    derivs[3*j + 2] = dBydx * v10[2] + dBydy * v20[2];
  }
}
