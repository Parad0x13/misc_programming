/*	Functions for the Camera!
	This source is not from me, I mearly used it
	from a tutorial online... I have yet to
	fully understand all the progrmming
	concepts behind this, but it will have
	to do untill I learn otherwise	*/

#include "main.h"

// CCamera's Position Camera Function

void CCamera::Position_Camera(float pos_x, float pos_y, float pos_z,
							  float view_x, float view_y, float view_z,
							  float up_x, float up_y, float up_z)
{
	mPos=tVector3(pos_x, pos_y, pos_z);		// Set Position
	mView=tVector3(view_x, view_y, view_z);	// Set View
	mUp=tVector3(up_x, up_y, up_z);			// Set Up Vector
}

// CCamera's Move Camera Function

void CCamera::Move_Camera(float speed)
{
	tVector3 vVector=mView-mPos;	// Gets the View Vector

	// Forward, positive, camera speeds and backward, negative, camera speeds
	mPos.x=mPos.x+vVector.x*speed;
	mPos.z=mPos.z+vVector.z*speed;
	mView.x=mView.x+vVector.x*speed;
	mView.z=mView.z+vVector.z*speed;
}

// CCamera's Rotate View Function

void CCamera::Rotate_View(float speed)
{
	tVector3 vVector = mView - mPos;	// The View Vector

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}

// CCamera's Strafe View Function

void CCamera::Strafe_Camera(float speed)
{
	tVector3 vVector = mView - mPos;	// Get the view vector
	tVector3 vOrthoVector;              // Orthogonal vector for the view vector

	vOrthoVector.x = -vVector.z;
	vOrthoVector.z =  vVector.x;

	// left positive cameraspeed and right negative -cameraspeed.
	mPos.x  = mPos.x  + vOrthoVector.x * speed;
	mPos.z  = mPos.z  + vOrthoVector.z * speed;
	mView.x = mView.x + vOrthoVector.x * speed;
	mView.z = mView.z + vOrthoVector.z * speed;
}
