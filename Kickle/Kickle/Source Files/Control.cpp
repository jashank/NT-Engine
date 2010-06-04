#include "Control.h"

Control::Control()
 : m_rect( 0.0f, 0.0f, 0.0f, 0.0f ),
   m_id( 0 ) {
}

Control::~Control() {
}

void Control::Render() {
}

void Control::Update() {
}

void Control::HandleEvents() {
}

void Control::SetPosition( float x, float y ) {
  m_rect.Left = x;
  m_rect.Top = y;
}

void Control::SetX( float x ) {
  m_rect.Left = x;
}

void Control::SetY( float y ) {
  m_rect.Top = y;
}

float Control::GetX() {
  return m_rect.Left;
}

float Control::GetY() {
  return m_rect.Top;
}

void Control::SetHeight( float height ) {
  m_rect.Bottom = (m_rect.Top+height);
}

void Control::SetWidth( float width ) {
  m_rect.Right = (m_rect.Left+width);
}

float Control::GetHeight() {
  return (m_rect.Bottom-m_rect.Top);
}

float Control::GetWidth() {
  return (m_rect.Right-m_rect.Left);
}

void Control::SetId( unsigned int id ) {
  m_id = id;
}

unsigned int Control::GetId() {
  return m_id;
}
