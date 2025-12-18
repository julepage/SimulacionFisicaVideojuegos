// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  



#include "Camera.h"
#include <ctype.h>
#include "foundation/PxMat33.h"
#include <windows.h>        // Necesario en Visual Studio para usar OpenGL
#include <GL/gl.h>          // glGetIntegerv, glGetDoublev
#include <GL/glu.h>
#include "../globales.h"
#include <iostream>

using namespace physx;

namespace Snippets
{

	Camera::Camera(const PxVec3& eye, const PxVec3& dir)
	{
		
		mEye = eye;
		mEyeIni = eye;
		mDir = dir.getNormalized();
		mDirIni = dir.getNormalized();
		mMouseX = 0;
		mMouseY = 0;
		ultimaDireccion = Vector3D(1, 0, 0);
		setDir(PxVec3(1, 0, 0));
		
	}
	void Camera::resetCamera()
	{
		// Posición inicial de la cámara
		mEye = mEyeIni;  // eye = Vector3D inicial de la cámara
		mDir = mDirIni; // dir = dirección inicial
		ultimaDireccion = Vector3D(1, 0, 0); // dirección por defecto
		mMouseX = 0;
		mMouseY = 0;

		// Configurar dirección de PhysX o similar
		setDir(PxVec3(1, 0, 0)); // vector dirección inicial
	}

	void Camera::handleMouse(int button, int state, int x, int y)
	{
		PX_UNUSED(state);
		PX_UNUSED(button);
		mMouseX = x;
		mMouseY = y;
	}

	bool Camera::handleKey(unsigned char key, int x, int y, float speed)
	{	
		if(pelota->estaParada())
		{
			PX_UNUSED(x);
			PX_UNUSED(y);

			PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();
			switch (toupper(key))
			{
			case 'W':    mEye += mDir * 2.0f * speed;        break;
			case 'S':    mEye -= mDir * 2.0f * speed;        break;
			case 'A':    mEye -= viewY * 2.0f * speed;        break;
			case 'D':    mEye += viewY * 2.0f * speed;        break;
			default:                            return false;
			}
		}
		return true;
	}

	void Camera::handleAnalogMove(float x, float y)
	{
		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();
		mEye += mDir * y;
		mEye += viewY * x;
	}

	void Camera::handleMotion(int x, int y)
	{
		int dx = mMouseX - x;
		int dy = mMouseY - y;

		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0)).getNormalized();

		PxQuat qx(PxPi * dx / 180.0f, PxVec3(0, 1, 0));
		mDir = qx.rotate(mDir);
		PxQuat qy(PxPi * dy / 180.0f, viewY);
		mDir = qy.rotate(mDir);

		mDir.normalize();

		mMouseX = x;
		mMouseY = y;
	}

	PxTransform Camera::getTransform() const
	{
		PxVec3 viewY = mDir.cross(PxVec3(0, 1, 0));

		if (viewY.normalize() < 1e-6f)
			return PxTransform(mEye);

		PxMat33 m(mDir.cross(viewY), viewY, -mDir);
		return PxTransform(mEye, PxQuat(m));
	}

	PxVec3 Camera::getEye() const
	{
		return mEye;
	}

	PxVec3 Camera::getDir() const
	{
		return mDir;
	}

	bool Camera::getPickRay(int mouseX, int mouseY, PxVec3& origin, PxVec3& direction)
	{
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);

		double projection[16];
		glGetDoublev(GL_PROJECTION_MATRIX, projection);

		double modelview[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

		double x = mouseX;
		double y = viewport[3] - mouseY;

		double ox, oy, oz;
		double tx, ty, tz;

		// Punto en el near plane
		gluUnProject(x, y, 0.0, modelview, projection, viewport, &ox, &oy, &oz);

		// Punto en el far plane
		gluUnProject(x, y, 1.0, modelview, projection, viewport, &tx, &ty, &tz);

		origin = PxVec3(ox, oy, oz);
		direction = PxVec3(tx - ox, ty - oy, tz - oz).getNormalized();

		return true;
	}

	void Camera::seguirPelota()
	{
		if (!pelota) return;
		if (pelota->estaParada()) return;

		Vector3D posPelota = pelota->getPos();

		float alturaExtra = 15.0f;//altura sobre la pelota
		float distancia = 40.0f;//distancia detrás (en XZ)
		float distanciaMin = 20.0f;//distancia mínima

		// Vector desde cámara hacia pelota en XZ
		Vector3D camToPelotaXZ = Vector3D(posPelota.getX() - mEye.x, 0, posPelota.getZ() - mEye.z);
		if (camToPelotaXZ.modulo() < 0.01f)
			camToPelotaXZ = Vector3D(0, 0, 1); // dirección por defecto si está directamente encima
		camToPelotaXZ = camToPelotaXZ.normalize();

		// Posición objetivo de la cámara (detrás en XZ y altura relativa)
		Vector3D posCamObj = posPelota - camToPelotaXZ * distancia;
		posCamObj.setY(posPelota.getY() + alturaExtra);

		// Suavizado
		float factorSuavizado = 0.1f;
		mEye = mEye + (PxVec3(posCamObj.getX(), posCamObj.getY(), posCamObj.getZ()) - mEye) * factorSuavizado;

		// Limitar distancia mínima
		Vector3D diff = Vector3D(mEye.x, mEye.y, mEye.z) - posPelota;
		float dist = diff.modulo();
		if (dist < distanciaMin)
		{
			Vector3D diffNorm = diff.normalize();
			Vector3D nuevaPos = posPelota + diffNorm * distanciaMin;
			mEye = PxVec3(nuevaPos.getX(), nuevaPos.getY(), nuevaPos.getZ());
		}

		// Mirar siempre hacia la pelota
		Vector3D dirCam = (posPelota - Vector3D(mEye.x, mEye.y, mEye.z)).normalize();
		mDir = PxVec3(dirCam.getX(), dirCam.getY(), dirCam.getZ());
		
	}

	void Camera::update(float deltaTime)
	{
		seguirPelota();  // ajusta posición y dirección de la cámara
	}
}

