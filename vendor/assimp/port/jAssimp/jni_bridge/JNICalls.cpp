/*
---------------------------------------------------------------------------
Open Asset Import Library (ASSIMP)
---------------------------------------------------------------------------

Copyright (c) 2006-2008, ASSIMP Development Team

All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the following 
conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the ASSIMP team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the ASSIMP Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------
*/

/** @file Implementation of the JNI API for jAssimp */


// include the header files generated by javah
#include "assimp_Importer.h"

// include assimp
#include <aiTypes.h>
#include <aiMesh.h>
#include <aiAnim.h>
#include <aiScene.h>
#include <aiAssert.h>
#include <aiPostProcess.h>
#include <assimp.hpp>
#include <DefaultLogger.h>

// include all jAssimp internal header files
#include "JNIEnvironment.h"
#include "JNILogger.h"

using namespace Assimp;

#include <list>

namespace Assimp {
namespace JNIBridge {

// used as error return code
#define AI_JNI_ERROR_RETURN 0xffffffff

// typedef for a jassimp context, used to uniquely identify
// the Importer object which belongs to a java Importer

typedef uint64_t JASSIMP_CONTEXT;

#if (defined _DEBUG)

	typedef std::list< JASSIMP_CONTEXT > ImporterContextList;
	static ImporterContextList g_listActiveContexts;

// ------------------------------------------------------------------------------------------------
/* Used in debug builds to validate a context
*/
bool jValidateContext (JASSIMP_CONTEXT context)
{
	for (ImporterContextList::const_iterator
		i =  g_listActiveContexts.begin();
		i != g_listActiveContexts.end();++i)
	{
		if (context == *i)return true;
	}
	DefaultLogger::get()->error("[jnibridge] Invalid context");
	return false;
}

// ------------------------------------------------------------------------------------------------
/* Used in debug builds to validate a given scene
*/
bool jValidateScene (const aiScene* scene)
{
	if (!scene)
	{
		DefaultLogger::get()->error("[jnibridge] No asset loaded at the moment");
		return false;
	}
	return true;
}

#endif // ! ASSIMP_DEBUG

// ------------------------------------------------------------------------------------------------
/* Used in debug builds to validate a given scene
*/
Assimp::Importer* jGetValidImporterScenePair (JASSIMP_CONTEXT jvmcontext)
{
#if (defined _DEBUG)
	if (!jValidateContext((JASSIMP_CONTEXT)jvmcontext))return NULL;
#endif // ! ASSIMP_DEBUG

	// get the importer instance from the context
	Assimp::Importer* pcImp = (Assimp::Importer*)jvmcontext;

#if (defined _DEBUG)
	if (!jValidateScene(pcImp->GetScene()))return NULL;
#endif // ! ASSIMP_DEBUG
	return pcImp;
}

// ------------------------------------------------------------------------------------------------
/*
 * Class:     assimp_Importer
 * Method:    _NativeInitContext
 * Signature: ()I
 */
JNIEXPORT jlong JNICALL Java_assimp_Importer__1NativeInitContext
  (JNIEnv * jvmenv, jobject jvmthis)
{
	// 2^64-1 indicates error
	JASSIMP_CONTEXT context = 0xffffffffffffffffL;

	// create a new Importer instance
	Assimp::Importer* pcImp = new Assimp::Importer();
	context = (JASSIMP_CONTEXT)(uintptr_t)pcImp;

#if (defined _DEBUG)
	g_listActiveContexts.push_back(context);
#endif // ! ASSIMP_DEBUG

	// need to setup the logger
	JNILogDispatcher* pcLogger;
	if (DefaultLogger::isNullLogger())
	{
		pcLogger = new JNILogDispatcher();
		DefaultLogger::set (pcLogger);
	}
	else
	{
		JNILogDispatcher* pcLogger = ( JNILogDispatcher* )DefaultLogger::get();
		pcLogger->AddRef();
	}

	// setup the JNI environment  ...
	// simply setup the newest JNIEnv*
	if(!JNIEnvironment::Get()->AttachToCurrentThread(jvmenv))
		return 0xffffffffffffffffL;

	return context;
}

// ------------------------------------------------------------------------------------------------
/*
 * Class:     assimp_Importer
 * Method:    _NativeFreeContext
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_assimp_Importer__1NativeFreeContext
  (JNIEnv * jvmenv, jobject jvmthis, jlong jvmcontext)
{

#if (defined _DEBUG)
	if (!jValidateContext((JASSIMP_CONTEXT)jvmcontext))return AI_JNI_ERROR_RETURN;
#endif // ! ASSIMP_DEBUG

	// delete the Importer instance
	Assimp::Importer* pcImp = (Assimp::Importer*)jvmcontext;
	delete pcImp;

#if (defined _DEBUG)
	g_listActiveContexts.remove(jvmcontext);
#endif // ! ASSIMP_DEBUG

	JNIEnvironment::Get()->DetachFromCurrentThread();
	return 0;
}

// ------------------------------------------------------------------------------------------------
/*
 * Class:     assimp_Importer
 * Method:    _NativeLoad
 * Signature: (Ljava/lang/String;II)I
 */
JNIEXPORT jint JNICALL Java_assimp_Importer__1NativeLoad
  (JNIEnv *jvmenv, jobject jvmthis, jstring jvmpath, jint jvmflags, jlong jvmcontext)
{
	jint iRet = 0;

#if (defined _DEBUG)
	if (!jValidateContext((JASSIMP_CONTEXT)jvmcontext))return AI_JNI_ERROR_RETURN;
#endif // ! ASSIMP_DEBUG

	// get the path from the jstring
	const char* szPath = JNU_GetStringNativeChars(jvmenv,jvmpath);
	if (!szPath)
	{
		DefaultLogger::get()->error("[jnibridge] Unable to get path string from the java vm");
		return AI_JNI_ERROR_RETURN;
	}
	// get the importer instance from the context
	Assimp::Importer* pcImp = (Assimp::Importer*)jvmcontext;
	const aiScene* pcOut;

	// and load the file. The aiScene object itself remains accessible
	// via Importer.GetScene().
	if(!(pcOut = pcImp->ReadFile(std::string(szPath),(unsigned int)jvmflags)))
	{
		DefaultLogger::get()->error("[jnibridge] Unable to load asset");

		// release the path again
		free((void*)szPath);
		return AI_JNI_ERROR_RETURN;
	}

	// release the path again
	::free((void*)szPath);

	// allocate a new assimp.Scene object to be returned by the importer
	jobject jScene;
	if(!(jScene = jvmenv->AllocObject(AIJ_GET_HANDLE(assimp.Importer.Class))))
	{
		DefaultLogger::get()->error("[jnibridge] Unable to allocate output scene");
		return AI_JNI_ERROR_RETURN;
	}

	// fill the assimp.Scene instance
	JNIEnvironment::Get()->assimp.Scene.Fill(jScene,pcOut);

	// and store it in the Importer instance
	jvmenv->SetObjectField(jvmthis,AIJ_GET_HANDLE(assimp.Importer.scene),jScene);
	return iRet;
}

// ------------------------------------------------------------------------------------------------
/*
 * Class:     assimp_Importer
 * Method:    _NativeSetPropertyInt
 * Signature: (Ljava/lang/String;IJ)I
 */
JNIEXPORT jint JNICALL Java_assimp_Importer__1NativeSetPropertyInt
  (JNIEnv * jvmenv, jobject _this, jstring name, jint value, jlong jvmcontext)
{
#if (defined _DEBUG)
	if (!jValidateContext((JASSIMP_CONTEXT)jvmcontext))return AI_JNI_ERROR_RETURN;
#endif // ! ASSIMP_DEBUG

	Assimp::Importer* pcImp = (Assimp::Importer*)jvmcontext;
	const char* sz = JNU_GetStringNativeChars(jvmenv,name);
	pcImp->SetPropertyInteger(sz,(int)value,NULL);
	::free((void*)sz);
	return 0;
}


// ------------------------------------------------------------------------------------------------
/*
 * Class:     assimp_Importer
 * Method:    _NativeSetPropertyFloat
 * Signature: (Ljava/lang/String;FJ)I
 */
JNIEXPORT jint JNICALL Java_assimp_Importer__1NativeSetPropertyFloat
	(JNIEnv * jvmenv, jobject _this, jstring name, jfloat value, jlong jvmcontext)
{
#if (defined _DEBUG)
	if (!jValidateContext((JASSIMP_CONTEXT)jvmcontext))return AI_JNI_ERROR_RETURN;
#endif // ! ASSIMP_DEBUG

	Assimp::Importer* pcImp = (Assimp::Importer*)jvmcontext;
	const char* sz = JNU_GetStringNativeChars(jvmenv,name);
	pcImp->SetPropertyFloat(sz,(float)value,NULL);
	::free((void*)sz);
	return 0;
}

// ------------------------------------------------------------------------------------------------
/*
 * Class:     assimp_Importer
 * Method:    _NativeSetPropertyString
 * Signature: (Ljava/lang/String;Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_assimp_Importer__1NativeSetPropertyString
  (JNIEnv * jvmenv, jobject _this, jstring name, jstring value, jlong jvmcontext)
{
#if (defined _DEBUG)
	if (!jValidateContext((JASSIMP_CONTEXT)jvmcontext))return AI_JNI_ERROR_RETURN;
#endif // ! ASSIMP_DEBUG

	Assimp::Importer* pcImp = (Assimp::Importer*)jvmcontext;
	const char* sz  = JNU_GetStringNativeChars(jvmenv,name);
	const char* sz2 = JNU_GetStringNativeChars(jvmenv,value);
	pcImp->SetPropertyString(sz,sz2,NULL);
	::free((void*)sz);::free((void*)sz2);
	return 0;
}

}; //! namespace JNIBridge
}; //! namespace Assimp
