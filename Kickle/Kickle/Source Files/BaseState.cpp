#include "BaseState.h"

BaseState::BaseState() {

}

BaseState::~BaseState() {
}

bool BaseState::IsInit() const {
	return m_initialized;
}

void BaseState::SetInit( bool initialized ) {
	m_initialized = initialized;
}