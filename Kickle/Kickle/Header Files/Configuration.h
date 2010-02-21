#ifndef CONFIGURATION_H
#define CONFIGURATION_H

class Configuration {
public:
	Configuration();
	virtual ~Configuration();
	
	void SetHeight( unsigned int height );
	unsigned int GetHeight();

	void SetWidth( unsigned int width );
	unsigned int GetWidth();

private:
	unsigned int m_height;
	unsigned int m_width;
};

#endif