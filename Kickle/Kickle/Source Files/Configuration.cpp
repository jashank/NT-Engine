#include "Configuration.h"

Configuration::Configuration() {
	m_width = 1024;
	m_height = 768;
}

Configuration::~Configuration() {
}
	
void Configuration::SetHeight( unsigned int height ) {
	m_height = height;
}

unsigned int Configuration::GetHeight() {
	return m_height;
}

void Configuration::SetWidth( unsigned int width ) {
	m_width = width;
}

unsigned int Configuration::GetWidth() {
	return m_width;
}