#pragma once
class MeshManager
{
public:
	MeshManager(void);
	virtual ~MeshManager(void);

private:		
	static bool m_Instantiated;
	
};

bool MeshManager::m_Instantiated = false;