//
// Camera
//

#include "BCamera.h"

BCamera::BCamera() {
  SetInitialLocation();
}

void BCamera::SetInitialLocation() {
  m_vFixLocation.Set(100 - 5, 60 - 5, -3.0);
	m_vLocation = m_vFixLocation;
  m_orientation.m_vForward = BVector(100, 60, -3.0) - m_vLocation;
  m_orientation.m_vUp = BVector(0, 0, -1);
  m_orientation.m_vRight = m_orientation.m_vForward.CrossProduct(m_orientation.m_vUp);
  m_orientation.Normalize();
  m_locMode = FIXED;
  m_bInitLoc = true;
  m_dAngleOfView = 75.0;
}
