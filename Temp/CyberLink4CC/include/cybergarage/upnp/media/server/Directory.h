/******************************************************************
*
*	MediaServer for CyberLink
*
*	Copyright (C) Satoshi Konno 2003
*
*	File : Directory.h
*
*	Revision:
*
*	03/23/04
*		- first revision.
*
******************************************************************/

#ifndef _CLINK_MEDIA_DIRECTORY_H_
#define _CLINK_MEDIA_DIRECTORY_H_

#include <cybergarage/upnp/media/server/object/container/ContainerNode.h>
#include <uhttp/util/Mutex.h>

namespace CyberLink {

class Directory : public ContainerNode
{
private:

	uHTTP::Mutex *mutex;

	////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////
	
public:

	Directory(ContentDirectory *cdir, const char *name)
	{
		setContentDirectory(cdir);
		setFriendlyName(name);
		mutex = new uHTTP::Mutex();
	}

	Directory(const char *name)
	{
		setContentDirectory(NULL);
		setFriendlyName(name);
		mutex = new uHTTP::Mutex();
	}
	
    virtual ~Directory()
    {
        delete mutex;
    }
	////////////////////////////////////////////////
	// Name
	////////////////////////////////////////////////

public:

	void setFriendlyName(const char *name)
	{
		setTitle(name);
	}
	
	const char *getFriendlyName()
	{
		return getTitle();
	}
	
	
	////////////////////////////////////////////////
	// Mutex
	////////////////////////////////////////////////

public:

	void lock()
	{
		mutex->lock();
	}
	
	void unlock()
	{
		mutex->unlock();
	}

	////////////////////////////////////////////////
	// update
	////////////////////////////////////////////////
	
public:

	virtual void update() = 0;
};

}

#endif


