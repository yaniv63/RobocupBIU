/*
#include "SharedMemory.h"

SharedMemory::SharedMemory(DetectedObject* object)
{
	sem_init(&m_semaphore, 0, 1);
	m_shraedObject = object;
}

SharedMemory::SharedMemory()
{
	sem_init(&m_semaphore, 0, 1);
}

SharedMemory::~SharedMemory()
{
	delete m_shraedObject;
	sem_destroy(&m_semaphore);
}


void SharedMemory::SafeWrite(DetectedObject* objectToWrite)
{
	if (sem_wait(&m_semaphore) != 0)
	{
		cout << "Couldn't sem_wait\n";
	}

	// Free the memory of the old class, then store the new pointer.
	// delete m_shraedObject;
	m_shraedObject = objectToWrite;

	if (sem_post(&m_semaphore) != 0)
	{
		cout << "Couldn't sem_post\n";
	}
}

DetectedObject* SharedMemory::SafeRead()
{
	if (sem_wait(&m_semaphore) != 0)
	{
		cout << "Couldn't sem_wait\n";
	}

	DetectedObject* objectToReturn = m_shraedObject;

	if (sem_post(&m_semaphore) != 0)
	{
		cout << "Couldn't sem_post\n";
	}

	return objectToReturn;
}
*/
