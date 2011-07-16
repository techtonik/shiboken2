/*
 * This file is part of the Shiboken Python Bindings Generator project.
 *
 * Copyright (C) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * Contact: PySide team <contact@pyside.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef BASEWRAPPER_H
#define BASEWRAPPER_H

#include <Python.h>
#include "python25compat.h"
#include "bindingmanager.h"
#include <list>
#include <map>
#include <string>

extern "C"
{

struct SbkObjectPrivate;

/// Base Python object for all the wrapped C++ classes.
struct LIBSHIBOKEN_API SbkObject
{
    PyObject_HEAD
    /// Instance dictionary.
    PyObject* ob_dict;
    /// List of weak references
    PyObject* weakreflist;
    SbkObjectPrivate* d;
};


/// Dealloc the python object \p pyObj and the C++ object represented by it.
LIBSHIBOKEN_API void SbkDeallocWrapper(PyObject* pyObj);
LIBSHIBOKEN_API void SbkDeallocWrapperWithPrivateDtor(PyObject* self);

struct SbkObjectType;

/// Function signature for the multiple inheritance information initializers that should be provided by classes with multiple inheritance.
typedef int* (*MultipleInheritanceInitFunction)(const void*);

/**
 *   Special cast function is used to correctly cast an object when it's
 *   part of a multiple inheritance hierarchy.
 *   The implementation of this function is auto generated by the generator and you don't need to care about it.
 */
typedef void* (*SpecialCastFunction)(void*, SbkObjectType*);
typedef SbkObjectType* (*TypeDiscoveryFunc)(void*, SbkObjectType*);

typedef void* (*ExtendedToCppFunc)(PyObject*);
typedef bool (*ExtendedIsConvertibleFunc)(PyObject*);

// Used in userdata dealloc function
typedef void (*DeleteUserDataFunc)(void*);

typedef void (*ObjectDestructor)(void*);

typedef void (*SubTypeInitHook)(SbkObjectType*, PyObject*, PyObject*);

extern LIBSHIBOKEN_API PyTypeObject SbkObjectType_Type;
extern LIBSHIBOKEN_API SbkObjectType SbkObject_Type;


struct SbkObjectTypePrivate;
/// PyTypeObject extended with C++ multiple inheritance information.
struct LIBSHIBOKEN_API SbkObjectType
{
    PyHeapTypeObject super;
    SbkObjectTypePrivate* d;
};

LIBSHIBOKEN_API PyObject* SbkObjectTpNew(PyTypeObject* subtype, PyObject*, PyObject*);

} // extern "C"

namespace Shiboken
{

/**
*   Init shiboken library.
*/
LIBSHIBOKEN_API void init();


/// Delete the class T allocated on \p cptr.
template<typename T>
void callCppDestructor(void* cptr)
{
    delete reinterpret_cast<T*>(cptr);
}

LIBSHIBOKEN_API bool        importModule(const char* moduleName, PyTypeObject*** cppApiPtr);
LIBSHIBOKEN_API void        setErrorAboutWrongArguments(PyObject* args, const char* funcName, const char** cppOverloads);

namespace ObjectType {

/**
*   Returns true if the object is an instance of a type created by the Shiboken generator.
*/
LIBSHIBOKEN_API bool        checkType(PyTypeObject* pyObj);

/**
*   Returns true if this object is an instance of an user defined type derived from an Shiboken type.
*/
LIBSHIBOKEN_API bool        isUserType(PyTypeObject* pyObj);

/**
*   Returns true if the constructor of \p ctorType can be called for a instance of type \p myType.
*   \note This function set a python error when returning false.
*/
LIBSHIBOKEN_API bool        canCallConstructor(PyTypeObject* myType, PyTypeObject* ctorType);

LIBSHIBOKEN_API void        setExternalCppConversionFunction(SbkObjectType* self, ExtendedToCppFunc func);
LIBSHIBOKEN_API void        setExternalIsConvertibleFunction(SbkObjectType* self, ExtendedIsConvertibleFunc func);
LIBSHIBOKEN_API bool        hasExternalCppConversions(SbkObjectType* self);
LIBSHIBOKEN_API bool        isExternalConvertible(SbkObjectType* self, PyObject* obj);
LIBSHIBOKEN_API void*       callExternalCppConversion(SbkObjectType* self, PyObject* obj);

LIBSHIBOKEN_API bool        hasCast(SbkObjectType* self);
LIBSHIBOKEN_API void*       cast(SbkObjectType* self, SbkObject* obj, PyTypeObject* target);
LIBSHIBOKEN_API void        setCastFunction(SbkObjectType* self, SpecialCastFunction func);

LIBSHIBOKEN_API void        setOriginalName(SbkObjectType* self, const char* name);
LIBSHIBOKEN_API const char* getOriginalName(SbkObjectType* self);

LIBSHIBOKEN_API void        setTypeDiscoveryFunction(SbkObjectType* self, TypeDiscoveryFunc func);
LIBSHIBOKEN_API TypeDiscoveryFunc getTypeDiscoveryFunction(SbkObjectType* self);

LIBSHIBOKEN_API void        copyMultimpleheritance(SbkObjectType* self, SbkObjectType* other);
LIBSHIBOKEN_API void        setMultipleIheritanceFunction(SbkObjectType* self, MultipleInheritanceInitFunction func);
LIBSHIBOKEN_API MultipleInheritanceInitFunction getMultipleIheritanceFunction(SbkObjectType* self);

LIBSHIBOKEN_API void        setDestructorFunction(SbkObjectType* self, ObjectDestructor func);

LIBSHIBOKEN_API void        initPrivateData(SbkObjectType* self);

/**
 *  Set the subtype init hook for a type.
 *
 *  This hook will be invoked every time the user creates a sub-type inherited from a Shiboken based type.
 *  The hook gets 3 params, they are: The new type being created, args and kwds. The last two are the very
 *  same got from tp_new.
 */
LIBSHIBOKEN_API void        setSubTypeInitHook(SbkObjectType* self, SubTypeInitHook func);

/**
 *  Get the user data previously set by Shiboken::Object::setTypeUserData
 */
LIBSHIBOKEN_API void*       getTypeUserData(SbkObjectType* self);
LIBSHIBOKEN_API void        setTypeUserData(SbkObjectType* self, void* userData, DeleteUserDataFunc d_func);
}

namespace Object {

/**
*   Returns true if the object is an instance of a type created by the Shiboken generator.
*/
LIBSHIBOKEN_API bool        checkType(PyObject* pyObj);
/**
 *  Returns true if this object type is an instance of an user defined type derived from an Shiboken type.
 *  \see Shiboken::ObjectType::isUserType
 */
LIBSHIBOKEN_API bool        isUserType(PyObject* pyObj);

/**
 *  Bind a C++ object to Python.
 * \param instanceType equivalent Python type for the C++ object.
 * \param hasOwnership if true, Python will try to delete the underlying C++ object when there's no more refs.
 * \param isExactType if false, Shiboken will use some heuristics to detect the correct Python type of this C++
 *                    object, in any case you must provide \p instanceType, it'll be used as search starting point
 *                    and as fallback.
 * \param typeName    If non-null, this will be used as helper to find the correct Python type for this object.
 */
LIBSHIBOKEN_API PyObject*   newObject(SbkObjectType* instanceType,
                                      void* cptr,
                                      bool hasOwnership = true,
                                      bool isExactType = false,
                                      const char* typeName = 0);

/**
 *  Changes the valid flag of a PyObject, invalid objects will raise an exception when someone tries to access it.
 */
LIBSHIBOKEN_API void        setValidCpp(SbkObject* pyObj, bool value);
/**
 *  Tells shiboken the Python object \p pyObj has a C++ wrapper used to intercept virtual method calls.
 */
LIBSHIBOKEN_API void        setHasCppWrapper(SbkObject* pyObj, bool value);
/**
 *  Return true if the Python object \p pyObj has a C++ wrapper used to intercept virtual method calls.
 */
LIBSHIBOKEN_API bool        hasCppWrapper(SbkObject* pyObj);

/**
 *  Return true if the Python is responsible for deleting the underlying C++ object.
 */
LIBSHIBOKEN_API bool        hasOwnership(SbkObject* pyObj);
/**
 *  Sets python as responsible to delete the underlying C++ object.
 *  \note You this overload only when the PyObject can be a sequence and you want to
 *  call this function for every item in the sequence.
 *  \see getOwnership(SbkObject*)
 */
LIBSHIBOKEN_API void        getOwnership(PyObject* pyObj);
/**
 *  Sets python as responsible to delete the underlying C++ object.
 */
LIBSHIBOKEN_API void        getOwnership(SbkObject* pyObj);

/**
 *  Release the ownership, so Python will not delete the underlying C++ object.
 *  \note You this overload only when the PyObject can be a sequence and you want to
 *  call this function for every item in the sequence.
 *  \see releaseOwnership(SbkObject*)
 */
LIBSHIBOKEN_API void        releaseOwnership(PyObject* pyObj);
/**
 *  Release the ownership, so Python will not delete the underlying C++ object.
 */
LIBSHIBOKEN_API void        releaseOwnership(SbkObject* pyObj);

/**
 *  Returns true if the pyObj holds information about their parents.
 */
LIBSHIBOKEN_API bool        hasParentInfo(SbkObject* pyObj);

/**
 *   Get the C++ pointer of type \p desiredType from a Python object.
 */
LIBSHIBOKEN_API void*       cppPointer(SbkObject* pyObj, PyTypeObject* desiredType);

/**
 *   Set the C++ pointer of type \p desiredType of a Python object.
 */
LIBSHIBOKEN_API bool        setCppPointer(SbkObject* sbkObj, PyTypeObject* desiredType, void* cptr);

/**
 * Returns false and sets a Python RuntimeError if the Python wrapper is not marked as valid.
 */
LIBSHIBOKEN_API bool        isValid(PyObject* pyObj);

/**
 * Returns false if the Python wrapper is not marked as valid.
 * \param pyObj the object.
 * \param throwPyError sets a Python RuntimeError when the object isn't valid.
 */
LIBSHIBOKEN_API bool        isValid(SbkObject* pyObj, bool throwPyError = true);

/**
 * Returns false if the Python wrapper is not marked as valid.
 * \param pyObj the object.
 * \param throwPyError sets a Python RuntimeError when the object isn't valid.
 */
LIBSHIBOKEN_API bool        isValid(PyObject* pyObj, bool throwPyError);

/**
*   Set the parent of \p child to \p parent.
*   When an object dies, all their children, grandchildren, etc, are tagged as invalid.
*   \param parent the parent object, if null, the child will have no parents.
*   \param child the child.
*/
LIBSHIBOKEN_API void        setParent(PyObject* parent, PyObject* child);

/**
*   Remove this child from their parent, if any.
*   \param child the child.
*/
LIBSHIBOKEN_API void        removeParent(SbkObject* child, bool giveOwnershipBack = true, bool keepReferenc = false);

/**
* \internal This is an internal function called by tp_dealloc, it's exported just for technical reasons.
* \note Do not call this function inside your bindings.
*/
LIBSHIBOKEN_API void        destroyParentInfo(SbkObject* obj, bool removeFromParent = true);

/**
 * Mark the object as invalid
 */
LIBSHIBOKEN_API void        invalidate(SbkObject* self);

/**
 * Help function can be used to invalidate a sequence of object
 **/
LIBSHIBOKEN_API void        invalidate(PyObject* pyobj);

/**
 * Make the object valid again
 */
LIBSHIBOKEN_API void        makeValid(SbkObject* self);

/**
 * Destroy any data in Shiboken structure and c++ pointer if the pyboject has the ownership
 **/
LIBSHIBOKEN_API void        destroy(SbkObject* self); //DEPRECATED
LIBSHIBOKEN_API void        destroy(SbkObject* self, void* cppData);

/**
 *  Set user data on type of \p wrapper.
 *  \param wrapper instance object, the user data will be set on his type
 *  \param userData the user data
 *  \param d_func a function used to delete the user data
 */
LIBSHIBOKEN_API void        setTypeUserData(SbkObject* wrapper, void* userData, DeleteUserDataFunc d_func);
/**
 *  Get the user data previously set by Shiboken::Object::setTypeUserData
 */
LIBSHIBOKEN_API void*       getTypeUserData(SbkObject* wrapper);

/**
 *   Increments the reference count of the referred Python object.
 *   A previous Python object in the same position identified by the 'key' parameter
 *   will have its reference counter decremented automatically when replaced.
 *   All the kept references should be decremented when the Python wrapper indicated by
 *   'self' dies.
 *   No checking is done for any of the passed arguments, since it is meant to be used
 *   by generated code it is supposed that the generator is correct.
 *   \param self            the wrapper instance that keeps references to other objects.
 *   \param key             a key that identifies the C++ method signature and argument where the referred Object came from.
 *   \param referredObject  the object whose reference is used by the self object.
 */
LIBSHIBOKEN_API void        keepReference(SbkObject* self, const char* key, PyObject* referredObject, bool append = false);

/**
 *   Removes any reference previously added by keepReference function
 *   \param self            the wrapper instance that keeps references to other objects.
 *   \param key             a key that identifies the C++ method signature and argument from where the referred Object came.
 *   \param referredObject  the object whose reference is used by the self object.
 */
LIBSHIBOKEN_API void        removeReference(SbkObject* self, const char* key, PyObject* referredObject);

} // namespace Object

} // namespace Shiboken

#endif // BASEWRAPPER_H

